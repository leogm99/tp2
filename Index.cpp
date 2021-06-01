#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <mutex>
#include <utility>
#include <sstream>
#include "Index.h"

// Si bien esto se puede hacer con istringstream
// encontre que la version mas verbosa es considerablemente mas veloz
Index::Index(const std::string& index_file_path) {
    std::ifstream index_file(index_file_path);
    if (!index_file.is_open()){
        throw std::invalid_argument("Could not open Index file\n");
    }
    // default, se devuelve cuando no encuentro una url
    this->indexMap["getDefault"] = std::make_pair(0, 0);
    std::string i_entry;
    while (getline(index_file, i_entry)){
        std::stringstream s(i_entry);
        std::string url, offset, length;
        s >> url >> offset >> length;
        this->indexMap[std::move(url)] = std::move(
                           std::make_pair(
                        std::stoi(offset, 0, 16),
                        std::stoi(length, 0, 16)));
    }
}

bool Index::contains(const std::string& url) const {
    return indexMap.find(url) != indexMap.end();
}

const std::pair<uint32_t, uint32_t>& Index::
getIfPresent(const Url& url) {
    std::lock_guard<std::mutex> lock(indexMutex);
    const std::string rawUrl = url.getRawUrl();
    if (contains(rawUrl)){
        return indexMap.at(rawUrl);
    }
    return indexMap.at("getDefault");
}

Index::Index(Index &&other)
: indexMap(std::move(other.indexMap)){
}

Index &Index::operator=(Index &&other) {
    if (this == &other){
        return *this;
    }
    indexMap = std::move(other.indexMap);
    return *this;
}
