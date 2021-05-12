#include "Crawler.h"
#include "PagesHandler.h"
#include <utility>
#include <string>
#include <vector>

Crawler::Crawler(PagesHandler& pages, Index &indexMap,
                 std::string& allowed, BlockingQueue& queue,
                 std::vector<std::pair<std::string, std::string>>& doneUrls,
                 std::mutex& crawlerMutex)
    : pages(pages), indexMapping(indexMap),
      allowed(allowed), urlsQueue(queue), doneUrls(doneUrls),
      crawlerMutex(crawlerMutex){

}

const std::pair<uint32_t, uint32_t>& Crawler::
getUrlInfo(const std::string &url) {
    return this->indexMapping.getIfPresent(url);
}

void Crawler::filterAllowed(std::vector<std::string>& rawUrls) {
    auto it = rawUrls.begin();
    while (it != rawUrls.end()){
        std::size_t pos = (*it).find(this->allowed);
        if (pos == std::string::npos) {
            it = rawUrls.erase(it);
        } else {
            std::size_t last_char = (*it).find_first_of('/', 7);
            if ((pos > last_char) ||
                ((last_char - pos) != this->allowed.size())) {
                it = rawUrls.erase(it);
            } else {
                ++it;
            }
        }
    }
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

void Crawler::store(std::pair<std::string, std::string>&& url) {
    std::lock_guard<std::mutex> lock(crawlerMutex);
    doneUrls.push_back(std::move(url));
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
