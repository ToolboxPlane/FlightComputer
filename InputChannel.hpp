#ifndef _CHANNEL_HPP_
#define _CHANNEL_HPP_

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <optional>

template<typename T>
class InputChannel {
    public:
        InputChannel() : closed{false} {}

        InputChannel(const InputChannel<T> &) = delete;

        InputChannel &operator=(const InputChannel<T> &) = delete;

        void close() {
            std::unique_lock<std::mutex> lock(m);
            closed = true;
            cv.notify_all();
        }

        bool isClosed() {
            std::unique_lock<std::mutex> lock(m);
            return closed;
        }

        void put(const T i) {
            std::unique_lock<std::mutex> lock(m);
            if (closed) {
                throw std::logic_error("put to closed channel");
            }
            queue.push_back(i);
            cv.notify_one();
            //std::cerr << this->queue.size() << "\t" << typeid(T).name() << std::endl;
        }

        bool get(T &out, bool wait = true) {
            std::unique_lock<std::mutex> lock(m);
            if (wait) {
                cv.wait(lock, [&]() {
                    return closed || !queue.empty();
                });
            }
            if (queue.empty()) {
                return false;
            }
            out = queue.front();
            queue.pop_front();
            return true;
        }

        auto get(std::optional<T> &t) -> bool {
            T newT;
            if (this->get(newT, false)) {
                t = std::move(newT);
                return true;
            } else {
                return false;
            }
        }

        auto numAvailable() const -> std::size_t {
            if (closed) {
                return 0;
            } else {
                return queue.size();
            }
        }

    private:
        std::list<T> queue;
        std::mutex m;
        std::condition_variable cv;
        bool closed;
};

#endif
