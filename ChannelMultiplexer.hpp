//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTER_CHANNELMULTIPLEXER_HPP
#define FLIGHTCOMPUTER_CHANNELMULTIPLEXER_HPP

#include <vector>
#include "Channel.hpp"

template <typename T>class ChannelMultiplexer {
    void addInput(Channel<T> &channel) {
        inChannel.push_back(channel);
    }

    void addOutput(Channel<T> &channel) {
        outChannel.push_back(channel);
    }

    void run() {
        for(auto channel: inChannel) {
            if(!channel.isClosed()) {
                T item;
                while(channel.get(item, false)) {
                    sendAll(item);
                }
            }
        }
    }
private:
    std::vector<Channel<T>> &inChannel, &outChannel;
    void sendAll(T item) {
        for(auto channel: outChannel) {
            channel.put(item);
        }
    }
};

#endif //FLIGHTCOMPUTER_CHANNELMULTIPLEXER_HPP
