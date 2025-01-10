#include "arr.h"

template <typename T>
void arr<T>::realoc(){
    size_t newCapacity;
    if (capacity == 0){
        newCapacity = 1;
    }
    else {
        newCapacity = capacity * 2;
    }
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    if (data) {
        delete[] data;
    }
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void arr<T>::push_back(const T& value) {
    if (size >= capacity) {
        realoc();
    }
    data[size++] = value;
}

template <typename T>
size_t arr<T>::get_size() const  {
    return size;
}

template <typename T>
void arr<T>::erase() {
    if (size == 0) {
        throw std::out_of_range("Array is empty");
    }
    --size;
}

template <typename T>
void arr<T>::del(const size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    for (size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1]; // Сдвигаем элементы влево
    }
    size--;
}

template<typename T>
void arr<T>::clear() {
    delete this->data;
    this-> capacity = 0;
    this->size = 0;
    this->realoc();
}


arr<string> splitToArr(const string &input, const char delimiter){
    string word;
    arr<string> output;
    for (auto ch : input){
        if (ch == delimiter) {
            output.push_back(word);
            word = "";
        }
        else{
            word += ch;
        }
    }
    if (word != ""){
        output.push_back(word);
    }
    return output;
}

arr<string> splitToArr(const string& input, const string& delimiter){
    string word;
    arr<string> output;
    bool isDelim;
    int j;
    for (int i = 0; i < input.size(); ++i){
        if (input[i] == delimiter[0]){
            isDelim = true;
            for (j = 0; j < delimiter.size(); ++j){
                if (delimiter[j] != input[i + j]) isDelim = false;
            }
            if (isDelim){
                output.push_back(word);
                word = "";
                i += j - 1;
            }
            else {
                word += input[i];
            }
        }
        else {
            word += input[i];
        }
    }
    if (word != ""){
        output.push_back(word);
    }
    return output;
}

string unsplit(const arr<string>& array, const char delimiter){
    string output;
    for (int i = 0; i < array.get_size(); ++i){
        if (i + 1 != array.get_size()) {
            output += array[i] + delimiter;
        }
        else {
            output += array[i];
        }
    }
    return output;
}
