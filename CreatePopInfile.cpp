//
// Created by kwoodle on 11/23/19.
//
#include <ktrade/Ksql.h>
#include <ktrade/Util.h>

using namespace std;

int main() {
    bool do_build{true};       // does table need to be built?
    bool do_test(true);        // test if table is ok
    const string service = "localhost";
    const string user = "kwoodle";
    const string pass = "Rancity1!";
    const string table = "apportion";
    const string database = "census";

    drk::KSql kSql(service, user, pass);
    kSql.Execute("use " + database);

    const string csv_file = "/home/kwoodle/Documents/Census/data/est2018-01.csv";
    const string csv_outfile = "/home/kwoodle/Documents/Census/data/est2018fixed.csv";
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
            // and periods
            line.erase(std::remove(line.begin(), line.end(), '.'), line.end());
            cout << line << endl;
            ofile << line << endl;
        }
    }

    return 0;
}
