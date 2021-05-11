#ifndef TP2_THREAD_H
#define TP2_THREAD_H
#include <thread>

class Thread {
private:
    std::thread handler;
protected:
    virtual void run() = 0;
public:
    Thread() = default;

    // no copy i said
    Thread(const Thread& other) = delete;
    Thread& operator= (const Thread& other) = delete;

    Thread(Thread&& other);
    Thread& operator= (Thread&& other);

    void start();
    void join();

    virtual ~Thread() = default;
};


#endif
