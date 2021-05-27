#ifndef TP2_PAGESHANDLER_H
#define TP2_PAGESHANDLER_H
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>
#include <utility>

class PagesHandler {
private:
    std::mutex pagesMutex;
    std::ifstream pageHandler;
public:
    explicit PagesHandler(const std::string& pagesPath);
    std::vector<std::string> operator()(std::pair<uint32_t, uint32_t> urlInfo);

    PagesHandler(const PagesHandler& other) = delete;
    PagesHandler& operator=(const PagesHandler& other) = delete;

    PagesHandler(PagesHandler&& other);
    PagesHandler& operator=(PagesHandler&& other);
};


#endif
