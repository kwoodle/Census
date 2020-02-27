//
// Created by kwoodle on 12/5/19.
//

#include "ElColl.h"
#include <boost/range/counting_range.hpp>
#include <boost/range/irange.hpp>
#include <limits>

using namespace std;

map<int, double> multipliers;

// lambda for comparator for set with States as keys
// larger state first
auto StateCompare = [](const State &state1, const State &state2) -> bool {
    return state1.pop > state2.pop;
};

void calc_reps(set<State, decltype(StateCompare)> &states) {
    // 50 reps have been allocated. 435 left
    int imin{51};
    int imax{436};
    for (int n = imin; n < imax; ++n) {
        double max_priority{numeric_limits<double>::min()};
        string st;
        State topstate;
        for (auto &e : states) {
            double spri = e.pop * multipliers[e.reps + 1];
            if (spri > max_priority) {
                max_priority = spri;
                topstate = e;
            }
        }
        ++topstate.reps;
        states.erase(topstate);
        states.insert(topstate);
    }
}

int main() {

    // for apportionment by Equal proportions method
    // initialize multipliers [reciprocal of the geometric mean]
    // Each state gets one so lowest 'next' rep is 2
    // int2 represents highest conceivable number of reps for any individual state.
    int int_s1{2};
    int int_s2{60};
    for (int n : boost::irange(int_s1, int_s2)) {
        multipliers[n] = multiplier(n);
    }

    drk::KSql kSql;
    kSql.Execute("use census");

    const string sql{R"%%(select t1.code, t2.population, t2.reps from state_names as t1
inner join apportion as t2 on t1.state = t2.state order by population desc;
)%%"};
    auto res0{kSql.ExcuteQuery(sql)};
    set<State, decltype(StateCompare)> states0(StateCompare);
    while (res0->next()) {
        string c = res0->getString("code");
        int ipop{res0->getInt("population")};
        State s(c);
        s.pop = ipop;
        states0.insert(s);
    }
    calc_reps(states0);

    const string sql3{R"%%(select t1.code, t3.TotRes from state_names as t1
inner join apportion as t2 on t1.state = t2.state
inner join over18_2018 as t3 on t2.state = t3.area order by TotRes desc;
)%%"};
    auto res{kSql.ExcuteQuery(sql3)};
    set<State, decltype(StateCompare)> states(StateCompare);
    while (res->next()) {
        string c = res->getString("code");
        int ipop_2018{res->getInt("TotRes")};
        State s(c);
        s.pop = ipop_2018;
        states.insert(s);
    }

    // 50 reps have been allocated. 435 left
    calc_reps(states);
    int reps_tot{0};
    cout << "code \tr2010 \tpop2010\t\tr2018\tpop2018\n";
    for (auto st:states) {
        for (auto st0:states0) {
            if (st.code == st0.code) {
                cout << st.code << "\t" << st0.reps << "\t" << st0.pop << "\t\t" <<
                     st.reps << "\t" << st.pop << endl;
            }
        }
        reps_tot += st.reps;
    }
    cout << "reps_tot " << reps_tot;
    return 0;
}

double multiplier(int n) {
    if (n < 2)
        throw std::range_error("multplier(n) requires n>1");
    return 1. / sqrt(n * (n - 1));
}

