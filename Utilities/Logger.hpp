//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_LOGGER_HPP
#define FLIGHTCOMPUTER_LOGGER_HPP

#include <chrono>
#include <iostream>
#include <utility>

#include "../Framework/InputChannel.hpp"
#include "../Framework/Node.hpp"

namespace debug {
    std::mutex _lock; // Not in the class because of template fuckups

    template<typename T>
    class Logger : public Node {
      public:
        explicit Logger(std::string tag, bool enabled = true, std::ostream &stream = std::cout) :
            tag(std::move(tag)),
            stream(stream),
            enabled(enabled) {
            if (enabled) {
                this->start();
            } else {
                in.close();
            }
        }

        InputChannel<T> &getChannelIn() {
            return in;
        }

      private:
        void run() override {
            T item;
            while (!in.isClosed()) {
                if (in.get(item)) {
                    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                        std::chrono::system_clock::now().time_since_epoch())
                                        .count();
                    _lock.lock();
                    stream << "[" << tag << " " << time << "]\t" << item << "\n";
                    _lock.unlock();
                }
            }
        }

        InputChannel<T> in;
        std::string tag;
        std::ostream &stream;
        bool enabled;
    };
} // namespace debug

#endif // FLIGHTCOMPUTER_LOGGER_HPP
