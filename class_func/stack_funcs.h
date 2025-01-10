#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include "generalFuncs.h"
#include "../classes/Stack.h"

using namespace std;

void stackToFile(const Stack<string> &stack, fstream &out);
void stackPush(const request& request);
void stackPop(const request& request);
void stackGet(const request& request);
void printStack(const request& request);

#endif //STACK_FUNCS_H
