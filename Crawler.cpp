#include "Crawler.h"

Crawler::Crawler(const string &pagesFile, Index &indexMap, string allowed, BlockingQueue& queue,
                 vector<pair<string, UrlState>>& doneUrls, mutex& crawlerMutex)
    : pages(pagesFile.c_str()), indexMapping(indexMap), allowed(std::move(allowed)), urlsQueue(queue), doneUrls(doneUrls),
      crawlerMutex(crawlerMutex){
    if (!pages){
        cout << "could not open file\n";
    }
}

vector<string> Crawler::readChunk(uint32_t offset, uint32_t length) {
    pages.seekg(offset);
    string line;
    size_t bytes = 0;
    vector<string> vec;
    while ((bytes < length) && std::getline(pages, line)){
        std::size_t pos = 0;
        std::size_t endurl_pos;
        while (((pos = line.find("http://", pos)) != std::string::npos) && ((endurl_pos = line.find(' ', pos)) != std::string::npos)){
            std::string url = line.substr(pos, endurl_pos - pos);
            vec.push_back(url);
            pos+= url.size();
        }
        bytes = (uint32_t) pages.tellg() - offset;
    }
    return vec;
}

const pair<uint32_t, uint32_t> &Crawler::getUrlInfo(const string &url) {
    return this->indexMapping.getIfPresent(url);
}

void Crawler::filterAllowed(vector<string>& rawUrls) {
    for (auto it = rawUrls.begin(); it != rawUrls.end(); ++it){
        size_t pos = (*it).find(this->allowed);
        if (pos == string::npos) {
            rawUrls.erase(it--);
        } else {
            size_t last_char = (*it).find_first_of('/', 7);
            if ((pos > last_char) || ((last_char - pos) != this->allowed.size())) {
                rawUrls.erase(it--);
            }
        }
    }
}

void Crawler::run() {
    while (!urlsQueue.isClosed()) {
        string url = std::move(urlsQueue.pop());
        if (url.empty()){
            break;
        }
        auto urlInfo = getUrlInfo(url);
        if (!urlInfo.first && !urlInfo.second){
            store(move(make_pair(move(url), UrlState::DEAD)));
            continue;
        }
        vector<string> v = std::move(readChunk(urlInfo.first, urlInfo.second)); // move const
        filterAllowed(v);
        store(move(make_pair(move(url), UrlState::EXPLORED)));
        for (string filteredUrl : v) {
            urlsQueue.push(std::move(filteredUrl));
        }
    }
}

void Crawler::store(pair<string, UrlState>&& url) {
    lock_guard<mutex> lock(crawlerMutex);
    doneUrls.push_back(url);
}

Crawler::~Crawler() {
    //this->pages.close(); // es raii, no hace falta cerrar el archivo aca
}



/*Crawler::Crawler(Crawler &&other) noexcept : indexMapping(other.indexMapping) {
    this->pages = std::move(other.pages);
    this->allowed = std::move(other.allowed);
    this->urlsQueue = std::move(other.urlsQueue);
    this->doneUrls = std::move(other.doneUrls);
    this->crawlerMutex = std::move(other.crawlerMutex);
}

Crawler& Crawler::operator=(Crawler&& other) noexcept {
    this->pages = std::move(other.pages);
    this->allowed = std::move(other.allowed);
    this->urlsQueue = std::move(other.urlsQueue);
    this->doneUrls = std::move(other.doneUrls);
    this->crawlerMutex = std::move(other.crawlerMutex);
    return *this;
}
*/