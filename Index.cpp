#include "Index.h"

Index::Index(const string& index_file_path) {
    ifstream index_file(index_file_path);
    if (!index_file){
        cout << "could not open file\n";
    }

    // default, se devuelve cuando no encuentro una url
    this->indexMap["getDefault"] = make_pair(0, 0);

    string i_entry;
    while (getline(index_file, i_entry)){
        string url = i_entry.substr(0, i_entry.find_first_of(' '));
        size_t end_of_offset = 0;
        uint32_t offset = stoul(i_entry.substr(url.size(),
                                i_entry.find_first_of(' ')),
                                &end_of_offset, 16);
        uint32_t length = stoul(i_entry.substr(end_of_offset + url.size(),
                             i_entry.find_last_of(' ')),
                                nullptr, 16);
        this->indexMap[url] = std::make_pair(offset, length);
    }
    index_file.close();
}

bool Index::contains(const string& url) {
    return indexMap.find(url) != indexMap.end();
}

const pair<uint32_t, uint32_t>& Index::getIfPresent(const string& url) {
    lock_guard<mutex> lock(indexMutex);
    if(contains(url)){
        return indexMap.at(url);
    }
    return indexMap.at("getDefault");
}
