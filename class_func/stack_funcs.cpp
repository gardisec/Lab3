#include "stack_funcs.h"


Stack<string> getStack(fstream &stream) {
    size_t len;
    stream.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (stream.eof()) throw runtime_error("data is broken");
    Stack<string> outStack(len);
    size_t size;
    for (int i = 0; i < outStack.get_size(); ++i) {
        stream.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (i != outStack.get_size() - 1 && stream.eof()) throw runtime_error("data is broken");
        string sDat(size, ' ');
        stream.read(sDat.data(), size);
        outStack[i] = sDat;
    }
    return outStack;
}


void stackToFile(const Stack<string> &stack, fstream &out) {
    auto size = stack.get_size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < stack.get_size(); ++i) {
        auto dataSize = stack[i].size();
        out.write(reinterpret_cast<char*>(&dataSize), sizeof(size_t));
        out.write(stack[i].c_str(), stack[i].size());
    }
}


void stackPush(const request& request){

    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 3) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    string value = request.query[2];
    bool varIsExist = false;
    char ch;
    string varName;
    Stack<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == ']') {
            varName = getVarName(file);
            var = getStack(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.push(value);
            }
            tmpFile.put(']');
            nameToFile(varName, tmpFile);
            stackToFile(var, tmpFile);
        }
        else {
            tmpFile.put(ch);
        }
    }
    if (!varIsExist){
        cout << "making new Stack" << endl;
        Stack<string> newVar;
        newVar.push(value);
        tmpFile.put(']');
        nameToFile(name, tmpFile);
        stackToFile(newVar, tmpFile);
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


void stackPop(const request& request){
    fstream file(request.file, ios::in);
    fstream tmpFile("data/tmp.data", ios::out);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    if (request.query.get_size() != 2) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    bool varIsExist = false;
    char ch;
    string varName;
    Stack<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == ']') {
            varName = getVarName(file);
            var = getStack(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.pop();
            }
            if (var.get_size() != 0) {
                tmpFile.put(']');
                nameToFile(varName, tmpFile);
                stackToFile(var, tmpFile);
            }
        }
        else {
            tmpFile.put(ch);
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis stack doesn't exist" << endl;
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


void stackGet(const request& request){

    fstream file(request.file, ios::in);
    if (request.query.get_size() != 2) throw runtime_error("Wrong command syntax");
    string name = request.query[1];
    bool varIsExist = false;
    char ch;
    string varName;
    Stack<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == ']') {
            varName = getVarName(file);
            var = getStack(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                cout << var.getLast() << endl;
                break;
            }
        }
    }
    if (!varIsExist){
        cout << "This stack isn't exist" << endl;
    }
}

void printStack(const request& request) {
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

            if (ch == ']') {
                varName = getVarName(file);
                Stack<string> var = getStack(file);
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

            if (ch == ']') {
                varName = getVarName(file);
                Stack<string> var = getStack(file);
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
            cout << "Stack " << name << " not found" << endl;
        }
    } else {
        cout << "Wrong syntax" << endl;
    }

    file.close();
}