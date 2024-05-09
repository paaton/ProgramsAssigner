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
 * groups-file => string tiecode, string groupid, string grouptype
 * users_file => int uid,int user_like,string user_TIE,int lecture_id,string lecture_name,string lecture_time1, string lecture_time2*/
vector<RGroup> import_Rgroups(string groups_file, string users_file) {
    CSVReader groupsreader(groups_file);
    CSVReader usersreader(users_file);
    vector<RGroup> imported_groups = {};
    vector<RUser> imported_users = {};

    for (CSVRow& row : usersreader) {
        if (row["uid"].get<string>() == "") continue;
        imported_users.push_back(RUser(
                row["uid"].get<int>(),
                row["user_TIE"].get<string>(),
                row["lecture_id"].get<int>(),
                row["lecture_name"].get<string>(),
                row["lecture_time"].get<string>(),
                row["lecture_time2"].get<string>(),
                row["lecture_capacity"].get<int>()));
    }
    vector<RGroup> cache;
    string gid;
    map<string, string> TieGroupMap;
    int groupIdIterator  = 0;
    for (CSVRow& row : groupsreader) {
        gid = row["groupid"].get<string>();  // nemám tušení, jestli to výkonu pomůže, či ne :D
        copy_if(imported_groups.begin(), imported_groups.end(), back_inserter(cache), [&gid](const RGroup& g) {return g.publicId == gid;});
        if (cache.empty()) {
            imported_groups.push_back(RGroup(groupIdIterator, row["groupid"].get<string>(), {}, row["grouptype"].get<string>()));
            groupIdIterator++;
        }
        TieGroupMap[row["tiecode"].get<string>()] = row["groupid"].get<string>();
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


int export_block_from_sluzba_division(string output_file, vector<RUser> RUsers) {

    //invoke write stream
    ofstream ofile(output_file);
    CSVWriter<ofstream> writer(ofile);

    // columns
    writer << userExportColumns();

    // write data
    for (RUser u : RUsers) {
        if (u.lectureInstance != 0) {
            u.lecture.time = u.lecture.time2;
        }
        writer << vector<string>({to_string(u.id), "N/A",u.TIE, to_string(u.lecture.id), u.lecture.name, u.lecture.time, "N/A"});
     }
    return 0;
}


int main() {

    //import
    vector<RGroup> Rgroups = import_Rgroups("./groups.csv", "./1.csv");
    vector<Lecture> lectures = import_blocks("./blocks.csv")[0].lectures; //blocks not alwaays ordered TODO: Get "služba" block by find_if

    // Variables
    const int num_groups = Rgroups.size();
    const int num_lectures = lectures.size();
    const int max_group_size = 15;

    // Create the solver
    MPSolver solver("Lecture_Assignment", MPSolver::SAT_INTEGER_PROGRAMMING);

    vector<MPVariable*> x(num_groups);
    vector<MPVariable*> y(num_lectures);
    // groups variable
    for (RGroup g : Rgroups) {
            x[g.id] = solver.MakeIntVar(0, 1, "x_" + to_string(g.id)); // works only with group itself - whole group a) goes, b) does no go (actually goes in the second Block of Sluzba
        }


    //x[g] => g - group;

    // Objective function: TODO: rewrite to strike for lowest difference between capacity and assignment
    MPObjective* objective = solver.MutableObjective();
    for (const RGroup g : Rgroups) {
            objective->SetCoefficient(x[g.id], 1);
        }
    objective->SetMaximization();

    for (Lecture l : lectures) {
        MPConstraint *capacity_constraint = solver.MakeRowConstraint(l.capacity/2, l.capacity/2);
        for (RGroup g : Rgroups) {
            for (RUser u : g.users) {
                if (u.lecture.id == l.id) {
                    capacity_constraint->SetCoefficient(x[g.id], 1);
                }
            }
        }
    }

    // Solve the problem
    const MPSolver::ResultStatus result_status = solver.Solve();

    // Display the results

    if (result_status == MPSolver::OPTIMAL) {
        cout << "Optimal solution found:" << endl;
        for (const RGroup g : Rgroups) {
            for (RUser u : g.users) {
                u.lectureInstance = x[g.id]->solution_value();
            }


        }

        //write results to files TODO: export func to export sluzba block with information about time and also ropro block containing their group and



    } else {
        cout << to_string(result_status);
        cerr << "No optimal solution found." << endl; // this cant happen with this optimization f.- TODO: replace with debug print
    }

    return 0;
}