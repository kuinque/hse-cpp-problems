#pragma once

#include <cstdint>

struct Node {
    int32_t value;
    Node* next;
    Node(int32_t value, Node* next) {
        this->value = value;
        this->next = next;
    }
};

class Queue {
private:
    Node* front_;
    Node* tail_;
    int32_t size_;

public:
    Queue() {
        front_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    ~Queue() {
        while (front_ != nullptr) {
            Node* tmp = front_->next;
            delete front_;
            front_ = tmp;
        }
        size_ = 0;
    }

    void Push(int32_t value) {
        ++size_;
        Node* tmp = new Node(value, nullptr);
        if (tail_ != nullptr) {
            tail_->next = tmp;
        }
        tail_ = tmp;
        if (front_ == nullptr) {
            front_ = tail_;
        }
    }

    void Pop() {
        if (front_ == nullptr) {
            return;
        }
        --size_;
        Node* tmp = front_->next;
        delete front_;
        front_ = tmp;
    }

    int32_t Front() {
        if (front_ == nullptr) {
            return 0;
        }
        return front_->value;
    }

    int32_t Size() const {
        return size_;
    }

    void Clear() {
        while (front_ != nullptr) {
            Node* tmp = front_->next;
            delete front_;
            front_ = tmp;
        }
        size_ = 0;
    }

    bool Empty() {
        return front_ == nullptr;
    }
};
