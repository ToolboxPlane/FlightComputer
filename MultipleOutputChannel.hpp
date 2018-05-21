//
// Created by paul on 16.05.18.
//

#ifndef FLIGHTCOMPUTER_MIMOCHANNEL_HPP
#define FLIGHTCOMPUTER_MIMOCHANNEL_HPP

#include <vector>
#include "Channel.hpp"

template<typename T>
class MultipleOutputChannel {
public:
    void put(T item) {
        for(auto &channel: outputChannelList) {
            channel.get().put(item);
        }
    }

    void operator>>(Channel<T> & channel) {
        outputChannelList.push_back(channel);
    }
private:
    std::vector<std::reference_wrapper<Channel<T>>> outputChannelList;

};


#endif //FLIGHTCOMPUTER_MIMOCHANNEL_HPP
