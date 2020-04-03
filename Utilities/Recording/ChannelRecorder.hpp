//
// Created by paul on 06.05.18.
//

#ifndef FLIGHTCOMPUTER_RECORDER_HPP
#define FLIGHTCOMPUTER_RECORDER_HPP

#include <iostream>
#include "../../Node.hpp"
#include "../../InputChannel.hpp"

namespace recording {
    template<typename T>
    class ChannelRecorder : public Node {
        public:
            explicit ChannelRecorder(std::ostream &ostream) : ostream(ostream), count{0} {
                this->start();
            };

            InputChannel<T> &getChannelIn() {
                return channelIn;
            }

        private:
            InputChannel<T> channelIn;
            std::ostream &ostream;
            std::size_t count;

            void run() override {
                T item;
                ostream << std::chrono::duration_cast
                        <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
                        << "\n";
                ostream << header<T>() << std::endl;

                while (!channelIn.isClosed()) {
                    if (channelIn.get(item)) {
                        ostream << std::chrono::duration_cast
                                <std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()).count();
                        ostream << "; " << getLine(item) << "\n";
                        count += 1;
                        if (count % 20 == 0) {
                            ostream << std::flush;
                        }
                    }
                }
            }
    };
}

#endif //FLIGHTCOMPUTER_RECORDER_HPP
