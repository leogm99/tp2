#include "Crawler.h"
#include "PagesHandler.h"
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

Crawler::Crawler(PagesHandler& pages, Index &indexMap,
                 std::string& allowed, BlockingQueue& queue,
                 std::map<std::string, std::string>& doneUrls,
                 std::mutex& crawlerMutex)
    : pages(pages), indexMapping(indexMap),
      allowed(allowed), urlsQueue(queue), doneUrls(doneUrls),
      crawlerMutex(crawlerMutex){
}

const std::pair<uint32_t, uint32_t>& Crawler::
getUrlInfo(const std::string &url) const {
    return this->indexMapping.getIfPresent(url);
}

void Crawler::filterAllowed(std::vector<std::string>& rawUrls) const {
    auto end = std::remove_if(rawUrls.begin(), rawUrls.end(),
    [&](const std::string& url){
        std::size_t pos = url.find(allowed);
        if (pos == std::string::npos){
            return true;
        }
        std::size_t lastForwardSlash = url.find_first_of('/', 7); // http://
        if (pos > lastForwardSlash ||
            (lastForwardSlash - pos) != this->allowed.size()){
            return true;
        }
        return false;
    });
    rawUrls.erase(end, rawUrls.end());
}

void Crawler::run() {
    while (!urlsQueue.isClosed()) {
        std::string url = std::move(urlsQueue.pop());
        if (url.empty()){
            break;
        }
        const auto& urlInfo = getUrlInfo(url);
        if (!urlInfo.first && !urlInfo.second){
            store(std::move(
                    std::make_pair(std::move(url),std::string("dead"))));
            continue;
        }

        std::vector<std::string> v = std::move(pages(urlInfo));
        filterAllowed(v);
        store(std::move(
                std::make_pair(std::move(url), std::string("explored"))));
        for (std::string& filteredUrl : v) {
            urlsQueue.push(std::move(filteredUrl));
        }
    }
}

void Crawler::store(std::pair<std::string, std::string> url) {
    std::lock_guard<std::mutex> lock(crawlerMutex);
    doneUrls.insert(std::move(url));
}

Crawler::~Crawler() {
}

Crawler::Crawler(Crawler &&other)
        : pages(other.pages),
          indexMapping(other.indexMapping),
          allowed(other.allowed),
          urlsQueue(other.urlsQueue), doneUrls(other.doneUrls),
          crawlerMutex(other.crawlerMutex){
}

Crawler& Crawler::operator=(Crawler &&other) {
    return *this;
}

