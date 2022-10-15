//
// Created by paul on 06.05.18.
//

#ifndef FLIGHTCOMPUTER_CHANNELREPLAY_HPP
#define FLIGHTCOMPUTER_CHANNELREPLAY_HPP

#include <chrono>
#include <fstream>

#include "../../Framework/InputChannel.hpp"
#include "../../Framework/Node.hpp"
#include "../../Framework/OutputChannel.hpp"
#include "Serialization.hpp"

namespace recording {
    template<typename T>
    class ChannelReplay : public Node {
      public:
        explicit ChannelReplay(std::ifstream &&istream) : stream(std::move(istream)) {
            this->start();
        };

        OutputChannel<T> &getChannelOut() {
            return channelOut;
        }

        // This method solely exists for compatibility reasons
        InputChannel<T> &getChannelIn() {
            return channelIn;
        }

      private:
        std::ifstream stream;
        OutputChannel<T> channelOut;
        InputChannel<T> channelIn;

        void run() override {
            auto replayStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                                       std::chrono::system_clock::now().time_since_epoch())
                                       .count();
            std::string line;
            std::getline(stream, line);
            auto offset = replayStart - std::stoll(line);
            std::getline(stream, line);
            std::vector<std::string> remainingItems;
            remainingItems.reserve(256);

            while (std::getline(stream, line)) {
                std::string item;
                std::stringstream linestream(line);
                std::getline(linestream, item, ';');
                auto timestamp = std::stoll(item);

                remainingItems.resize(0);
                while (std::getline(linestream, item, ';')) {
                    remainingItems.emplace_back(item);
                }

                auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                                           std::chrono::system_clock::now().time_since_epoch())
                                           .count();
                auto timeToWait = (timestamp + offset) - currentTime;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeToWait));
                channelOut.put(getItem<T>(remainingItems));
            }
        }
    };
} // namespace recording

#endif // FLIGHTCOMPUTER_CHANNELREPLAY_HPP
