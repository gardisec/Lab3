#include "tree_funcs.h"


Tree* getTree(fstream& file) {
    size_t valueLen;
    if (!file.read(reinterpret_cast<char*>(&valueLen), sizeof(valueLen))) {
        throw runtime_error("Failed to read tree value length");
    }

    string value(valueLen, '\0');
    file.read(&value[0], valueLen);

    Tree* root = new Tree(value);

    Queue<Tree*> nodeQueue;
    nodeQueue.push(root);

    while (nodeQueue.get_size() > 0) {
        Tree* current = nodeQueue.getFirst();
        nodeQueue.pop();

        size_t leftValueLen;
        if (file.read(reinterpret_cast<char*>(&leftValueLen), sizeof(leftValueLen))) {//проходимся по левому потомку
            if (leftValueLen > 0) {
                string leftValue(leftValueLen, '\0');
                file.read(&leftValue[0], leftValueLen);
                Tree* leftChild = new Tree(leftValue);
                current->setLeft(leftChild);
                nodeQueue.push(leftChild);
            }
        }
        size_t rightValueLen;
        if (file.read(reinterpret_cast<char*>(&rightValueLen), sizeof(rightValueLen))) {//проходимся по правому потомку
            if (rightValueLen > 0) {
                string rightValue(rightValueLen, '\0');
                file.read(&rightValue[0], rightValueLen);
                Tree* rightChild = new Tree(rightValue);
                current->setRight(rightChild);
                nodeQueue.push(rightChild);
            }
        }
    }

    return root;
}

void treeToFile(Tree* tree, fstream& file) {
    if (!tree) return;

    Queue<Tree*> nodeQueue;
    nodeQueue.push(tree);

    while (nodeQueue.get_size() > 0) {
        Tree* current = nodeQueue.getFirst();
        nodeQueue.pop();

        const string& value = current->getKey();
        uint32_t valueLen = value.size();
        file.write(reinterpret_cast<const char*>(&valueLen), sizeof(valueLen));
        file.write(value.c_str(), valueLen);


        if (current->getLeft()) {//записываем левый потомок
            nodeQueue.push(current->getLeft());
        } else {
            valueLen = 0;  //если нет левого потомка, записываем длину 0
            file.write(reinterpret_cast<const char*>(&valueLen), sizeof(valueLen));
        }

        if (current->getRight()) { //записываем правый потомок
            nodeQueue.push(current->getRight());
        } else {
            valueLen = 0;  //если нет правого потомка, записываем длину 0
            file.write(reinterpret_cast<const char*>(&valueLen), sizeof(valueLen));
        }
    }
}

void treeInsert(const request& request) {
    if (request.query.get_size() != 3) {
        throw runtime_error("Wrong command syntax");
    }

    string name = request.query[1];
    string value = request.query[2];
    bool varIsExist = false;

    fstream file(request.file, ios::in | ios::binary);
    fstream tmpFile("data/tmp.data", ios::out | ios::binary | ios::trunc);

    if (!file.is_open()) {
        throw runtime_error("Failed to open input file");
    }
    if (!tmpFile.is_open()) {
        file.close();
        throw runtime_error("Failed to open temporary file");
    }

    try {
        while (true) {
            size_t nameLen;
            if (!file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen))) {
                break;
            }
            string varName(nameLen, '\0');
            file.read(&varName[0], nameLen);
            Tree* tree = getTree(file);

            if (varName == name) {
                varIsExist = true;
                tree->insertToTree(value);
            }

            size_t prefixedNameLen = varName.size();
            tmpFile.write(reinterpret_cast<const char*>(&prefixedNameLen), sizeof(prefixedNameLen));
            tmpFile.write(varName.c_str(), prefixedNameLen);
            treeToFile(tree, tmpFile);

            delete tree;
        }
        if (!varIsExist) {
            uint32_t nameLen = name.size();
            tmpFile.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
            tmpFile.write(name.c_str(), nameLen);

            Tree* newTree = new Tree(value);
            treeToFile(newTree, tmpFile);

            delete newTree;
        }
    } catch (...) {
        file.close();
        tmpFile.close();
        throw;
    }

    file.close();
    tmpFile.close();

    ifstream tmpInput("data/tmp.data", ios::binary);
    ofstream outputFile(request.file, ios::binary | ios::trunc);

    if (!tmpInput.is_open() || !outputFile.is_open()) {
        throw runtime_error("Error during final file operations");
    }

    outputFile << tmpInput.rdbuf();

    tmpInput.close();
    outputFile.close();

    ofstream clearTmp("data/tmp.data", ios::trunc);
    clearTmp.close();
}

void treeComplete(const request& request) {
    fstream file(request.file, ios::in | ios::binary);
    if (request.query.get_size() != 2) {
        throw runtime_error("Wrong command syntax");
    }

    string name = request.query[1];
    bool found = false;

    if (!file.is_open()) {
        throw runtime_error("Failed to open file");
    }

    while (true) {
        size_t nameLen;
        if (!file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen))) {
            break;
        }
        string varName(nameLen, '\0');
        file.read(&varName[0], nameLen);
        Tree* tree = getTree(file);

        if (varName == name && !found) {
            found = true;
            if (Tree::isCompleteBinaryTree(tree)) {
                cout << name << " is a complete binary tree\n";
            } else {
                cout << name << " is not a complete binary tree\n";
            }
        }
        delete tree;
    }

    if (!found) {
        cout << "Tree " << name << " not found\n";
    }

    file.close();
}

void treeSearch(const request& request) {
    fstream file(request.file, ios::in | ios::binary);
    if (request.query.get_size() != 3) {
        throw runtime_error("Wrong command syntax");
    }
    string name = request.query[1];
    string value = request.query[2];
    bool found = false;

    if (!file.is_open()) {
        throw runtime_error("Failed to open file");
    }

    while (true) {
        uint32_t nameLen;
        if (!file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen))) {
            break;
        }
        string varName(nameLen, '\0');
        file.read(&varName[0], nameLen);
        Tree* tree = getTree(file);

        if (varName == name) {
            if (Tree::search(tree, value)) {
                cout << value << " is in " << name << endl;
            } else {
                cout << value << " is not in " << name << endl;
            }
            found = true;
            delete tree;
            break;
        }
        delete tree;
    }

    if (!found) {
        throw runtime_error("Tree not found");
    }

    file.close();
}

void printInOrder(Tree* node) {
    if (node == nullptr) return;
    printInOrder(node->getLeft());
    cout << node->getKey() << " ";
    printInOrder(node->getRight());
}

void printTree(const request& request) {
    if (request.query.get_size() != 2) {
        throw runtime_error("Wrong command syntax: Expected tree name");
    }

    string treeName = request.query[1];
    fstream file(request.file, ios::in | ios::binary);
    bool treeFound = false;
    if (!file.is_open()) {
        throw runtime_error("Failed to open file");
    }

    try {
        while (true) {
            uint32_t nameLen;
            if (!file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen))) {
                break;
            }
            string varName(nameLen, '\0');
            file.read(&varName[0], nameLen);
            Tree* tree = getTree(file);

            if (varName == treeName) {
                treeFound = true;
                cout << "Tree: " << treeName << endl;

                printInOrder(tree);
                cout << endl;

                delete tree;
                break;
            }
            delete tree;
        }
        if (!treeFound) {
            cout << "Tree " << treeName << " not found" << endl;
        }
    } catch (...) {
        file.close();
        throw;
    }

    file.close();
}

