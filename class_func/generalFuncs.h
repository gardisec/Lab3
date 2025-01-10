#ifndef GENERAL_FUNCS_H
#define GENERAL_FUNCS_H


#include <iostream>
#include <fstream>
#include <utility>
#include "../classes/arr.h"

using namespace std;

enum commands{
    Get,
    push,
    pop,
    del,
    insert,
    set,
    add,
    at,
    Search,
    complete,
    print
};


enum structures{
    ARRAY,
    LIST,
    QUEUE,
    STACK,
    HASHMAP,
    SET,
    TREE
};


struct request{
    arr<string> query;
    string file;
};


struct fileData{
    string type;
    string name;
    string data;

    fileData () = default;
    fileData (string t, string n, string d) : type(t), name(n), data(d) {};

    void getVarInfo(const string &input) {
        arr<string> split;
        split = splitToArr(input, ';');
        if(split.get_size() != 3 || split[0][0] != '#') {
            throw runtime_error("Error in fileData: data is broken");
        }
        this->type = split[0];
        this->name = split[1];
        this->data = split[2];
    }
};

bool isFileExist(const string& fileName);
bool isItNumber(const string& str);
void nameToFile(const string &name, fstream &out);
string getVarName(fstream &stream);

#endif //GENERAL_FUNCS_H
