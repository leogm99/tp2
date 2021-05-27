#include "TargetLoader.h"
#include "Url.h"
#include <fstream>
#include <queue>

TargetLoader::TargetLoader(const char *targetPath) {
    target.open(targetPath);
    if (!target.is_open()){
        throw std::invalid_argument("Target file could not open");
    }
}

void TargetLoader::loadIntoQueue(std::queue<Url>& urls) {
    std::string url;
    while (getline(target, url)) {
        urls.emplace(url);
    }
}

TargetLoader::~TargetLoader() {

}

TargetLoader::TargetLoader(TargetLoader &&other)
:target(std::move(other.target)){
}

TargetLoader &TargetLoader::operator=(TargetLoader &&other) {
    if (this == &other){
        return *this;
    }
    target = std::move(other.target);
    return *this;
}
