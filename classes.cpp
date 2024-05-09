//
// Created by Ondřej Paatz on 02.05.2024.
//
#include "classes.h"
#include <iostream>
#include <vector>
#include "csv.hpp"

using namespace std;
using namespace csv;

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

vector<string> userExportColumns() {
    return {"uid", "user_like","user_TIE", "lecture_id", "lecture_name", "lecture_time", "lecture_time2"};
};
vector<string> putovaniTies() {
    return {"AZOWWF", "QHDQFX", "TKJFZS", "PZMZGD", "QMNNJD", "JTEJAE", "QDHVEK", "TGKTQU", "LEYEQG", "CFLANR", "HYHNAZ", "WROPZE", "DOCAKY", "JUBHNU", "ZEDUCW", "AGSWFO", "ETPGBK", "NZMSMA", "KGBYPC", "XWFANN", "MQUFXR", "QEROSF", "STEDDY", "PXGMPD", "JVBUJB", "JYMSAN", "UXEJKU", "OFTMUZ", "FKCMWA", "GVNEFX", "WNWBLX", "JVBDTM", "LJXATF", "PFNHXS", "QXRULH", "CPLASU", "OESXYH", "SBHRPL", "GGOZVZ", "SYRKFQ", "DGGBYF", "UFRLST", "HYAJSY", "AHTDFN", "ATLQSE", "VRNRQX", "QPWWQA", "WDCZQR", "ZXJZTW", "ULLFUJ", "KHNAFY", "SZDQAQ", "WXWJAY", "NBFTGN", "QTEQTU", "OJPBCJ", "RABDML", "ZFKTFN", "PPYQWQ", "GBSPOD", "SGHUHA", "CESHKH", "DJZBQN", "ZDTYNQ", "YBDJPV", "QWMRWA", "ZZMYZT", "OKBCER", "LWJXXD", "JNEGKS", "VUEKBV", "MZUOEX", "AXMUBT", "SYRQYD", "UASAFZ", "ZBCVZQ", "BCWCDT", "LXYQUC", "WTERQX", "JMTZJG", "XSUTUJ", "EYASYO", "DDCRBP", "QQRVLQ", "TRXPYZ", "CCKBPC", "BTFKCY", "QGPJFQ", "JXPLXD", "YLNFCP", "GHDJVD", "SLBJZL", "REQHYY", "ZJUKGU", "MKTATP", "PJVRQZ", "MFCDYN", "BNMNCG", "NLPBRY", "HAMVYU", "ZCJKKM", "KLBTVF", "ENEVCQ", "XCCELM", "JWDZXK", "KCWPLJ", "CYWTBE", "KPMUJL", "QLSYSU", "JCTKJB", "HGLLOO", "JWLFXD", "UTYUFZ", "MSFLDX", "NFXZMJ", "UDQXLT", "ECOYHY", "ELLXCP", "KPFXDW", "MLZTBF", "AKVXZY", "MBVVJJ", "CXURGR"};
};