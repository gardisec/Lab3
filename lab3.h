#ifndef LAB_1_H
#define LAB_1_H

#include "class_func/generalFuncs.h"
#include "class_func/arr_funcs.h"
#include "class_func/list_funcs.h"
#include "class_func/stack_funcs.h"
#include "class_func/queue_funcs.h"
#include "class_func/hashMap_funcs.h"
#include "class_func/set_funcs.h"
#include "class_func/tree_funcs.h"

#include <fstream>
#include <iostream>


string delFirstLetter(const string &input);

structures getType(const string& fName);

structures structRequest(const string& input);

commands commandRequest(const string& input);

request getRequest(int argc, char *argv[]);

#endif // LAB_1_H
