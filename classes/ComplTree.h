#ifndef COMPLTREE_H
#define COMPLTREE_H

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include "arr.h"
#include "queue.h"

using namespace std;

class Tree {
public:
    explicit Tree(string val);

    void insertToTree(const string& value);
    const string& getKey() const;
    Tree* getLeft() const;
    Tree* getRight() const;
    void setLeft(Tree* leftChild);
    void setRight(Tree* rightChild);
    static bool isCompleteBinaryTree(Tree* root);
    static bool search(Tree* root, const string& value);

    friend ostream& operator<<(ostream& os, const Tree& tree);

private:
    string key;
    Tree* left;
    Tree* right;
};

#endif
