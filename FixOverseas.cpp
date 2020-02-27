//
// Created by kwoodle on 1/1/20.
//
//#include <ktrade/Ksql.h>
//#include <ktrade/Util.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    const string root = "/home/kwoodle/Documents/Census/data/Apportionment2010/";
    const string csv_file = root + "2010CensusOverseasCounts_edited.csv";
    const string csv_outfile = root + "2010CensusOverseasCounts_fixed.csv";
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
    return 0;
}

