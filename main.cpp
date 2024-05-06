#include <iostream>
#include <vector>
#include <ortools/linear_solver/linear_solver.h>
#include "csv.hpp"
#include "classes.h"

using namespace operations_research;
using namespace std;
using namespace csv;

vector<Block> import_blocks(string import_file) {
    Block sluzba = Block(0, {}, 1);
    Block seminare = Block(1, {}, 0);
    CSVReader reader(import_file);
    for (CSVRow& row : reader) {
        Lecture l = Lecture(row["id"].get<int>(), row["name"].get<string>(), row["time"].get<string>(), row["time2"].get<string>(), row["capacity"].get<int>());
        if (row["type"].get<int>() == 1) {
            sluzba.lectures.push_back(l);
        } else {
            seminare.lectures.push_back(l);
        }
    }
    return {sluzba, seminare};
}
vector<User> import_users(string import_file, vector<Block> blocks_to_pair) {
    vector<User> imported_users;
    CSVReader reader(import_file);
    int helpvar;
    for (CSVRow& row : reader) {
        if (row["id"].get<string>() == "" || row["tiecode"].get<string>() == "") { continue;}
        User new_user = User(row["id"].get<int>(), {}, row["tiecode"].get<string>());
        for (Block block : blocks_to_pair) {
            for (Lecture lecture : block.lectures) {
                if (row[lecture.name].get<string>() == "") { helpvar = 3;}
                else {helpvar = row[lecture.name].get<int>();}
                new_user.likes.push_back(helpvar);
            }
        }
        imported_users.push_back(new_user);
    }
    return imported_users;
}

int export_block(Block block, vector<vector<vector<MPVariable*>>> result, vector<User> users) {
    //invoke write stream
    ofstream ofile(to_string(block.type)  + ".csv");
    CSVWriter<ofstream> writer(ofile);

    // columns
    writer << vector<string>({"uid", "user_like","user_TIE", "lecture_id", "lecture_name", "lecture_time", "lecture_time2"});
    // write data
    Lecture user_lecture(-1, "", "", "", 0);
    for (User u : users) {
        for (Lecture l : block.lectures) {
            if (result[block.id][u.id][l.id]->solution_value() == 1) {user_lecture = l; break;}
        }
        if (user_lecture.id != -1) {
            writer << vector<string>({to_string(u.id), to_string(u.likes[user_lecture.id]),u.TIE, to_string(user_lecture.id), user_lecture.name, user_lecture.time, user_lecture.time2});
        }
    }
    return 0;
}
int main() {

    //import
    vector<Block> blocks = import_blocks("./blocks.csv");
    vector<User> users = import_users("./users.csv", blocks);

    // Variables
    const int num_users = static_cast<int>(users.size());
    int cache = 0;
    for (Block block : blocks) {
        cache += static_cast<int>(block.lectures.size());
    }
    const int num_lectures = cache;
    const int num_program_blocks = static_cast<int>(blocks.size());


    //transpose data (1->5 => 5->1)
    for (auto&& u : users) {
        for (auto&& like : u.likes) {
            like = abs(like-6);
        }
    }

    // Create the solver
    MPSolver solver("Lecture_Assignment", MPSolver::SAT_INTEGER_PROGRAMMING);

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
                objective->SetCoefficient(x[p.id][i.id][j.id], i.likes[ j.id]); // hledám lepší objective, než pouze průměr "happines" mezi lidmi, nicméně takto to funguje
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

    //User can be assigned to only one lecture per program block
    for (const User& i : users) {
        for (Block p : blocks) {
            MPConstraint* user_constraint = solver.MakeRowConstraint(1, 1);
            for (Lecture j : p.lectures) {
                user_constraint->SetCoefficient(x[p.id][i.id][j.id], 1);
            }
        }
    }


    // Solve the problem
    const MPSolver::ResultStatus result_status = solver.Solve();

    // Display the results

    if (result_status == MPSolver::OPTIMAL) {
        cout << "Optimal solution found:" << endl;
        //write results to files
        for (Block b : blocks) {
            export_block(b, x, users);
        }
        /*
        for (User i : users) {

            string un;
            string deux;
            int happiness = 0;
            for (Block p : blocks) {
                un += " ";
                deux += " ";
                for (Lecture j : p.lectures) {
                    un += to_string(i.likes[j.id]);
                    deux += to_string(x[p.id][i.id][j.id]->solution_value()*i.likes[j.id])[0];
                    happiness += x[p.id][i.id][j.id]->solution_value()*i.likes[j.id];
                }
            }
            //cout << i.TIE << "\n" << un << "\n" << deux << "\n " << to_string(happiness)<<" \n\n";
        }*/
        string cap;
        int capn;
        for (Block b : blocks) {
            for (Lecture l : b.lectures) {
                cap = l.name;
                capn = 0;
                for (int n = 0; n < 1508; n++) {
                    capn += x[b.id][n][l.id]->solution_value();
                }
                cout << cap + " : " + to_string(capn) + "/" + to_string(l.capacity) + "\n";
            }
        }

    } else {
        cout << to_string(result_status);
        cerr << "No optimal solution found." << endl; // this cant happen with this optimization f.- TODO: replace with debug print
    }

    return 0;
}
