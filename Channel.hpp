#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

template<class item>
class Channel {
private:
    std::list<item> queue;
    std::mutex m;
    std::condition_variable cv;
    bool closed;
public:
    Channel() : closed(false), m(), cv(), queue() { }

    void close() {
        std::unique_lock<std::mutex> lock(m);
        closed = true;
        cv.notify_all();
    }
    bool isClosed() {
        std::unique_lock<std::mutex> lock(m);
        return closed;
    }
    void put(const item &i) {
        std::unique_lock<std::mutex> lock(m);
        if(closed)
            throw std::logic_error("put to closed channel");
        queue.push_back(i);
        cv.notify_one();
    }
    bool get(item &out, bool wait = true) {
        std::unique_lock<std::mutex> lock(m);
        if(wait)
            cv.wait(lock, [&](){ return closed || !queue.empty(); });
        if(queue.empty())
            return false;
        out = queue.front();
        queue.pop_front();
        return true;
    }
};
