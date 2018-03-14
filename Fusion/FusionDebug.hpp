//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_FUSIONDEBUG_HPP
#define FLIGHTCOMPUTER_FUSIONDEBUG_HPP

#include <string>
#include "Fusion.hpp"

class FusionDebug {
public:
    explicit FusionDebug(const std::string &tag): tag(tag) {}
    void run();

    Channel<state_t> &getChannelIn();
private:
    std::string tag;
    Channel<state_t> in;
};


#endif //FLIGHTCOMPUTER_FUSIONDEBUG_HPP
