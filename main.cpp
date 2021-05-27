#include "Index.h"
#include "CrawlerHandler.h"
#include "Crawler.h"
#include "PagesHandler.h"
#include "TargetLoader.h"
#include <utility>
#include <queue>
#include <string>

const int NUM_ARGS = 7;

int main(int argc, const char* argv[]){
    if (argc != NUM_ARGS){
        return -1;
    }
    std::string allowed = argv[2];

    try {
        TargetLoader targetLoader(argv[1]);
        PagesHandler pages(argv[5]);
        Index index(argv[4]);

        BlockingQueue bq(targetLoader);
        CrawlerHandler ch(atoi(argv[3]), pages, index, bq, std::move(allowed));
        ch.doStart(atoi(argv[6]));
        ch.printDone();
    } catch(const std::invalid_argument& e){
        std::cout << e.what();
        return -1;
    }
    return 0;
}
