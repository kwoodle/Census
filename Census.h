//
// Created by kwoodle on 4/21/18.
//

#ifndef CENSUS_CENSUS_H
#define CENSUS_CENSUS_H

#include <string>
#include <map>
#include <vector>
#include <ktrade/nlohmann/json.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

using std::string;
using std::vector;
using std::map;
using json = nlohmann::json;

int build_age_table(po::variables_map &vm);

/*  From metadata.json
 *             "columns": {
                "B01001001": {
                    "name": "Total:",
                    "indent": 0
                },
                "B01001002": {
                    "name": "Male:",
                    "indent": 1
                },
                "B01001003": {
                    "name": "Under 5 years",
                    "indent": 2
                },
                "B01001004": {
                    "name": "5 to 9 years",
                    "indent": 2
                },

 */
#endif //CENSUS_CENSUS_H
