#include "stack.h"

#include <stdexcept>

Node::Node(int32_t value, Node* previous) : value(value), previous(previous) {
}

Stack::Stack() {
    head_ = nullptr;
    size_ = 0;
}

Stack::~Stack() {
    while (head_ != nullptr) {
        Node* tmp = head_->previous;
        delete head_;
        head_ = tmp;
    }
    size_ = 0;
}

void Stack::Push(int32_t value) {
    ++size_;
    Node* tmp = new Node(value, head_);
    head_ = tmp;
}

void Stack::Pop() {
    if (size_ == 0) {
        return;
    }
    --size_;
    Node* tmp = head_->previous;
    delete head_;
    head_ = tmp;
}

int32_t Stack::Top() const {
    if (size_ == 0) {
        throw std::out_of_range(0);
    }
    return head_->value;
}

int32_t Stack::Size() const {
    return static_cast<int32_t>(size_);
}

void Stack::Clear() {
    while (head_ != nullptr) {
        Node* tmp = head_->previous;
        delete head_;
        head_ = tmp;
    }
    size_ = 0;
}

bool Stack::Empty() const {
    return (size_ == 0);
}
