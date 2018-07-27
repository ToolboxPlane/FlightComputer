//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_LORA_HPP
#define FLIGHTCOMPUTER_LORA_HPP

#include "../../Filter.hpp"
#include "../../Channel.hpp"
#include "RH_RF95.h"
#include <unistd.h>
#include "../rcLib/PackageExtended.hpp"
#include "../../MultipleOutputChannel.hpp"

class LoRa : public Filter {
public:
    LoRa();

    MultipleOutputChannel<rcLib::PackageExtended> &getChannelOut();
    Channel<rcLib::PackageExtended> &getChannelIn();
private:
    Channel<rcLib::PackageExtended> in;
    MultipleOutputChannel<rcLib::PackageExtended> out;
    void run() override;
    uint8_t* buf;
    uint8_t len;
    RH_RF95 rf95;
};


#endif //FLIGHTCOMPUTER_LORA_HPP
