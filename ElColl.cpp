//
// Created by kwoodle on 11/2/19.
//

#include "ElColl.h"

using namespace std;

int main() {
    // Get parameters from config file
    ifstream cfg("../elcoll.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("mysql.table_2010", po::value<string>())
            ("mysql.table_2018", po::value<string>())
            ("mysql.database", po::value<string>())
            ("files.csv_file", po::value<string>())
            ("files.csv_outfile", po::value<string>())
            ("cfg.do_build", po::value<bool>())
            ("cfg.do_test", po::value<bool>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    po::store(parse_config_file(cfg, desc, true), vm);
    notify(vm);
    cfg.close();
    const string table_2010 = vm["mysql.table_2010"].as<string>();
    const string table_2018 = vm["mysql.table_2018"].as<string>();  // total population and over 18 pop for 2018
    const string database = vm["mysql.database"].as<string>();

    bool do_build = vm["cfg.do_build"].as<bool>();
    bool do_test = vm["cfg.do_test"].as<bool>();
    drk::MySqlOptions opts;
    drk::KSql kSql(opts);
    kSql.Execute("use " + database);
    string hdr;
    const string csv_file = vm["files.csv_file"].as<string>();
    const string csv_outfile = vm["files.csv_outfile"].as<string>();
    if (do_test) {
        drk::Cols cols = kSql.get_cols("census", table_2010);
        for (auto &c:cols) {
            string col{c.first};
            hdr += col;
            hdr += ", ";
        }
        cout << "cols " << hdr << endl;
        cout << kSql.DisplayTable("census", table_2010, 60) << endl;
    }
    if (do_build) {
        ofstream ofile(csv_outfile);
        ifstream csv_in(csv_file);
        if (!csv_in.is_open()) {
            cout << "Failed to open csv file " << csv_file << endl;
            return 1;
        }
        string line;
        while (getline(csv_in, line)) {
            // remove those pesky commas
            line.erase(std::remove(line.begin(), line.end(), ','), line.end());
            cout << line << endl;
            ofile << line << endl;
        }
    }
    if (do_build) {
        kSql.Execute("use census");
        kSql.Execute("drop table if exists `apportion`");
        string create_connect_file {R"%%(
        create table `apportion`
(
    `STATE`      VARCHAR(50) NOT NULL,
    `POPULATION` INT         NOT NULL,
    `REPS`       SMALLINT    NOT NULL,
    `DELTA`      VARCHAR(5) DEFAULT NULL
) ENGINE=CONNECT DEFAULT CHARSET=latin1 `table_type`=CSV `file_name`=')%%"
        };
        create_connect_file += csv_outfile ;
        string create_connect_file_tail {R"%%(' `header`=1 `sep_char`=';')%%"};
        create_connect_file += create_connect_file_tail;
        cout<<"create_connect_file\n"<<create_connect_file<<endl;
        kSql.Execute(create_connect_file);
    }
    int total_pop{0};
    int total_reps{0};
    string sql0{"select population, reps from "};
    sql0 += table_2010;
    sql0 += " where state = 'TOTAL1'";
    auto res0 = kSql.ExecuteQuery(sql0);
    while (res0->next()) {
        total_pop = res0->getInt("population");
        total_reps = res0->getInt("reps");
    }
//    int total_electors = total_reps + 100;
    float reps_per_pop{float(total_reps) / total_pop};
    string sql1{"select TotRes from "};
    sql1 += table_2018;
    sql1 += " where area = 'United States'";
    auto res1 = kSql.ExecuteQuery(sql1);
    int uspop_2018{0};
    while (res1->next()) {
        uspop_2018 = res1->getInt("TotRes");
    }
    float reps_per_pop_2018{float(total_reps) / uspop_2018};
    const string sql3{R"%%(select t1.code, t2.state, t2.population, t2.reps, t3.TotRes from state_names as t1
inner join apportion as t2 on t1.state = t2.state
inner join over18_2018 as t3 on t2.state = t3.area order by population desc;
)%%"};
    auto res = kSql.ExecuteQuery(sql3);
    // output a row like code pop rep_act rep_excess rep_effective...
    const string ecolout{"../elcoll.gpdat"};
    ofstream ecollstrm{ecolout};
    if (!ecollstrm) {
        cerr << "Failed to open " << ecolout << endl;
        return 1;
    }
    ecollstrm << "code pop\trep_actual reps_deserved rep_excess\t\tpop_2018\t\tdeserved_2018 excess_2018" << endl;
    cout << "\ncode pop\trep_actual reps_deserved rep_excess\t\tpop_2018\t\tdeserved_2018 excess_2018" << endl;
    while (res->next()) {
        string code{res->getString("code")};
        string state{res->getString("state")};
        int ipop{res->getInt("population")};
        int ipop_2018{res->getInt("TotRes")};
        int reps{res->getInt("reps")};
        float reps_deserved{float(ipop) * reps_per_pop};
        float reps_desereved_2018{float(ipop_2018) * reps_per_pop_2018};
        float reps_excess{reps - reps_deserved};
        float reps_excess_2018{reps - reps_desereved_2018};
        if (state != "TOTAL1") {
            cout << code << " " << ipop << "\t\t" << reps << "\t\t" << reps_deserved << "\t\t" << reps_excess
                 << "\t\t" << ipop_2018 << "\t\t" << reps_desereved_2018 << "\t\t" << reps_excess_2018 << endl;
            ecollstrm << code << " " << ipop << "\t\t" << reps << "\t\t" << reps_deserved << "\t\t" << reps_excess
                 << "\t\t" << ipop_2018 << "\t\t" << reps_desereved_2018 << "\t\t" << reps_excess_2018 << endl;
        }
    }
    return 0;
}
