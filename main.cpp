#include "Index.h"
#include "CrawlerHandler.h"
#include "Crawler.h"
#include "PagesHandler.h"
#include <utility>
#include <queue>
#include <string>

const int NUM_ARGS = 7;

int main(int argc, const char* argv[]){
    if (argc != NUM_ARGS){
        return -1;
    }
    PagesHandler pages(argv[5]);

    std::queue<std::pair<std::string, std::string>> target_urls;
    std::ifstream f(argv[1]);
    if (!f){
        return -1;
    }

    std::string url;
    while (getline(f, url)) {
        target_urls.emplace(
                std::make_pair(std::move(url), "ready"));
    }

    BlockingQueue bq(target_urls);
    Index index(argv[4]);
    std::string allowed = argv[2];
    CrawlerHandler ch(atoi(argv[3]), pages, index, bq, std::move(allowed));
    ch.doStart(atoi(argv[6]));
    ch.printDone();
    return 0;
}
