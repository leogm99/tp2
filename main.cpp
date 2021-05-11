#include "Index.h"
#include "Crawler.h"
#include "UrlState.h"
#include <chrono>
#include <utility>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>

static void loadTargets(std::queue<std::string>& targetUrls,
                        std::string&& targetPath){
    std::ifstream f(targetPath);
    if (!f){
        std::cout << "could not load targets\n";
    }
    std::string url;
    while (getline(f, url)){
        targetUrls.emplace(url);
    }
}

static std::string handleUrlState(const UrlState& state){
    switch (state){
        case UrlState::READY:
            return std::string("ready");
        case UrlState::EXPLORED:
            return std::string("explored");
        case UrlState::DEAD:
            return std::string("dead");
    }
    return std::string();
}

int main(int argc, const char* argv[]){
    std::queue<std::string> target_urls;
    loadTargets(target_urls, argv[1]);

    BlockingQueue bq(target_urls);
    std::vector<std::pair<std::string, UrlState>> doneUlrs;

    Index index(argv[4]);
    int workers = atoi(argv[3]);

    std::vector<Crawler> myLittleSpiders;
    std::mutex crawlerMutex;

    for (int i = 0; i < workers; ++i){
        myLittleSpiders.emplace_back(
                std::move(
                        Crawler(argv[5], index,
                                argv[2], bq, doneUlrs, crawlerMutex)));
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
         [=](const std::pair<std::string, UrlState> &l,
             const std::pair<std::string, UrlState> &r) {
        return l.first < r.first;
    });

    for (const auto& a : doneUlrs){
        std::cout << a.first << " -> "
        << std::move(handleUrlState(a.second)) << std::endl;
    }
    return 0;
}
