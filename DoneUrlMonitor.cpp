#include "DoneUrlMonitor.h"
#include "Url.h"
#include <iostream>
#include <mutex>
#include <set>
#include <utility>


DoneUrlMonitor::DoneUrlMonitor() {
}

void DoneUrlMonitor::store(Url url) {
    std::lock_guard<std::mutex> lockGuard(urlMutex);
    doneUrls.insert(std::move(url));
}

void DoneUrlMonitor::print() {
    for (const auto& url : doneUrls){
        std::cout << std::move(url.print()) << std::endl;
    }
}

DoneUrlMonitor::~DoneUrlMonitor() {
}

DoneUrlMonitor::DoneUrlMonitor(DoneUrlMonitor &&other)
: doneUrls(std::move(other.doneUrls)){
}

DoneUrlMonitor &DoneUrlMonitor::operator=(DoneUrlMonitor &&other) {
    if (this == &other){
        return *this;
    }
    doneUrls = std::move(other.doneUrls);
    return *this;
}
