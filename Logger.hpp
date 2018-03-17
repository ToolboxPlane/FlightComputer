//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_LOGGER_HPP
#define FLIGHTCOMPUTER_LOGGER_HPP

#include <iostream>
#include "ThreadModule.hpp"
#include "Channel.hpp"

static std::mutex lock; // Not in the class because of template fuckups

template <typename T>
class Logger : ThreadModule{
public:
    explicit Logger(std::string tag, bool enabled = true) : ThreadModule(), tag(std::move(tag)),
        enabled(enabled){}
    Channel<T> &getChannelIn() {
        return in;
    }

private:
    void run() override {
        T item;
        while(enabled && !in.isClosed()) {
            if(in.get(item)) {
                lock.lock();
                std::cout << "[" << tag << "]\t" << item << std::endl;
                lock.unlock();
            }
        }
    }
    Channel<T> in;
    std::string tag;
    bool enabled;
};

#endif //FLIGHTCOMPUTER_LOGGER_HPP
