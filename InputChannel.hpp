#ifndef _CHANNEL_HPP_
#define _CHANNEL_HPP_

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<class item>
class InputChannel {
public:
    InputChannel() : closed(false), m(), cv(), queue() { }

    void close() {
        std::unique_lock<std::mutex> lock(m);
        closed = true;
        cv.notify_all();
    }
    bool isClosed() {
        std::unique_lock<std::mutex> lock(m);
        return closed;
    }
    void put(const item i) {
        std::unique_lock<std::mutex> lock(m);
        if(closed)
            throw std::logic_error("put to closed channel");
        queue.push_back(i);
        cv.notify_one();
        //std::cerr << this->queue.size() << "\t" << typeid(item).name() << std::endl;
    }
    bool get(item &out, bool wait = true) {
        std::unique_lock<std::mutex> lock(m);
        if(wait) {
            cv.wait(lock, [&]() {
                return closed || !queue.empty();
            });
        }
        if(queue.empty()) {
            return false;
        }
        out = queue.front();
        queue.pop_front();
        return true;
    }
private:
    std::list<item> queue{};
    std::mutex m{};
    std::condition_variable cv{};
    bool closed{false};
};

#endif
