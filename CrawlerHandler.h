#ifndef TP2_CRAWLERHANDLER_H
#define TP2_CRAWLERHANDLER_H
#include "Crawler.h"
#include "Index.h"
#include "PagesHandler.h"
#include "BlockingQueue.h"
#include <utility>
#include <map>
#include <string>
#include <vector>

class CrawlerHandler {
private:
    size_t numWorkers;
    PagesHandler& pages;
    Index& index;
    BlockingQueue& q;
    std::string allowed;
    std::mutex crawlerMutex;
    std::map<std::string, std::string> doneUrls;
public:
    CrawlerHandler(size_t workers, PagesHandler& pages,
                   Index& index, BlockingQueue& q,
                   std::string allowed);

    CrawlerHandler(const CrawlerHandler& other) = delete;
    CrawlerHandler& operator=(const CrawlerHandler& other) = delete;

    CrawlerHandler(CrawlerHandler&& other);
    CrawlerHandler& operator=(CrawlerHandler&& other);

    void doStart(int time);

    void printDone();
};


#endif //TP2_CRAWLERHANDLER_H
