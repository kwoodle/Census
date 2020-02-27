//
// Created by kwoodle on 4/21/18.
//

#include <iostream>
#include <sstream>
#include <ktrade/Ksql.h>
#include <ktrade/Util.h>
#include "Census.h"

using namespace std;

int main() {
    // Get parameters from config file
    ifstream cfg("../census.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("mysql.census_database", po::value<string>())
            ("mysql.database", po::value<string>())
            ("mysql.table", po::value<string>())
            ("files.csv_file", po::value<string>())
            ("files.json_file", po::value<string>())
            ("mysql.service", po::value<string>())
            ("mysql.user", po::value<string>())
            ("cfg.do_build", po::value<string>())
            ("mysql.password", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    po::store(parse_config_file(cfg, desc, true), vm);
    notify(vm);
    cfg.close();
    string do_build = vm["cfg.do_build"].as<string>();
    if (do_build == "true") {
        int ret = build_age_table(vm);
        return ret;
    }
    // get zip codes from ward4 (active)
    // to compare with Census Age by Sex data
    //
    string zipsf{"../ward4_zips.txt"};
    string msql{"/usr/bin/mysql -e 'select distinct rzip5 from ward4active' test > " + zipsf};
    drk::get_from_cmd(msql);

    ifstream zs{zipsf};
    vector<string> zips;
    string line;
    while (getline(zs, line)) {
        zips.push_back(line);
    }
/*
 * select timestampdiff(year, dob, '2017-11-08') as age_11713 from test.ward4active
where voterhistory is not null and timestampdiff(year, dob, '2017-11-08') > 84 and RZIP5 = 11713
and dob <> '1900-01-01'order by age_11713 desc;

 */
    string service = vm["mysql.service"].as<string>();
    string user = vm["mysql.user"].as<string>();
    string pass = vm["mysql.password"].as<string>();
    string table = vm["mysql.table"].as<string>();
    string database = vm["mysql.database"].as<string>();

//    drk::KSql kSql(service, user, pass);
//    kSql.Execute("use "+database);

    return 0;
}

int build_age_table(po::variables_map &vm) {
    string service = vm["mysql.service"].as<string>();
    string user = vm["mysql.user"].as<string>();
    string pass = vm["mysql.password"].as<string>();
    string table = vm["mysql.table"].as<string>();
    string database = vm["mysql.census_database"].as<string>();
    string csv_file = vm["files.csv_file"].as<string>();
    string json_file = vm["files.json_file"].as<string>();

    // csv_file from Census Reporter https://censusreporter.org/
    ifstream csv_in(csv_file);
    if (!csv_in.is_open()) {
        cout << "Failed to open csv file " << csv_file << endl;
        return 1;
    }
    // metadata.json defines table columns
    ifstream json_in(json_file);
    if (!json_in.is_open()) {
        cout << "Failed to open json file " << json_file << endl;
        return 1;
    }
    string meta{drk::slurp(json_in)};
    json layout = json::parse(meta);
    json cols = layout["tables"]["B01001"]["columns"];
//    cout<<cols.dump(4)<<"\n";
    const string bq{"`"};
    vector<string> colnames;
    string cn;
    int ind;
    for (auto el:cols) {
//        cout<<el.dump(4)<<"\n";
        string nm{el["name"].get<string>()};
        int i{el["indent"].get<int>()};
        if (i == 0) {
            colnames.push_back(bq + nm + bq);
            colnames.push_back(bq + nm + " error" + bq);
        } else if (i == 1) {
            cn = nm;
            colnames.push_back(bq + nm + bq);
            colnames.push_back(bq + nm + " error" + bq);
        } else if (i == 2) {
            colnames.push_back(bq + cn + nm + bq);
            colnames.push_back(bq + cn + nm + " error" + bq);
        }
    }
//    for (auto c:colnames) {
//        cout << c << "\n";
//    }

    drk::KSql kSql(service, user, pass);
    kSql.Execute("use " + database);
    kSql.Execute("drop table if exists " + table);
    const string notnul{" int NOT NULL,\n"};
    string sql{"create table `"};
    sql += table + "` (\n";
    sql += "`geoid` varchar(100) NOT NULL,\n";
    sql += "`name` varchar(100) NOT NULL,\n";
    for (auto c:colnames) {
        sql += c + notnul;
    }
    sql.erase(sql.length() - 2, 2);
    sql += "\n)ENGINE=InnoDB DEFAULT CHARSET=latin1\n";
    cout << "sql for create table\n" << sql << "\n";
    kSql.Execute(sql);
    kSql.Execute("truncate " + table);
    string load_data{"load data local infile '"};
    load_data += csv_file + "\' ";
    load_data += "replace into table " + table;
    load_data += " fields terminated by ',' enclosed by '\"'\nignore 1 lines";
    cout << "sql for load data\n" << load_data << "\n";
    kSql.Execute(load_data);
    return 0;
}
