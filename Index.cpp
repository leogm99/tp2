#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <mutex>
#include <utility>
#include "Index.h"

// Si bien esto se puede hacer con istringstream
// encontre que la version mas verbosa es considerablemente mas veloz
Index::Index(const std::string& index_file_path) {
    std::ifstream index_file(index_file_path);
    if (!index_file.is_open()){
        throw std::invalid_argument("could not open file\n");
    }
    // default, se devuelve cuando no encuentro una url
    this->indexMap["getDefault"] = std::make_pair(0, 0);

    std::string i_entry;
    while (getline(index_file, i_entry)){
        std::string url = i_entry.substr(0,
                                         i_entry.find_first_of(' '));
        std::size_t end_of_offset = 0;
        uint32_t offset = std::stoul(i_entry.substr(url.size(),
                                i_entry.find_first_of(' ')),
                                &end_of_offset, 16);
        uint32_t length = std::stoul(i_entry.substr(end_of_offset + url.size(),
                             i_entry.find_last_of(' ')),
                                nullptr, 16);
        this->indexMap[std::move(url)] = std::make_pair(offset, length);
    }
}

bool Index::contains(const std::string& url) const {
    return indexMap.find(url) != indexMap.end();
}

const std::pair<uint32_t, uint32_t>& Index::
getIfPresent(const Url& url) const {
    const std::string rawUrl = url.getRawUrl();
    if (contains(rawUrl)){
        return indexMap.at(rawUrl);
    }
    return indexMap.at("getDefault");
}
