//
// Created by leogm99 on 27/5/21.
//

#ifndef TP2_URL_H
#define TP2_URL_H
#include <string>

class Url {
private:
    std::string rawUrl;
    std::string state;
public:
    explicit Url(std::string rawUrl);

    Url(const Url& other) = delete;
    Url& operator=(const Url& other) = delete;

    const std::string& getRawUrl() const;

    Url(Url&& other);
    Url& operator=(Url&& other);

    bool isNotValid();
    void setExplored();
    void setDead();

    bool operator<(const Url& other) const;

    std::string print() const;

    ~Url();
};


#endif //TP2_URL_H
