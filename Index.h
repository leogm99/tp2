#ifndef TP2_INDEX_H
#define TP2_INDEX_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <mutex>

using namespace std;

class Index {
private:
    map<string, pair<uint32_t, uint32_t>> indexMap;
    mutex indexMutex;
    bool contains(const string& key);
public:
    explicit Index(const string& index_file_path);

    // No copy allowed
    Index(const Index& other) = delete;
    Index& operator= (const Index& other) = delete;
    // Also you get no default
    Index() = delete;

    pair<uint32_t, uint32_t> getIfPresent(const string& url);
    ~Index();
};


#endif //TP2_INDEX_H
