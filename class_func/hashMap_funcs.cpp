#include "hashMap_funcs.h"


hashMap getHashMap(fstream &stream) {
    size_t len;
    stream.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (stream.eof()) throw runtime_error("data is broken");
    hashMap outHashMaap;
    size_t keySize;
    size_t valueSize;
    for (int i = 0; i < len; ++i) {
        stream.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        if (i != len - 1 && stream.eof()) throw runtime_error("data is broken");
        string key(keySize, ' ');
        stream.read(key.data(), keySize);
        stream.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
        if (i != len - 1 && stream.eof()) throw runtime_error("data is broken");
        string value(valueSize, ' ');
        stream.read(value.data(), valueSize);
        outHashMaap.insert(key, value);
    }
    return outHashMaap;
}


void hashMapToFile(const hashMap &hm, fstream &out) {
    auto size = hm.get_size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < hm.get_bucketsCount(); ++i) {
        for (int j = 0; j < hm[i].get_size(); ++j) {
            Pair dat = hm[i][j];
            size_t keySize = dat.key.size();
            out.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
            out.write(dat.key.c_str(), keySize);
            size_t valueSize = dat.value.size();
            out.write(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
            out.write(dat.value.c_str(), valueSize);
        }
    }
}


void hashSetInsert(const request& request){

    fstream file(request.file, ios::in | ios::binary);
    fstream tmpFile("data/tmp.data", ios::out | ios::binary);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 4) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string key = request.query[2];
    string value = request.query[3];
    bool varIsExist = false;
    char ch;
    string varName;
    hashMap var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '#') {
            varName = getVarName(file);
            var = getHashMap(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.insert(key, value);
            }
            tmpFile.put('#');
            nameToFile(varName, tmpFile);
            hashMapToFile(var, tmpFile);
        }
        else {
            tmpFile.put(ch);
        }
    }
    if (!varIsExist){
        hashMap newVar;
        newVar.insert(key, value);
        tmpFile.put('#');
        nameToFile(name, tmpFile);
        hashMapToFile(newVar, tmpFile);
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out | ios::binary);
    tmpFile.open("data/tmp.data", ios::in | ios::binary);
    while (true){
        ch = tmpFile.get();
        if (tmpFile.eof()) break;
        file << ch;
    }
    file.close();
    tmpFile.close();
}


void hashSetDel(const request& request){

    fstream tmpFile("data/tmp.data", ios::out | ios::binary);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    fstream file(request.file, ios::in);
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string key = request.query[2];
    bool varIsExist = false;
    char ch;
    string varName;
    hashMap var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '#') {
            varName = getVarName(file);
            var = getHashMap(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.del(key);
            }
            if (var.get_size() != 0) {
                tmpFile.put('#');
                nameToFile(varName, tmpFile);
                hashMapToFile(var, tmpFile);
            }
        }
        else {
            tmpFile.put(ch);
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "This hashMap doesn't exist" << endl;
    } else {
        file.open(request.file, ios::out | ios::binary);
        tmpFile.open("data/tmp.data", ios::in | ios::binary);
        while (true){
            ch = tmpFile.get();
            if (tmpFile.eof()) break;
            file << ch;
        }
        file.close();
        tmpFile.close();
    }
}


void hashSetGet(const request& request){

    fstream file(request.file, ios::in | ios::binary);
    if (request.query.get_size() == 3){
        string name = request.query[1];
        string key = request.query[2];
        char ch;
        string varName;
        hashMap var;
        while (true){
            ch = file.get();
            if (file.eof()) break;
            if (ch == '#') {
                varName = getVarName(file);
                var = getHashMap(file);
                if (varName == name) {
                    if (var.Get(key).key.empty()){
                        cout << "there is no value for this key" << endl;
                    }
                    else {
                        cout << var.Get(key) << endl;
                    }
                    return;
                }
            }
        }
        cout << "This hashMap isn't exist" << endl;
    }
    else throw runtime_error("Wrong command syntax");
}

void printHash(const request& request) {
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

            if (ch == '#') {
                varName = getVarName(file);
                hashMap var = getHashMap(file);
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

            if (ch == '#') {
                varName = getVarName(file);
                hashMap var = getHashMap(file);
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
            cout << "hashMap " << name << " not found" << endl;
        }
    } else {
        cout << "Wrong syntax" << endl;
    }

    file.close();
}
