#include "list_funcs.h"

List<string> getList(fstream &stream) {
    size_t len;
    stream.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (stream.eof()) throw runtime_error("data is broken");
    List<string> outList(len);
    size_t size;
    for (int i = 0; i < outList.get_size(); ++i) {
        stream.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (i != outList.get_size() - 1 && stream.eof()) throw runtime_error("data is broken");
        string sDat(size, ' ');
        stream.read(sDat.data(), size);
        outList[i] = sDat;
    }
    return outList;
}

void listToFile(const List<string> &list, fstream &out) {
    auto size = list.get_size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < size; ++i) {
        auto dataSize = list[i].size();
        out.write(reinterpret_cast<char*>(&dataSize), sizeof(size_t));
        out.write(list[i].c_str(), list[i].size());
    }
}


void listPush(const request& request){

    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 4) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string place = request.query[2];
    if (place != "begin" && place != "end") throw runtime_error("Wrong insert place");
    string value = request.query[3];
    bool varIsExist = false;
    char ch;
    string varName;
    List<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '/') {
            varName = getVarName(file);
            var = getList(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                if (place == "begin"){
                    var.headInsert(value);
                }
                else {
                    var.backInsert(value);
                }
            }
            tmpFile.put('/');
            nameToFile(varName, tmpFile);
            listToFile(var, tmpFile);
        }
        else {
            tmpFile.put(ch);
        }
    }
    if (!varIsExist){
        List<string> newVar;
        newVar.headInsert(value);
        tmpFile.put('/');
        nameToFile(name, tmpFile);
        listToFile(newVar, tmpFile);
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


void listDel(const request& request){

    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string wh = request.query[2];
    bool varIsExist = false;
    char ch;
    string varName;
    List<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '/') {
            varName = getVarName(file);
            var = getList(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                if (wh == "begin") {
                    var.delFirst();
                }
                else if (wh == "end"){
                    var.delLast();
                }
                else {
                    var.delByVal(wh);
                }
            }
            if (var.get_size() != 0) {
                tmpFile.put('/');
                nameToFile(varName, tmpFile);
                listToFile(var, tmpFile);
            }
        }
        else {
            tmpFile.put(ch);
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis list doesn't exist" << endl;
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


void listGet(const request& request){

    fstream file(request.file, ios::in);
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    auto index = stoi(request.query[2]);
    char ch;
    string varName;
    List<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '/') {
            varName = getVarName(file);
            var = getList(file);
            if (varName == name) {
                cout << var[index] << endl;
                return;
            }
        }
    }
    cout << "This list isn't exist" << endl;
}

void printList(const request& request) {
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

            if (ch == '/') {
                varName = getVarName(file);
                List<string> var = getList(file);
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

            if (ch == '/') {
                varName = getVarName(file);
                List<string> var = getList(file);
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
            cout << "List " << name << " not found" << endl;
        }
    } else {
        cout << "Wrong syntax" << endl;
    }

    file.close();
}
