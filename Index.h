#ifndef TP2_INDEX_H
#define TP2_INDEX_H
#include "Url.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <mutex>
#include <utility>
#include <mutex>

class Index {
private:
    std::mutex indexMutex;
    std::map<std::string, std::pair<uint32_t, uint32_t>> indexMap;
    bool contains(const std::string& key) const;
public:
    explicit Index(const std::string& index_file_path);

    // No copy allowed
    Index(const Index& other) = delete;
    Index& operator= (const Index& other) = delete;
    // Also you get no default
    Index() = delete;

    Index(Index&& other);
    Index& operator= (Index&& other);

    const std::pair<uint32_t, uint32_t>&
    getIfPresent(const Url& url);
};

#endif
