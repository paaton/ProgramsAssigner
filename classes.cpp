//
// Created by Ondřej Paatz on 02.05.2024.
//
#include "classes.h"
#include <iostream>
#include <vector>

using namespace std;

User::User(int id, vector<int> likes, string TIE) {
    this->likes = likes;
    this->id = id;
    this->TIE = TIE;
}
Lecture::Lecture(int id, string name, string time, string time2, int capacity) {
    this->id = id;
    this->name = name;
    this->time = time;
    this->time2 = time2;
    this->capacity = capacity;
}
Block::Block(int id, vector<Lecture> lectures, int type) {
    this->id = id;
    this->lectures = lectures;
    this->type = type;
}
RUser::RUser (int id,
             std::string tie,
             int lectureId,
             string lectureName,
             string lectureTime,
             string lectureTime2,
             int lectureCapacity) : lecture(lectureId, lectureName, lectureTime, lectureTime2, lectureCapacity) {
    this->id = id;
    this->lectureInstance = 0;
}

RGroup::RGroup(int id, std::string publicId, vector<RUser> users, std::string type) {
    this->id  = id;
    this->publicId = publicId;
    this->users = users;
    this->type = type;
}