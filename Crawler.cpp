#include "Crawler.h"
#include <utility>
#include <string>
#include <vector>

Crawler::Crawler(const std::string &pagesFile, Index &indexMap,
                 std::string allowed, BlockingQueue& queue,
                 std::vector<std::pair<std::string, std::string>>& doneUrls,
                 std::mutex& crawlerMutex)
    : pages(pagesFile.c_str()), indexMapping(indexMap),
      allowed(std::move(allowed)), urlsQueue(queue), doneUrls(doneUrls),
      crawlerMutex(crawlerMutex){
    if (!pages){
        std::cout << "could not open file\n";
    }
}

std::vector<std::string> Crawler::readChunk(uint32_t offset, uint32_t length) {
    pages.seekg(offset);
    std::string line;
    std::size_t bytes = 0;
    std::vector<std::string> vec;
    while ((bytes < length) && getline(pages, line)){
        std::size_t pos = 0;
        std::size_t endurl_pos;
        while (((pos = line.find("http://", pos)) != std::string::npos) &&
               ((endurl_pos = line.find(' ', pos)) != std::string::npos)){
            std::string url = line.substr(pos, endurl_pos - pos);
            vec.push_back(url);
            pos+= url.size();
        }
        bytes = (uint32_t) pages.tellg() - offset;
    }
    return vec;
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
        auto urlInfo = getUrlInfo(url);
        if (!urlInfo.first && !urlInfo.second){
            store(std::move(
                    std::make_pair(move(url), std::string("dead"))));
            continue;
        }
        std::vector<std::string> v = std::move(readChunk(urlInfo.first,
                                                            urlInfo.second));
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
    doneUrls.push_back(url);
}

Crawler::~Crawler() {
    //this->pages.close(); // es raii, no hace falta cerrar el archivo aca
}

Crawler::Crawler(Crawler &&other)
        : indexMapping(other.indexMapping),
          urlsQueue(other.urlsQueue), doneUrls(other.doneUrls),
          crawlerMutex(other.crawlerMutex){
    this->allowed = std::move(other.allowed);
    this->pages = std::move(other.pages);
    other.pages.close();
}

Crawler& Crawler::operator=(Crawler &&other) {
    this->allowed = std::move(other.allowed);
    this->pages = std::move(other.pages);
    other.pages.close();
    return *this;
}
