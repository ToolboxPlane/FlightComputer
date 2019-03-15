//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_LOGGER_HPP
#define FLIGHTCOMPUTER_LOGGER_HPP

#include <iostream>
#include <chrono>
#include <utility>
#include "../Filter.hpp"
#include "../InputChannel.hpp"

static std::mutex lock; // Not in the class because of template fuckups

template <typename T>
class Logger : public Filter{
public:
    explicit Logger(std::string tag, bool enabled = true, std::ostream &stream = std::cout) :
            tag(std::move(tag)), stream(stream), enabled(enabled){
        this->start();
    }

    InputChannel<T> &getChannelIn() {
        return in;
    }

private:
    void run() override {
        T item;
        while(enabled && !in.isClosed()) {
            if(in.get(item)) {
                lock.lock();
                auto time = std::chrono::duration_cast< std::chrono::milliseconds >(
                        std::chrono::system_clock::now().time_since_epoch()
                ).count();
                stream << "[" << tag << " " << time <<"]\t" << item << "\n";
                lock.unlock();
            }
        }
    }
    InputChannel<T> in;
    std::string tag;
    std::ostream &stream;
    bool enabled;
};

#endif //FLIGHTCOMPUTER_LOGGER_HPP
