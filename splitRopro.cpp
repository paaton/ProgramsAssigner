//
// Created by Ondřej Paatz on 02.05.2024.
//
#include "classes.h"
#include <iostream>
#include <vector>
#include <map>
#include <ortools/linear_solver/linear_solver.h>
#include "csv.hpp"


using namespace operations_research;
using namespace std;
using namespace csv;

/*
 * expected format of csv files:
 * groups-file => int id, string tiecode, string groupid, string grouptype
 * users_file => int uid,int user_like,string user_TIE,int lecture_id,string lecture_name,string lecture_time1, string lecture_time2*/
vector<RGroup> import_Rgroups(string groups_file, string users_file) {
    CSVReader groupsreader(groups_file);
    CSVReader usersreader(users_file);
    vector<RGroup> imported_groups = {};
    vector<RUser> imported_users = {};

    for (CSVRow& row : usersreader) {
        if (row['uid'].get<string>() == "") continue;
        imported_users.push_back(RUser(
                row['uid'].get<int>(),
                row['user_TIE'].get<string>(),
                row['lecture_id'].get<int>(),
                row['lecture_name'].get<string>(),
                row['lecture_time'].get<string>(),
                row['lecture_time2'].get<string>(),
                row['lecture_capacity'].get<int>()));
    }
    vector<RGroup> cache;
    string gid;
    map<string, string> TieGroupMap;

    for (CSVRow& row : groupsreader) {
        gid = row['groupid'].get<string>();  // nemám tušení, jestli to výkonu pomůže, či ne :D
        copy_if(imported_groups.begin(), imported_groups.end(), back_inserter(cache), [&gid](const RGroup& g) {return g.publicId == gid;});
        if (cache.empty()) {
            imported_groups.push_back(RGroup(row['id'].get<int>(), row['groupid'].get<string>(), {}, row['grouptype'].get<string>()));
        }
        TieGroupMap[row['tiecode'].get<string>()] = row['groupid'].get<string>();
    }
    map<string, string> TieGroupMapFiltered;
    for (RGroup &g : imported_groups) {

        copy_if(TieGroupMap.begin(), TieGroupMap.end(), inserter(TieGroupMapFiltered, TieGroupMapFiltered.end()), [g](auto const& pair) {return pair.second == g.publicId;});
        for (auto const& [key, val] : TieGroupMapFiltered) {
            auto it = find_if(imported_users.begin(), imported_users.end(), [&key](const RUser& u) {return u.TIE == key;});
            if (it != imported_users.end()) {
                g.users.push_back(*it);
            }
        }
    }
    return imported_groups;
};



int main() {

    //import
    vector<RGroup> Rgroups = import_Rgroups("./groups.csv", "./1.csv");

    // Variables
    const int num_groups = Rgroups.size();

    // Create the solver
    MPSolver solver("Lecture_Assignment", MPSolver::SAT_INTEGER_PROGRAMMING);
//TODO:Remake the linear problem for division of users to "sluzba blocks" based on their Ropro group
    /*
    vector<vector<vector<MPVariable*>>> x(num_program_blocks,
                                          vector<vector<MPVariable*>>(num_users,
                                                                      vector<MPVariable*>(num_lectures)));

    for (Block p : blocks) {
        for (User i : users) {
            for (Lecture j : p.lectures) {
                x[p.id][i.id][j.id] = solver.MakeIntVar(0, 1, "x_" + to_string(p.id) + "_" + to_string(i.id) + "_" + to_string(j.id));
            }
        }
    }

    //x[p][i][j] => p - program_block; i - user; j - lecture

    // Objective function:
    MPObjective* objective = solver.MutableObjective();
    for (Block p : blocks) {
        for (User i : users) {
            for (Lecture j : p.lectures) {
                objective->SetCoefficient(x[p.id][i.id][j.id], i.likes[j.id]);
            }
        }
    }
    objective->SetMaximization();

    // Capacity constraints
    for (Block p: blocks) {
        for (Lecture j : p.lectures) {
            MPConstraint* capacity_constraint = solver.MakeRowConstraint(0, j.capacity);
            for (User i: users) {
                capacity_constraint->SetCoefficient(x[p.id][i.id][j.id], 1);
            }
        }
    }

*/

    // Solve the problem
    const MPSolver::ResultStatus result_status = solver.Solve();

    // Display the results

    if (result_status == MPSolver::OPTIMAL) {
        cout << "Optimal solution found:" << endl;
        //write results to files
        //#TODO:export to same format as was input "users_file"


    } else {
        cout << to_string(result_status);
        cerr << "No optimal solution found." << endl; // this cant happen with this optimization f.- TODO: replace with debug print
    }

    return 0;
}