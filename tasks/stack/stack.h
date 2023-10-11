#pragma once

#include <cstdint>

struct Node {
    int32_t value;
    Node* previous;
    Node(int32_t value, Node* previous) {
        this->value = value;
        this->previous = previous;
    }
};

class Stack {
private:
    Node* head_;
    int size_;

public:
    Stack() {
        head_ = nullptr;
        size_ = 0;
    }

    ~Stack() {
        delete head_;
    }

    void Push(int32_t value) {
        ++size_;
        Node* tmp = new Node(value, head_);
        head_ = tmp;
    }

    void Pop() {
        if (head_ == nullptr) {
            return;
        }
        --size_;
        Node* tmp = head_->previous;
        delete head_;
        head_ = tmp;
    }

    int32_t Top() {
        return head_->value;
    }

    int32_t Size() const {
        return static_cast<int32_t> (size_);
    }

    void Clear() {
        while (head_ != nullptr) {
            Node* tmp = head_->previous;
            delete head_;
            head_ = tmp;
        }
    }

    bool Empty() {
        return (head_ == nullptr);
    }
};
