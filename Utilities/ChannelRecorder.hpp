//
// Created by paul on 06.05.18.
//

#ifndef FLIGHTCOMPUTER_RECORDER_HPP
#define FLIGHTCOMPUTER_RECORDER_HPP

#include <iostream>
#include "../ThreadModule.hpp"
#include "../Channel.hpp"
#include "json.hpp"

using json = nlohmann::json;

template <typename T>
class ChannelRecorder : public ThreadModule {
public:
    explicit ChannelRecorder(std::ostream &ostream) : ostream(ostream){
        this->start();
    };
    Channel<T> &getChannelIn() {
        return channelIn;
    }


private:
    Channel<T> channelIn;
    std::ostream &ostream;

    void run() override {
        T item;
        ostream << std::chrono::duration_cast
                <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << "\n";
        ostream << T::header() << "\n";

        while(!channelIn.isClosed()) {
            if(channelIn.get(item)) {
                ostream << std::chrono::duration_cast
                        <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                ostream << "; " << item.getLine() << std::endl;
            }
        }
    }

};


#endif //FLIGHTCOMPUTER_RECORDER_HPP
