#include "queue_funcs.h"

Queue<string> getQueue(fstream &stream) {
    size_t len;
    stream.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (stream.eof()) throw runtime_error("data is broken");
    Queue<string> outQueue(len);
    size_t size;
    for (int i = 0; i < outQueue.get_size(); ++i) {
        stream.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (i != outQueue.get_size() - 1 && stream.eof()) throw runtime_error("data is broken");
        string sDat(size, ' ');
        stream.read(sDat.data(), size);
        outQueue[i] = sDat;
    }
    return outQueue;
}

void queueToFile(const Queue<string> &queue, fstream &out) {
    auto size = queue.get_size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < queue.get_size(); ++i) {
        auto dataSize = queue[i].size();
        out.write(reinterpret_cast<char*>(&dataSize), sizeof(size_t));
        out.write(queue[i].c_str(), queue[i].size());
    }
}


void queuePush(const request& request){

    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error( "Tmp file doesn't exist");
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string value = request.query[2];
    bool varIsExist = false;
    char ch;
    string varName;
    Queue<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '\\') {
            varName = getVarName(file);
            var = getQueue(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.push(value);
            }
            tmpFile.put('\\');
            nameToFile(varName, tmpFile);
            queueToFile(var, tmpFile);
        }
        else {
            tmpFile.put(ch);
        }
    }
    if (!varIsExist){
        Queue<string> newVar;
        newVar.push(value);
        tmpFile.put('\\');
        nameToFile(name, tmpFile);
        queueToFile(newVar, tmpFile);
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


void queuePop(const request& request){
    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 2) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    bool varIsExist = false;
    char ch;
    string varName;
    Queue<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '\\') {
            varName = getVarName(file);
            var = getQueue(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.pop();
            }
            if (var.get_size() != 0) {
                tmpFile.put('\\');
                nameToFile(varName, tmpFile);
                queueToFile(var, tmpFile);
            }
        }
        else {
            tmpFile.put(ch);
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis queue doesn't exist" << endl;
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


void queueGet(const request& request){

    fstream file(request.file, ios::in);
    if (request.query.get_size() != 2) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    char ch;
    string varName;
    Queue<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '\\') {
            varName = getVarName(file);
            var = getQueue(file);
            if (varName == name) {
                cout << var.getFirst() << endl;
                return;
            }
        }
    }
    cout << "This queue isn't exist" << endl;
}

void printQueue(const request& request) {
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

            if (ch == '\\') {
                varName = getVarName(file);
                Queue<string> var = getQueue(file);
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

            if (ch == '\\') {
                varName = getVarName(file);
                Queue<string> var = getQueue(file);
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
            cout << "Queue " << name << " not found" << endl;
        }
    } else {
        cout << "Wrong syntax" << endl;
    }

    file.close();
}
