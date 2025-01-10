#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "Node.h"
using namespace std;

template<typename T>
class Queue {
    Node<T>* head;
    Node<T>* tail;
    size_t size;
public:
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    Queue(const size_t len) : head(nullptr), tail(nullptr), size(0) {
        for (size_t i = 0; i < len; ++i) {
            this->push(T());
        }
    }

    void push(T val) { // вставляем в хвост
        Node<T>* newElement = new Node{val};
        if (this->head == nullptr) {
            this->head = newElement;
            this->tail = newElement;
            ++this->size;
        } else {
            this->tail->next = newElement;
            this->tail = newElement;
            ++this->size;
        }
    }

    void pop() { // удаляем с головы
        if (this->head == nullptr) return;
        auto tmpHead = this->head;
        this->head = this->head->next;
        delete tmpHead;
        --this->size;
    }

    T getFirst() {
        return this->head->value;
    }

    size_t get_size() const {
        return this->size;
    }

    friend ostream& operator<<(ostream& os, const Queue& que) {
        Node<T>* curr = que.head;
        while (curr != nullptr) {
            os << "[" << curr->value << "]";
            if (curr->next != nullptr) {
                os << " -> ";
            }
            curr = curr->next;
        }
        return os;
    }

    T& operator[](const size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        auto tmp = this->head;
        for (size_t i = 0; i != index; ++i) {
            tmp = tmp->next;
        }
        return tmp->value;
    }
};

#endif // QUEUE_H
