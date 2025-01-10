#ifndef LIST_FUNCS_H
#define LIST_FUNCS_H

#include "generalFuncs.h"
#include "../classes/list.h"

using namespace std;

void listToFile(const List<string> &list, fstream &out);
void listPush(const request& request);
void listDel(const request& request);
void listGet(const request& request);
void printList(const request& request);
#endif //LIST_FUNCS_H
