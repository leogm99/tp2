#include "Thread.h"
#include <utility>

Thread::Thread(Thread&& other) {
    this->handler = std::move(other.handler);
}

Thread& Thread::operator=(Thread&& other) {
    if (this == &other){
        return *this;
    }
    this->handler = std::move(other.handler);
    return *this;
}

void Thread::start() {
    handler = std::thread(&Thread::run, this);
}

void Thread::join() {
    handler.join();
}
