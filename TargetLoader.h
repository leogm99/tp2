//
// Created by leogm99 on 27/5/21.
//

#ifndef TP2_TARGETLOADER_H
#define TP2_TARGETLOADER_H
#include "Url.h"
#include <fstream>
#include <queue>


class TargetLoader {
private:
    std::ifstream target;
public:
    explicit TargetLoader(const char* targetPath);

    TargetLoader(const TargetLoader& other) = delete;
    TargetLoader& operator=(const TargetLoader& other) = delete;

    TargetLoader(TargetLoader&& other);
    TargetLoader& operator=(TargetLoader&& other);

    void loadIntoQueue(std::queue<Url>& urls);

    ~TargetLoader();
};


#endif //TP2_TARGETLOADER_H
