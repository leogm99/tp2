//
// Created by leogm99 on 6/5/21.
//

#include "BlockingQueue.h"
#include <thread>

BlockingQueue::BlockingQueue(queue<string>& urls) : closed(false), urls(urls){
}

string BlockingQueue::pop() {
    unique_lock<mutex> lock1(queueMutex);
    while (urls.empty()){
        if (closed){
            return string{};
        }
        cv.wait(lock1);
    }
    string url = std::move(urls.front()); // esta bien, total despues lo voy a popear
    urls.pop(); // bye bye
    return url;
}

void BlockingQueue::push(string&& url) {
    unique_lock<mutex> lock1(queueMutex);
    urls.push(url);
    cv.notify_all();
}

bool BlockingQueue::isClosed() {
    unique_lock<mutex> lock1(queueMutex);
    return closed;
}

void BlockingQueue::signalClosed() {
    unique_lock<mutex> lock1(queueMutex);
    cv.notify_all();
    closed = true;
}

/*BlockingQueue &BlockingQueue::operator=(BlockingQueue &&other) noexcept : closed(false){
    this->closed = other.closed;
    return *this;
}*/

/*BlockingQueue::BlockingQueue(BlockingQueue &&other) : urls(other.urls){
    this->closed = other.closed;
    other.closed = true;
}*/
