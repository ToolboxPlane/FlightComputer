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
        auto recordingStart = std::chrono::duration_cast
                        <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto offset = recordingStart - complete["starttime"].get<long int>();
        for(auto& data: dataList) {
            auto currentTime = std::chrono::duration_cast
                        <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto timeToWait = (data["timestamp"].get<long int>() + offset) - currentTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(timeToWait));
            channelOut.put(T(data["data"]));
        }
    }

    std::ifstream &ifstream;
    Channel<T> channelOut;
    Channel<T> channelIn;
};


#endif //FLIGHTCOMPUTER_CHANNELREPLAY_HPP
