#include "CrawlerHandler.h"
#include <chrono>
#include <string>
#include <vector>

CrawlerHandler::CrawlerHandler(size_t workers, PagesHandler &pages,
                               Index &index, BlockingQueue &q,
                               std::string allowed)
: numWorkers(workers), pages(pages), index(index), q(q),
  allowed(std::move(allowed)){
}

void CrawlerHandler::doStart(int time) {
    std::vector<Crawler> crawlers;
    crawlers.reserve(numWorkers);
    for (size_t i = 0; i < numWorkers; ++i){
        crawlers.emplace_back(pages, index,
                    allowed, q,
                    doneUrlMonitor);
    }

    for (auto& crawler : crawlers){
        crawler.start();
    }

    std::chrono::seconds workFor(time);
    std::this_thread::sleep_for(workFor);
    q.signalClosed();

    for (auto& crawler : crawlers){
        crawler.join();
    }
}

void CrawlerHandler::printDone() {
    doneUrlMonitor.print();
}

CrawlerHandler::CrawlerHandler(CrawlerHandler &&other)
: numWorkers(other.numWorkers),
  pages(other.pages), index(other.index), q(other.q),
  allowed(std::move(other.allowed)),
  doneUrlMonitor(std::move(other.doneUrlMonitor)){
    other.numWorkers = 0;
}

CrawlerHandler &CrawlerHandler::operator=(CrawlerHandler &&other) {
    if (this == &other){
        return *this;
    }
    this->numWorkers = other.numWorkers;
    other.numWorkers = 0;
    this->allowed = std::move(other.allowed);
    return *this;
}
