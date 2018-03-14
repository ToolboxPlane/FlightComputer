//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTER_CHANNELMULTIPLEXER_HPP
#define FLIGHTCOMPUTER_CHANNELMULTIPLEXER_HPP

#include <vector>
#include "Channel.hpp"

template <typename T>class ChannelMultiplexer {
public:
    ChannelMultiplexer() = default;

    void addInput(Channel<T> &channel) {
        inChannel.push_back(channel);
    }

    void addOutput(Channel<T> &channel) {
        outChannel.push_back(channel);
    }

    void run() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while(true) {
            for (auto &channel: inChannel) {
                if (!channel.get().isClosed()) {
                    T item;
                    while (channel.get().get(item, false)) {
                        sendAll(item);
                    }
                }
            }
        }
#pragma clang diagnostic pop
    }
private:
    std::vector<std::reference_wrapper<Channel<T>>> inChannel, outChannel;
    void sendAll(T item) {
        for(auto &channel: outChannel) {
            channel.get().put(item);
        }
    }
};

#endif //FLIGHTCOMPUTER_CHANNELMULTIPLEXER_HPP
