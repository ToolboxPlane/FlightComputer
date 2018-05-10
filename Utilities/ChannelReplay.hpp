//
// Created by paul on 06.05.18.
//

#ifndef FLIGHTCOMPUTER_CHANNELREPLAY_HPP
#define FLIGHTCOMPUTER_CHANNELREPLAY_HPP

#include <fstream>
#include <chrono>
#include <thread>
#include "../ThreadModule.hpp"
#include "../Channel.hpp"
#include "json.hpp"

using json = nlohmann::json;

template <typename T>
class ChannelReplay : public ThreadModule {
public:
    explicit ChannelReplay(std::ifstream &ifstream) : ThreadModule(), ifstream(ifstream){};

    Channel<T> &getChannelOut() {
        return channelOut;
    }

    // This method solely exists for compatibility reasons
    Channel<T> &getChannelIn() {
        return channelIn;
    }
private:
    void run() override {
        json complete;
        ifstream >> complete;
        json dataList = complete["recording"];
        auto offset = std::time(nullptr) - complete["starttime"].get<int>();
        for(auto& data: dataList) {
            auto timeToWait = (data["timestamp"].get<int>() + offset) - std::time(nullptr);
            std::this_thread::sleep_for(std::chrono::seconds(timeToWait));
            channelOut.put(T(data["data"]));
        }
    }

    std::ifstream &ifstream;
    Channel<T> channelOut;
    Channel<T> channelIn;
};


#endif //FLIGHTCOMPUTER_CHANNELREPLAY_HPP
