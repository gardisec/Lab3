#include "set_funcs.h"

#include "list_funcs.h"


Set getSet(fstream &stream) {
    size_t len;
    stream.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (stream.eof()) throw runtime_error("data is broken");
    Set outSet;
    int val;
    for (int i = 0; i < len; ++i) {
        stream.read(reinterpret_cast<char*>(&val), sizeof(val));
        if (i != len - 1 && stream.eof()) throw runtime_error("data is broken");
        outSet.insert(val);
    }
    return outSet;
}


void setToFile(const Set &set, fstream &out) {
    auto size = set.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < set.get_buckets(); ++i) {
        for (int j = 0; j < set[i].get_size(); ++j) {
            int dat = set[i][j];
            out.write(reinterpret_cast<char*>(&dat), sizeof(int));
        }
    }
}


void setAdd(const request& request) {

    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string value = request.query[2];
    if (!isItNumber(value)) {
        throw runtime_error("Wrong inserted value");
    }
    bool varIsExist = false;
    char ch;
    string varName;
    Set var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '|') {
            varName = getVarName(file);
            var = getSet(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.insert(stoi(value));
            }
            tmpFile.put('|');
            nameToFile(varName, tmpFile);
            setToFile(var, tmpFile);
        }
        else {
            tmpFile.put(ch);
        }
    }
    if (!varIsExist){
        cout << "making new set" << endl;
        Set newVar;
        newVar.insert(stoi(value));
        tmpFile.put('|');
        nameToFile(name, tmpFile);
        setToFile(newVar, tmpFile);
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out);
    tmpFile.open("data/tmp.data", ios::in);
    while (true){
        ch = tmpFile.get();
        if (tmpFile.eof()) break;
        file << ch;
    }
    file.close();
    tmpFile.close();
}

void setDel(const request& request) {

    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    fstream file(request.file, ios::in);
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    if (!isItNumber(request.query[2])) {
        throw runtime_error("Wrong inserted value");
    }
    int value = stoi(request.query[2]);
    bool varIsExist = false;
    char ch;
    string varName;
    Set var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '|') {
            varName = getVarName(file);
            var = getSet(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.del(value);
            }
            if (var.size() != 0) {
                tmpFile.put('|');
                nameToFile(varName, tmpFile);
                setToFile(var, tmpFile);
            }
        }
        else {
            tmpFile.put(ch);
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis set doesn't exist" << endl;
    } else {
        file.open(request.file, ios::out);
        tmpFile.open("data/tmp.data", ios::in);
        while (true){
            ch = tmpFile.get();
            if (tmpFile.eof()) break;
            file << ch;
        }
        file.close();
        tmpFile.close();
    }
}
void setAt(const request& request) {
    fstream file(request.file, ios::in);
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    if (!isItNumber(request.query[2])) {
        throw runtime_error("Wrong inserted value");
    }
    int value = stoi(request.query[2]);
    char ch;
    string varName;
    Set var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '|') {
            varName = getVarName(file);
            var = getSet(file);
            if (varName == name) {
                if (var.at(value)){
                    cout << "value " << value << " is in the set " << name << endl;
                }
                else {
                    cout << "value " << value << " isn't in the set " << name << endl;
                }
                return;
            }
        }
    }
    cout << "This set isn't exist" << endl;
}

void printSet(const request& request) {
    fstream file(request.file, ios::in | ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file");
    }

    if (request.query.get_size() == 1) {
        char ch;
        string varName;
        while (true) {
            ch = file.get();
            if (file.eof()) break;

            if (ch == '|') {
                varName = getVarName(file);
                Set var = getSet(file);
                cout << varName << ": " << var << '\n';
            } else {
                size_t skipSize;
                file.read(reinterpret_cast<char*>(&skipSize), sizeof(skipSize));
                file.seekg(skipSize, ios::cur);
            }
        }
    } else if (request.query.get_size() == 2) {
        string name = request.query[1];
        char ch;
        string varName;
        bool found = false;
        while (true) {
            ch = file.get();
            if (file.eof()) break;

            if (ch == '|') {
                varName = getVarName(file);
                Set var = getSet(file);
                if (varName == name) {
                    cout << varName << ": " << var << '\n';
                    found = true;
                    break;
                }
            } else {
                size_t skipSize;
                file.read(reinterpret_cast<char*>(&skipSize), sizeof(skipSize));
                file.seekg(skipSize, ios::cur);
            }
        }
        if (!found) {
            cout << "Set " << name << " not found" << endl;
        }
    } else {
        cout << "Wrong syntax" << endl;
    }

    file.close();
}