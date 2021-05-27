#include "BlockingQueue.h"
#include "Url.h"
#include <queue>
#include <string>
#include <utility>

BlockingQueue::BlockingQueue(TargetLoader& targetLoader)
: closed(false){
    targetLoader.loadIntoQueue(urls);
}

Url BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    while (urls.empty()){
        if (closed){
            return Url("");
        }
        cv.wait(lock1);
    }
    Url url = std::move(urls.front());
    urls.pop(); // bye bye
    return url;
}

void BlockingQueue::push(Url url) {
    std::unique_lock<std::mutex> lock1(queueMutex);
    urls.push(std::move(url));
    cv.notify_all();
}

// continue while
// q is not closed or its not empty
bool BlockingQueue::isNotClosedOrNotEmpty() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    return (!closed || !urls.empty());
}

void BlockingQueue::signalClosed() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    closed = true;
    cv.notify_all();
}

BlockingQueue::BlockingQueue(BlockingQueue &&other)
: closed(other.closed), urls(std::move(other.urls)){
}

BlockingQueue &BlockingQueue::operator=(BlockingQueue &&other) {
    if (this == &other) {
        return *this;
    }
    closed = other.closed;
    urls = std::move(other.urls);
    return *this;
}
