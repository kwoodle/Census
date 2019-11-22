//
// Created by kwoodle on 11/2/19.
//

#include "ElColl.h"
#include <iostream>
#include <sstream>
#include <ktrade/Ksql.h>
#include <ktrade/Util.h>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;
//using StateData = map<string, pair<int, int>>;
using StateData = multimap<int, pair<int, string>>;

//using StateData = map<string, tuple<int,int>>;
int main() {
//    bool do_build{true};
    bool do_build{true};       // does apportion table need to be built?
    bool do_test(true);        // test if apportion table is ok
    const string service = "localhost";
    const string user = "kwoodle";
    const string pass = "Rancity1!";
    const string table = "apportion";
    const string database = "census";

    drk::KSql kSql(service, user, pass);
    kSql.Execute("use " + database);
    string hdr;
    const string csv_file = "/home/kwoodle/Documents/Census/data/ApportionmentPop2010.csv";
    const string csv_outfile = "/home/kwoodle/Documents/Census/data/Apportionmentfixed2010.csv";
    if (do_test) {
        drk::Cols cols = kSql.get_cols("census", "apportion");
        for (auto &c:cols) {
            string col{c.first};
            hdr += col;
            hdr += ", ";
        }
        cout << "cols " << hdr << endl;
        cout << kSql.DisplayTable("census", "apportion", 60) << endl;
    }
    ofstream ofile(csv_outfile);
    if (do_build) {
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
        string load_data{"load data local infile '"};
        load_data += csv_outfile + "\' ";
        load_data += "replace into table " + table;
        load_data += " fields terminated by ';' ignore 1 lines";
        cout << "sql for load data\n" << load_data << "\n";
        kSql.Execute("truncate " + table);
        kSql.Execute(load_data);
    }
    int total_pop{0};
    int total_reps{0};
    const string sql0{"select population, reps from apportion where state = 'TOTAL1'"};
    auto res0 = kSql.ExcuteQuery(sql0);
    while (res0->next()) {
        total_pop = res0->getInt("population");
        total_reps = res0->getInt("reps");
    }
    int total_electors = total_reps + 100;
    float reps_per_pop{float(total_reps) / total_pop};
//    string sql{"select state, population, reps from apportion"};
    const string sql{R"%%(
select t1.code, t2.state, t2.population, t2.reps from state_names as t1
right join apportion as t2 on t1.state = t2.state order by population desc;
)%%"};

    auto res = kSql.ExcuteQuery(sql);
    StateData states;
    map<string, string> scodes;
    // output a row like code pop rep_act rep_excess rep_effective
    const string ecolout{"../elcoll.gpdat"};
    ofstream ecollstrm{ecolout};
    if (!ecollstrm) {
        cerr << "Failed to open " << ecolout << endl;
        return 1;
    }
    ecollstrm << "code pop rep_actual reps_deserved rep_excess" << endl;
    cout << "\ncode pop rep_actual reps_deserved rep_excess" << endl;
    while (res->next()) {
        string code{res->getString("code")};
        string state{res->getString("state")};
        int ipop{res->getInt("population")};
        int reps{res->getInt("reps")};
        float reps_deserved{float(ipop) * reps_per_pop};
        float reps_excess{reps - reps_deserved};
        if (state != "TOTAL1") {
            cout << code << " " << ipop << "\t\t" << reps << "\t\t" << reps_deserved << "\t\t" << reps_excess << endl;
            ecollstrm << code << " " << ipop << "\t\t" << reps << "\t\t" << reps_deserved << "\t\t" << reps_excess
                      << endl;
        }
//        continue;
    }
//    using SData = map<int, tuple<float, string, float>>;
//    using SData = vector<pair<string, tuple<float, int, float>>>;
//    SData sdata;
//    for (auto s:states) {
//        string nm{scodes[s.second.second]};
    //       if (nm != "") {
//        remember states.emplace(ipop, make_pair(reps, state));
//          float rep_pop{float(s.second.first) * reps_per_pop};
//            float rep_pop{float(s.first) * reps_per_pop};
//            auto rep_act{s.second.first};
//            float rep_excess{float(rep_act) - rep_pop};
//            float rep_effective(float(rep_act) + rep_excess);
//            sdata.emplace(make_pair())
//            cout<<nm<<" "<<rep_excess<<" "<<rep_act<<" "<<rep_effective<<endl;
//            ecollstrm<<nm<<" "<<rep_excess<<" "<<rep_act<<" "<<rep_effective<<endl;
//            sdata.push_back(make_pair(nm, make_tuple(rep_excess, rep_act, rep_effective)));
//        }
//    }
    return 0;
}
