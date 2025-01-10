#include "ComplTree.h"

Tree::Tree(string val) : key(std::move(val)), left(nullptr), right(nullptr) {}

void Tree::insertToTree(const string& value) {

    Queue<Tree*> que;
    que.push(this);

    while (que.get_size() > 0) {
        Tree* current = que.getFirst();
        que.pop();

        if (!current->getLeft()) {
            Tree* newNode = new Tree(value);
            current->setLeft(newNode);
            return;
        } else {
            que.push(current->getLeft());
        }

        if (!current->getRight()) {
            Tree* newNode = new Tree(value);
            current->setRight(newNode);
            return;
        } else {
            que.push(current->getRight());
        }
    }
}

bool Tree::isCompleteBinaryTree(Tree* root) {
    if (!root) {
        return true;
    }

    Queue<Tree*> que;
    que.push(root);
    bool mustBeLeaf = false;

    while (que.get_size() > 0) {
        Tree* current = que.getFirst();
        que.pop();

        if (current->left) {
            if (mustBeLeaf) {
                return true;
            }
            que.push(current->left);
        } else {
            mustBeLeaf = true;
        }

        if (current->right) {
            if (mustBeLeaf) {
                return true;
            }
            que.push(current->right);
        } else {
            mustBeLeaf = true;
        }
    }

    return true;
}

bool Tree::search(Tree* root, const string& value) {
    if (!root) {
        return false;
    }

    Queue<Tree*> que;
    que.push(root);

    while (que.get_size() > 0) {
        Tree* current = que.getFirst();
        que.pop();

        if (current->key == value) {
            return true;
        }

        if (current->left) {
            que.push(current->left);
        }
        if (current->right) {
            que.push(current->right);
        }
    }
    return false;
}

const string& Tree::getKey() const {
    return key;
}

Tree* Tree::getLeft() const {
    return left;
}

Tree* Tree::getRight() const {
    return right;
}

void Tree::setLeft(Tree* leftChild) {
    this->left = leftChild;
}

void Tree::setRight(Tree* rightChild) {
    this->right = rightChild;
}

ostream& operator<<(ostream& os, const Tree& tree) {
    if (!tree.left && !tree.right) {
        os << tree.key;
        return os;
    }

    Queue<const Tree*> que;
    que.push(&tree);

    while (que.get_size() > 0) {
        const Tree* current = que.getFirst();
        que.pop();

        os << current->key << " ";

        if (current->left) {
            que.push(current->left);
        }
        if (current->right) {
            que.push(current->right);
        }
    }

    return os;
}