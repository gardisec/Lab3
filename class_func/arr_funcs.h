#ifndef ARR_FUNCS_H
#define ARR_FUNCS_H

#include "../classes/arr.h"
#include "generalFuncs.h"

using namespace std;

void arrToFile(const arr<string> &array, fstream &out);
void arrPush(const request& request);
void arrDel(const request& request);
void arrGet(const request& request);
void printArr(const request& request);

#endif //ARR_FUNCS_H
