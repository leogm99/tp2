#ifndef TP2_CRAWLER_H
#define TP2_CRAWLER_H
#include "Thread.h"
#include "Index.h"
#include "BlockingQueue.h"
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

class Crawler : public Thread{
private:
    std::ifstream pages;
    Index& indexMapping;
    std::string allowed;
    BlockingQueue& urlsQueue;
    std::vector<std::pair<std::string, std::string>>& doneUrls;
    std::mutex& crawlerMutex;

protected:
    void run() override;

public:
    Crawler(const std::string& pagesFile, Index& indexMap,
            std::string allowed, BlockingQueue& queue,
            std::vector<std::pair<std::string,
            std::string>>& doneUrls, std::mutex& crawlerMutex);

    // don't want any ugly copies
    Crawler(const Crawler& other) = delete;
    Crawler& operator= (const Crawler& other) = delete;

    Crawler(Crawler&& other);
    Crawler& operator= (Crawler&& other);

    std::vector<std::string> readChunk(uint32_t offset, uint32_t length);
    void filterAllowed(std::vector<std::string>& rawUrls);
    const std::pair<uint32_t, uint32_t>& getUrlInfo(const std::string& url);

    void store(std::pair<std::string, std::string>&& url);
    ~Crawler() override;
};

#endif
