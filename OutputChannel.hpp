//
// Created by paul on 16.05.18.
//

#ifndef FLIGHTCOMPUTER_MIMOCHANNEL_HPP
#define FLIGHTCOMPUTER_MIMOCHANNEL_HPP

#include <vector>
#include "InputChannel.hpp"

template<typename T>
class OutputChannel {
public:
    OutputChannel() = default;
    OutputChannel(const OutputChannel<T> &) = delete;
    OutputChannel& operator=(const OutputChannel<T> &) = delete;

    void put(T item) {
        for(auto &channel: outputChannelList) {
            if (!channel.get().isClosed()) {
                channel.get().put(item);
            }
        }
    }

    void operator>>(InputChannel<T> &channel) {
        outputChannelList.emplace_back(channel);
    }
private:
    std::vector<std::reference_wrapper<InputChannel<T>>> outputChannelList;

};


#endif //FLIGHTCOMPUTER_MIMOCHANNEL_HPP
