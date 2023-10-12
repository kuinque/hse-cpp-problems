#include "stack.h"

Node::Node(int32_t value, Node* previous) {
    this->value = value;
    this->previous = previous;
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
    if (head_ == nullptr) {
        return;
    }
    --size_;
    Node* tmp = head_->previous;
    delete head_;
    head_ = tmp;
}

int32_t Stack::Top() const {
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
    return (head_ == nullptr);
}
