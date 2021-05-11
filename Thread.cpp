#include "Thread.h"
#include <utility>

Thread::Thread(Thread&& other) noexcept {
    this->handler = std::move(other.handler);
}

Thread& Thread::operator=(Thread&& other) noexcept {
    this->handler = std::move(other.handler);
    return *this;
}

void Thread::start() {
    handler = std::thread(&Thread::run, this);
}

void Thread::join() {
    handler.join();
}
