#include "Crawler.h"
#include "PagesHandler.h"
#include "DoneUrlMonitor.h"
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

Crawler::Crawler(PagesHandler& pages, Index &indexMap,
                 std::string& allowed, BlockingQueue& queue,
                 DoneUrlMonitor& doneUrls)
    : pages(pages), indexMapping(indexMap),
      allowed(allowed), urlsQueue(queue),
      doneUrlMonitor(doneUrls){
}

const std::pair<uint32_t, uint32_t>& Crawler::
getUrlInfo(const Url& url) const {
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
    while (urlsQueue.isNotClosedOrNotEmpty()) {
        Url url = std::move(urlsQueue.pop());
        if (url.isNotValid()){
            break;
        }
        const auto& urlInfo = getUrlInfo(url);
        if (!urlInfo.first && !urlInfo.second){
            url.setDead();
            store(std::move(url));
            continue;
        }

        std::vector<std::string> v = std::move(pages(urlInfo));
        filterAllowed(v);
        url.setExplored();
        store(std::move(url));
        for (std::string& filteredUrl : v) {
            Url newUrl(filteredUrl);
            urlsQueue.push(std::move(newUrl));
        }
    }
}

void Crawler::store(Url url) {
    doneUrlMonitor.store(std::move(url));
}

Crawler::~Crawler() {
}

Crawler::Crawler(Crawler &&other)
        : pages(other.pages),
          indexMapping(other.indexMapping),
          allowed(other.allowed),
          urlsQueue(other.urlsQueue),
          doneUrlMonitor(other.doneUrlMonitor){
}

Crawler& Crawler::operator=(Crawler &&other) {
    return *this;
}

