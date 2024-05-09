//
// Created by Ondřej Paatz on 02.05.2024.
//
#include <iostream>
#include <vector>
using namespace std;

#ifndef PROGRAMSWITHOR_CLASSES_H
#define PROGRAMSWITHOR_CLASSES_H

#endif //PROGRAMSWITHOR_CLASSES_H



class User {
public:
    vector<int> likes;
    int id;
    string TIE;
    User(int id, vector<int> likes, string TIE);

};

class Lecture {
public:
    int id;
    string name;
    string time;
    string time2;
    int capacity;
    Lecture(int id, string name, string time, string time2, int capacity);
};

class Block {
public:
    int id;
    vector<Lecture> lectures;
    int type; //0 - normal, 1 - sluzba
    Block(int id, vector<Lecture> lectures, int type);
};

class RUser {
public:
    int id;
    Lecture lecture;
    string TIE;
    int lectureInstance;
    RUser(int id, string tie, int lectureId, string lectureName, string lectureTime, string lectureTime2, int lectureCapacity);

};
class RGroup {
public:
    int id;
    string publicId;
    vector<RUser> users;
    string type;
    RGroup(int id, string publicId, vector<RUser> users, string type);
};

vector<Block> import_blocks(string import_file);
vector<string> userExportColumns();
vector<string> putovaniTies();