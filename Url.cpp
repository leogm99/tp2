#include "Url.h"
#include <string>

Url::Url(std::string rawUrl)
: rawUrl(std::move(rawUrl)){
    state = "ready";
}

Url::Url(Url &&other)
: rawUrl(std::move(other.rawUrl)),
  state(std::move(other.state)){
}

Url &Url::operator=(Url &&other) {
    if (this == &other){
        return *this;
    }

    this->rawUrl = std::move(other.rawUrl);
    this->state = std::move(other.state);
    return *this;
}

Url::~Url() {
}

void Url::setExplored() {
    state = "explored";
}

void Url::setDead() {
    state = "dead";
}

std::string Url::print() const {
    return rawUrl + " -> " + state;
}

bool Url::isNotValid() {
    return rawUrl.empty();
}

bool Url::operator<(const Url &other) const {
    return rawUrl < other.rawUrl;
}

const std::string &Url::getRawUrl() const {
    return rawUrl;
}


