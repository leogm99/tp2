#ifndef TP2_CRAWLER_H
#define TP2_CRAWLER_H
#include "Thread.h"
#include "Index.h"
#include "BlockingQueue.h"
#include "UrlState.h"
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

class Crawler : public Thread{
private:
    ifstream pages;
    Index& indexMapping;
    string allowed;
    BlockingQueue& urlsQueue;
    vector<pair<string, UrlState>>& doneUrls;
    mutex& crawlerMutex;

protected:
    void run() override;

public:
    //explicit Crawler(const string& pagesFile, Index& indexMap, const string& allowed, queue<string>& urls);
    explicit Crawler(const string& pagesFile, Index& indexMap,
                     string allowed, BlockingQueue& queue, vector<pair<string, UrlState>>& doneUrls, mutex& crawlerMutex);

    // don't want any ugly copies
    Crawler(const Crawler& other) = delete;
    Crawler& operator= (const Crawler& other) = delete;
    Crawler() = delete;

    //Crawler(Crawler&& other) noexcept;
    //Crawler& operator=(Crawler&& other) noexcept;

    vector<string> readChunk(uint32_t offset, uint32_t length);
    void filterAllowed(vector<string>& rawUrls);
    const pair<uint32_t, uint32_t>& getUrlInfo(const string& url);

    void store(pair<string, UrlState>&& url);
    ~Crawler() override;

};


#endif //TP2_CRAWLER_H
