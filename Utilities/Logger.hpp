//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_LOGGER_HPP
#define FLIGHTCOMPUTER_LOGGER_HPP

#include <iostream>
#include <utility>
#include "../ThreadModule.hpp"
#include "../Channel.hpp"

static std::mutex lock; // Not in the class because of template fuckups

template <typename T>
class Logger : ThreadModule{
public:
    explicit Logger(std::string tag, bool enabled = true, std::ostream &stream = std::cout) :
            tag(std::move(tag)), stream(stream), enabled(enabled){
        this->start();
    }

    Channel<T> &getChannelIn() {
        return in;
    }

private:
    void run() override {
        T item;
        while(enabled && !in.isClosed()) {
            if(in.get(item)) {
                lock.lock();
                stream << "[" << tag <<"]\t" << item << "\n";
                lock.unlock();
            }
        }
    }
    Channel<T> in;
    std::string tag;
    std::ostream &stream;
    bool enabled;
};

#endif //FLIGHTCOMPUTER_LOGGER_HPP
