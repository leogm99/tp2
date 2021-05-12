#include "Index.h"
#include "Crawler.h"
#include "PagesHandler.h"
#include <chrono>
#include <utility>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>


int main(int argc, const char* argv[]){
    if (argc != 7){
        return -1;
    }
    PagesHandler pages(argv[5]);

    std::queue<std::string> target_urls;
    std::ifstream f(argv[1]);
    if (!f){
        return -1;
    }

    std::string url;
    while (getline(f, url)) {
        target_urls.emplace(url);
    }

    BlockingQueue bq(target_urls);
    std::vector<std::pair<std::string, std::string>> doneUlrs;

    Index index(argv[4]);
    int workers = atoi(argv[3]);

    std::vector<Crawler> myLittleSpiders;
    std::mutex crawlerMutex;
    std::string allowed = argv[2];
    for (int i = 0; i < workers; ++i){
        myLittleSpiders.emplace_back(
                std::move(
                        Crawler(pages, index,
                                allowed, bq, doneUlrs, crawlerMutex)));
    }

    for (auto & myLittleSpider : myLittleSpiders){
        myLittleSpider.start();
    }

    std::chrono::seconds time_sec(atoi(argv[6]));
    std::this_thread::sleep_for(time_sec);
    bq.signalClosed();

    for (auto & myLittleSpider : myLittleSpiders){
        myLittleSpider.join();
    }

    sort(doneUlrs.begin(), doneUlrs.end(),
         [=](const std::pair<std::string, std::string> &l,
             const std::pair<std::string, std::string> &r) {
        return l.first < r.first;
    });

    for (const auto& a : doneUlrs){
        std::cout << a.first << " -> "
        << a.second << std::endl;
    }
    return 0;
}
