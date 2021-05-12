#ifndef TP2_CRAWLER_H
#define TP2_CRAWLER_H
#include "Thread.h"
#include "Index.h"
#include "BlockingQueue.h"
#include "PagesHandler.h"

class Crawler : public Thread{
private:
    PagesHandler& pages;
    Index& indexMapping;
    std::string& allowed;
    BlockingQueue& urlsQueue;
    std::vector<std::pair<std::string, std::string>>& doneUrls;
    std::mutex& crawlerMutex;

protected:
    void run() override;

public:
    Crawler(PagesHandler& pages, Index& indexMap,
            std::string& allowed, BlockingQueue& queue,
            std::vector<std::pair<std::string,
            std::string>>& doneUrls, std::mutex& crawlerMutex);

    // don't want any ugly copies
    Crawler(const Crawler& other) = delete;
    Crawler& operator= (const Crawler& other) = delete;

    Crawler(Crawler&& other);
    Crawler& operator= (Crawler&& other);

    void filterAllowed(std::vector<std::string>& rawUrls);
    const std::pair<uint32_t, uint32_t>& getUrlInfo(const std::string& url);

    void store(std::pair<std::string, std::string>&& url);
    ~Crawler() override;
};

#endif
