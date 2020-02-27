//
// Created by kwoodle on 12/5/19.
//

#ifndef CENSUS_ELCOLL_H
#define CENSUS_ELCOLL_H
#include <iostream>
#include <ktrade/Ksql.h>
#include <ktrade/Util.h>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
extern double multiplier(int n);

struct State {
    int pop;
    int reps;
    string code;
    // every state gets at least one rep
    State(string c) : code(c), reps(1){};
    State(const State&) = default;
    State(State&) = default;
    State() = default;
    State& operator=(const State&) = default;
};

#endif //CENSUS_ELCOLL_H
