#include "generalFuncs.h"

bool isFileExist(const string& fileName){
    std::ifstream file(fileName);
    if(!file.is_open()) return false;
    file.close();
    return true;
}

string delFirstLetter(const string &input){
    string output;
    bool z = false;
    for(auto ch : input){
        if(!z){
            z = true;
        }
        else {
            output += ch;
        }
    }
    return output;
}

commands commandRequest(const string& input){
    if (input == "push") return commands::push;
    if (input == "pop") return commands::pop;
    if (input == "print") return commands::print;
    if (input == "del") return commands::del;
    if (input == "Get") return commands::Get;
    if (input == "complete") return commands::complete;
    if (input == "search") return commands::Search;
    if (input == "set") return commands::set;
    if (input == "insert") return commands::insert;
    if (input == "add") return commands::add;
    if (input == "at") return commands::at;

    throw runtime_error("This command isn't exist");
}

structures structRequest(const string& input){
    if (input[0] == 'S' && input[1] == 'E' && input[2] == 'T') return structures::SET;
    if (input[0] == 'A') return structures::ARRAY;
    if (input[0] == 'L') return structures::LIST;
    if (input[0] == 'Q') return structures::QUEUE;
    if (input[0] == 'S') return structures::STACK;
    if (input[0] == 'H') return structures::HASHMAP;
    if (input[0] == 'T') return structures::TREE;
    throw runtime_error("This structure is not exist");
}

structures getType(const string& typeInFile){
    if (typeInFile == "#ARRAY") return structures::ARRAY;
    if (typeInFile == "#LIST") return structures::LIST;
    if (typeInFile == "#QUEUE") return structures::QUEUE;
    if (typeInFile == "#STACK") return structures::STACK;
    if (typeInFile == "#HASHMAP") return structures::HASHMAP;
    if (typeInFile == "#SET") return structures::SET;
    if (typeInFile == "#TREE") return structures::TREE;
    throw runtime_error("This structure isn't exist");
}

request getRequest(int argc, char *argv[] ){
    request output;
    for (int i = 0; i < argc; ++i){
        if (static_cast<string>(argv[i]) == "--file") {

            if (i + 1 == argc || argv[i + 1][0] == '-') {
                throw runtime_error(("Error: empty query.\n Specify what to execute"));
            }
            output.file = argv[i + 1];
            continue;
        }
        if (static_cast<string>(argv[i]) == "--query") {
            if (i + 1 == argc || argv[i + 1][0] == '-') {
                throw runtime_error(("Error: empty query.\n Specify what to execute"));
            }
            output.query = splitToArr(argv[i + 1]);
        }
    }
    return output;
}

bool isItNumber(const string& str) {
    for (auto ch : str) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

void nameToFile(const string &name, fstream &out) {
    size_t nameLen = name.size();
    out.write(reinterpret_cast<char*>(&nameLen), sizeof(nameLen)); //вписываем размер имени
    out.write(name.c_str(), name.size()); //вписываем имя
}

string getVarName(fstream &stream) {
    size_t varNameLen;
    stream.read(reinterpret_cast<char*>(&varNameLen), sizeof(varNameLen)); //получаем размер имени
    if (stream.eof()) throw runtime_error("data is broken");
    string varName(varNameLen, ' '); //создаем буффер
    stream.read(varName.data(), varNameLen); //считываем имя
    return varName;
}