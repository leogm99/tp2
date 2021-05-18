#include "BlockingQueue.h"
#include <queue>
#include <string>
#include <utility>

BlockingQueue::BlockingQueue(std::queue<std::pair<std::string,
                             std::string>> &urls)
: closed(false), urls(urls){
}

std::string BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    while (urls.empty()){
        if (closed){
            return std::string();
        }
        cv.wait(lock1);
    }
    std::string url = std::move(urls.front()).first;
    urls.pop(); // bye bye
    return url;
}

void BlockingQueue::push(std::string&& url) {
    std::unique_lock<std::mutex> lock1(queueMutex);
    urls.push(std::make_pair(url, "ready"));
    cv.notify_all();
}

bool BlockingQueue::isClosed() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    return closed;
}

void BlockingQueue::signalClosed() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    cv.notify_all();
    closed = true;
}
