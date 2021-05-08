//
// Created by leogm99 on 7/5/21.
//

#include "Thread.h"

Thread::Thread(Thread&& other) noexcept {
    this->handler = std::move(other.handler);
}

Thread& Thread::operator=(Thread&& other) noexcept {
    this->handler = std::move(other.handler);
    return *this;
}

void Thread::start() {
    handler = thread(&Thread::run, this);
}

void Thread::join() {
    handler.join();
}
