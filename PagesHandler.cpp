#include "PagesHandler.h"
#include <string>
#include <vector>
#include <utility>

PagesHandler::PagesHandler(const std::string& pagesPath)
: pageHandler(pagesPath.c_str()) {
    if (!pageHandler.is_open()){
        throw std::invalid_argument("Could not open Page file\n");
    }
}

std::vector <std::string>
PagesHandler::operator()(std::pair <uint32_t, uint32_t> urlInfo) {
    std::lock_guard<std::mutex> lock(pagesMutex);
    pageHandler.seekg(urlInfo.first);
    std::string line;
    std::size_t bytes = 0;
    std::vector<std::string> vec;
    while ((bytes < urlInfo.second) && getline(pageHandler, line)){
        std::size_t pos = 0;
        std::size_t endurl_pos;
        std::string url;
        while (((pos = line.find("http://", pos)) != std::string::npos) &&
               ((endurl_pos = line.find(' ', pos)) != std::string::npos)){
            url = std::move(line.substr(pos, endurl_pos - pos));
            pos+= url.size();
            vec.push_back(std::move(url));
        }
        bytes = (uint32_t) pageHandler.tellg() - urlInfo.first;
    }
    return vec;
}

PagesHandler::PagesHandler(PagesHandler &&other)
: pageHandler(std::move(other.pageHandler)){
}

PagesHandler &PagesHandler::operator=(PagesHandler &&other) {
    if (this == &other){
        return *this;
    }
    pageHandler = std::move(other.pageHandler);
    return *this;
}
