#include "Crawler.h"

Crawler::Crawler(const string& pagesFile, Index& indexMap, string allowed, queue<string>& urls)
: pages(pagesFile), indexMapping(indexMap), allowed(std::move(allowed)), urls(urls){
    if (!pages){
        cout << "could not open file\n";
        return;
    }
}

vector<string> Crawler::readChunk(uint32_t offset, uint32_t length) {
    pages.seekg(offset);
    string line;
    size_t bytes = 0;
    vector<string> vec;
    while ((bytes < length) && std::getline(pages, line)){
        std::size_t pos;
        std::size_t endurl_pos;
        if (((pos = line.find("http://")) != std::string::npos) &&
            ((endurl_pos = line.find(' ', pos))  != std::string::npos)){
            std::string url = line.substr(pos, endurl_pos - pos);
            vec.push_back(url);
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
        } else if (*it != this->allowed) {
            if ((*it).size() < this->allowed.size()) {
                // chau, si la url es mas chica que "allowed", significa que debemos removerla
                // no existe ningun caso en el que esto sea real.
                rawUrls.erase(it--);
            } else {
                // caso interesante, para que sea subdominio
                // el string debe ser identico desde el primer / hacia la izquierda luego de http://...
                // es decir -> http://sub.sub.blablaba.com/ -> mi allowed deberia ser blablaba.com
                // por construccion, todas las urls que esten en el vector, tendran el http:// (prefijo)
                // y ademas tendran / luego del .com.ar.io.algo. ... -> leo luego del prefijo,
                // hasta la primera ocurriencia de /, eso sera todo el dominio
                // el prefijo es de 7, ergo, desde el caracter 7 (indexo en 0) en adelante, leo hasta /
                size_t last_char = (*it).find_first_of('/', 7);
                // no es subdominio si esta luego del /
                if ((pos > last_char) || ((last_char - pos) != this->allowed.size())) {
                    rawUrls.erase(it--);
                }
            }
        }
    }
}

void Crawler::run() {
    vector<string> final;
    while (!this->urls.empty()){
        string& url = this->urls.front();
        final.push_back(url);
        const pair<uint32_t, uint32_t>& urlInfo = getUrlInfo(url);

        vector<string> v{readChunk(urlInfo.first, urlInfo.second)}; // move const

        filterAllowed(v);
        this->urls.pop();
        for (auto& a : v){
            this->urls.push(std::move(a));
        }
    }
    sort(final.begin(), final.end());
    for (const auto &a : final)
        cout << a << endl;
}

Crawler::~Crawler() {
    this->pages.close();
}
