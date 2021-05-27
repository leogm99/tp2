#ifndef TP2_DONEURLMONITOR_H
#define TP2_DONEURLMONITOR_H

#include "Url.h"
#include <iostream>
#include <mutex>
#include <set>

class DoneUrlMonitor {
private:
    std::set<Url> doneUrls;
    std::mutex urlMutex;
public:
    DoneUrlMonitor();

    DoneUrlMonitor(const DoneUrlMonitor& other) = delete;
    DoneUrlMonitor& operator=(const DoneUrlMonitor& other) = delete;

    DoneUrlMonitor(DoneUrlMonitor&& other);
    DoneUrlMonitor& operator=(DoneUrlMonitor&& other);

    void store(Url url);
    void print();

    ~DoneUrlMonitor();
};


#endif
