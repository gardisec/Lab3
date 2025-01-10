#include "arr_funcs.h"


arr<string> getArr(fstream &stream) {
    size_t len;
    size_t cap;
    stream.read(reinterpret_cast<char*>(&cap), sizeof(cap));
    stream.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (stream.eof()) throw runtime_error("data is broken");
    arr<string> outArr(len);
    size_t size;

    for (int i = 0; i < outArr.get_size(); ++i) {
        stream.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (i != outArr.get_size() - 1 && stream.eof()) throw runtime_error("data is broken");
        string sDat(size, ' '); //буффер
        stream.read(sDat.data(), size);
        outArr[i] = sDat;
    }
    return outArr;
}


void arrToFile(const arr<string> &array, fstream &out) {
    auto size = array.get_size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size)); //вписываем capacity
    out.write(reinterpret_cast<char*>(&size), sizeof(size)); //вписываем размер
    for (int i = 0; i < array.get_size(); ++i) {
        auto dataSize = array[i].size(); //расчитываем размер элемента
        out.write(reinterpret_cast<char*>(&dataSize), sizeof(size_t)); //записываем размер
        out.write(array[i].c_str(), array[i].size()); //записываем сам элемент
    }
}


void arrPush(const request& request){
    fstream file(request.file, ios::in | ios::binary);
    fstream tmpFile("data/tmp.data", ios::out | ios::binary);
    if(!tmpFile.is_open()) throw runtime_error("Tmp file doesn't exist");
    string name;
    string value;
    int index;
    if (request.query.get_size() == 3) {
        name = request.query[1];
        index = -1;
        value = request.query[2];
    }
    else if (request.query.get_size() == 4){
        name = request.query[1];
        index = stoi(request.query[2]);
        value = request.query[3];
    }
    else throw runtime_error("Wrong command syntax");
    bool varIsExist = false;
    char ch;
    string varName;
    arr<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break; //если полностью прочитали файл выходим
        if (ch == '@') { //проверяем спецсимвол массива

            varName = getVarName(file);
            var = getArr(file);
            if (varName == name && !varIsExist) { //если нашли нужную или её нет в файле то начинаем запись
                varIsExist = true;
                var.push_back(value);
                if (var.get_size() - 1 != index && index != -1){ //перемещаемся на корректное место для вставки
                    string tmp;
                    for(size_t i = var.get_size() - 1; i > index; --i){
                        tmp = var[i];
                        var[i] = var[i - 1];
                        var[i - 1] = tmp;
                    }
                }
            }
            tmpFile.put('@'); //вставляем спец символ
            nameToFile(varName, tmpFile); //вставляем имя
            arrToFile(var, tmpFile); //вставляем всё остальное
        }
        else {
            tmpFile.put(ch);
        }
    }
    if (!varIsExist){//если переменной ещё нет в файле

        if (index != 0 && index != -1){
            throw runtime_error("Wrong index");
        }
        arr<string> newVar;
        newVar.push_back(value);
        tmpFile.put('@'); //вставляем спец символ
        nameToFile(name, tmpFile); //вставляем имя
        arrToFile(newVar, tmpFile); //вставляем всё остальное
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out | ios::binary);
    tmpFile.open("data/tmp.data", ios::in  | ios::binary);
    while (true){
        ch = tmpFile.get();
        if (tmpFile.eof()) break; //выходим если файл кончился
        file << ch;
    }
    file.close();
    tmpFile.close();
}


void arrDel(const request& request){

    fstream file(request.file, ios::in | ios::binary);
    fstream tmpFile("data/tmp.data", ios::out | ios::binary);
    if(!tmpFile.is_open()) throw runtime_error("tmp.data file doesn't exist");
    if (request.query.get_size() != 3 || !isItNumber(request.query[2])) throw runtime_error("wrong command syntax");
    string name = request.query[1];
    size_t index = stoi(request.query[2]);
    bool varIsExist = false;
    char ch;
    string varName;
    arr<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '@') {
            varName = getVarName(file);
            var = getArr(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                var.del(index);
            }
            if (var.get_size() != 0) {
                tmpFile.put('@');
                nameToFile(varName, tmpFile);
                arrToFile(var, tmpFile);
            }
        }
        else {
            tmpFile.put(ch);
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "This array isn't exist" << endl;
    }
    else{
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


void arrGet(const request& request){

    fstream file(request.file, ios::in | ios::binary);
    if (request.query.get_size() != 3 || !isItNumber(request.query[2])) throw runtime_error("wrong command syntax");
    string name = request.query[1];
    size_t index = stoi(request.query[2]);
    bool varIsExist = false;
    char ch;
    string varName;
    arr<string> var;
    while (true){
        ch = file.get();
        if (file.eof()) break;
        if (ch == '@') {
            varName = getVarName(file);
            var = getArr(file);
            if (varName == name && !varIsExist) {
                varIsExist = true;
                if (index < var.get_size()) cout << var[index] << endl;
                else cout << "Index out of range" << endl;
                break;
            }
        }
    }
    if (!varIsExist){
        cout << "This array isn't exist" << endl;
    }
}

void printArr(const request& request) {
    fstream file(request.file, ios::in | ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file");
    }

    string targetVarName = (request.query.get_size() == 2) ? request.query[1] : "";
    bool isSingleVar = !targetVarName.empty();

    char ch;
    string varName;

    while (true) {
        ch = file.get();
        if (file.eof()) break;

        if (ch == '@') {
            varName = getVarName(file);
            arr<string> var = getArr(file);

            if (isSingleVar) {
                if (varName == targetVarName) {
                    cout << varName << ": " << var << '\n';
                    file.close();
                    return;
                }
            } else {
                cout << varName << ": " << var << '\n';
            }
        } else {
            char ch;
            size_t dataSize;
            while (file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize))) {
                file.seekg(dataSize, ios::cur);
                if (file.eof()) break;
            }
        }
    }

    file.close();

    if (isSingleVar) {
        cout << "Variable " << targetVarName << " not found" << endl;
    }
}
