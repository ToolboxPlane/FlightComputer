//
// Created by paul on 06.05.18.
//

#ifndef FLIGHTCOMPUTER_RECORDER_HPP
#define FLIGHTCOMPUTER_RECORDER_HPP

#include <fstream>
#include "../../Node.hpp"
#include "../../InputChannel.hpp"

namespace recording {
    template<typename T>
    class ChannelRecorder : public Node {
        public:
            explicit ChannelRecorder(std::ofstream &&ostream) : stream(std::move(ostream)) {
                this->start();
            };

            InputChannel<T> &getChannelIn() {
                return channelIn;
            }

        private:
            InputChannel<T> channelIn;
            std::ofstream stream;

            void run() override {
                T item;
                auto lastUpdate = std::chrono::duration_cast
                        <std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                stream << lastUpdate << "\n";
                stream << header<T>() << std::endl;

                while (!channelIn.isClosed()) {
                    if (channelIn.get(item)) {
                        auto millis = std::chrono::duration_cast
                                <std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()).count();

                        stream << millis << "; " << getLine(item) << "\n";

                        if (millis - lastUpdate > 1000) {
                            stream << std::flush;
                        }
                    }
                }
            }
    };
}

#endif //FLIGHTCOMPUTER_RECORDER_HPP
