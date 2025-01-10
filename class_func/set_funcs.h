#ifndef SET_FUNCS_H
#define SET_FUNCS_H

#include "generalFuncs.h"
#include "../classes/Set.h"

using namespace std;

void setToFile(const Set &set, fstream &out);
void setAdd(const request& request);
void setDel(const request& request);
void setAt(const request& request);
void printSet(const request& request);
#endif //SET_FUNCS_H
