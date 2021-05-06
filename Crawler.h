#ifndef TP2_CRAWLER_H
#define TP2_CRAWLER_H
#include "Index.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>

using namespace std;

class Crawler {
private:
    ifstream pages;
    Index& indexMapping;
    const string allowed;
    queue<string>& urls;
public:
    explicit Crawler(const string& pagesFile, Index& indexMap, string allowed, queue<string>& urls);

    // don't want any ugly copies
    Crawler(const Crawler& other) = delete;
    Crawler& operator= (const Crawler& other) = delete;
    Crawler() = delete;

    void run();
    vector<string> readChunk(uint32_t offset, uint32_t length);
    void filterAllowed(vector<string>& rawUrls);
    const pair<uint32_t, uint32_t>& getUrlInfo(const string& url);
    ~Crawler();
};


#endif //TP2_CRAWLER_H
