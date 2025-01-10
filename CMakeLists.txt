cmake_minimum_required(VERSION 3.29)

project(Lab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab3
        classes/arr.cpp
        classes/hashMap.cpp
        classes/list.cpp
        classes/Pair.cpp
        classes/singleConnList.cpp
        classes/Stack.cpp
        classes/Set.cpp
        classes/ComplTree.cpp

        class_func/arr_funcs.cpp
        class_func/generalFuncs.cpp
        class_func/hashMap_funcs.cpp
        class_func/list_funcs.cpp
        class_func/queue_funcs.cpp
        class_func/stack_funcs.cpp
        class_func/set_funcs.cpp
        class_func/tree_funcs.cpp

        main.cpp
)