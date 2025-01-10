#ifndef QUEUE_FUNCS_H
#define QUEUE_FUNCS_H

#include "generalFuncs.h"
#include "../classes/queue.h"

using namespace std;

void queueToFile(const Queue<string> &queue, fstream &out);
void queuePush(const request& request);
void queuePop(const request& request);
void queueGet(const request& request);
void printQueue(const request& request);

#endif //QUEUE_FUNCS_H
