#ifndef TP2_CRAWLER_H
#define TP2_CRAWLER_H
#include "Thread.h"
#include "Index.h"
#include "BlockingQueue.h"
#include "PagesHandler.h"
#include <utility>
#include <map>
#include <string>
#include <vector>

class Crawler : public Thread{
private:
    PagesHandler& pages;
    Index& indexMapping;
    std::string& allowed;
    BlockingQueue& urlsQueue;
    std::map<std::string, std::string>& doneUrls;
    std::mutex& crawlerMutex;

    void filterAllowed(std::vector<std::string>& rawUrls) const;
    const std::pair<uint32_t, uint32_t>&
    getUrlInfo(const std::string& url) const;
    void store(std::pair<std::string, std::string> state);

protected:
    void run() override;

public:
    Crawler(PagesHandler& pages, Index& indexMap,
            std::string& allowed, BlockingQueue& queue,
            std::map<std::string, std::string>& doneUrls,
            std::mutex& crawlerMutex);

    // don't want any ugly copies
    Crawler(const Crawler& other) = delete;
    Crawler& operator= (const Crawler& other) = delete;

    Crawler(Crawler&& other);
    Crawler& operator= (Crawler&& other);


    ~Crawler() override;
};

#endif
