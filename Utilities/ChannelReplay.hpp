//
// Created by paul on 06.05.18.
//

#ifndef FLIGHTCOMPUTER_CHANNELREPLAY_HPP
#define FLIGHTCOMPUTER_CHANNELREPLAY_HPP

#include <fstream>
#include <chrono>
#include <thread>
#include "../Filter.hpp"
#include "../Channel.hpp"
#include "../MultipleOutputChannel.hpp"

template <typename T>
class ChannelReplay : public Filter {
public:
    explicit ChannelReplay(std::istream &istream) : istream(istream){
        this->start();
    };

    MultipleOutputChannel<T> &getChannelOut() {
        return channelOut;
    }

    // This method solely exists for compatibility reasons
    Channel<T> &getChannelIn() {
        return channelIn;
    }
private:
    void run() override {
        auto recordingStart = std::chrono::duration_cast
                        <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::string line;
        std::getline(istream, line);
        auto offset = recordingStart - std::stol(line);
        std::getline(istream, line);
        std::vector<std::string> remainingItems;
        remainingItems.reserve(256);

        while(std::getline(istream, line)) {
            std::string item;
            std::cerr << item << std::endl;
            std::stringstream linestream(line);
            std::getline(linestream, item, ';');
            auto timestamp = std::stol(item);

            remainingItems.resize(0);
            while(std::getline(linestream, item, ';')) {
                remainingItems.push_back(item);
            }

            auto currentTime = std::chrono::duration_cast
                    <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto timeToWait = (timestamp + offset) - currentTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(timeToWait));
            channelOut.put(T(remainingItems));
        }


    }

    std::istream &istream;
    MultipleOutputChannel<T> channelOut;
    Channel<T> channelIn;
};


#endif //FLIGHTCOMPUTER_CHANNELREPLAY_HPP
