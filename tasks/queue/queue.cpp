#include "queue.h"

#include <stdexcept>

Node::Node(int32_t value, Node* next) : value(value), next(next) {
}

Queue::Queue() : front_(nullptr), tail_(nullptr), size_(0) {
}

Queue::~Queue() {
    Clear();
}

void Queue::Push(int32_t value) {
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

void Queue::Pop() {
    if (front_ == nullptr) {
        throw std::logic_error("Queue is empty.");
    }
    --size_;
    Node* tmp = front_->next;
    delete front_;
    front_ = tmp;
}

int32_t Queue::Front() const {
    if (front_ == nullptr) {
        throw std::logic_error("Queue is empty.");
    }
    return front_->value;
}

int32_t Queue::Size() const {
    return size_;
}

void Queue::Clear() {
    while (front_ != nullptr) {
        Node* tmp = front_->next;
        delete front_;
        front_ = tmp;
    }
    size_ = 0;
}

bool Queue::Empty() const {
    return (size_ == 0);
}
