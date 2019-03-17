//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_LORA_HPP
#define FLIGHTCOMPUTER_LORA_HPP

#include "../../Node.hpp"
#include "../../InputChannel.hpp"
#include <unistd.h>
#include "../rcLib/PackageExtended.hpp"
#include "../../OutputChannel.hpp"

namespace device {
    class LoRa : public Filter {
    public:
        LoRa();

        MultipleOutputChannel <rcLib::PackageExtended> &getChannelOut();

        Channel <rcLib::PackageExtended> &getChannelIn();

    private:
        Channel <rcLib::PackageExtended> in;
        MultipleOutputChannel <rcLib::PackageExtended> out;

        void run() override;

        void selectReceiver();

        void unselectReceiver();

        unsigned char readRegister(unsigned char addr);

        void writeRegister(unsigned char addr, unsigned char value);

        void setOpMode(unsigned char mode);

        void setOpModeLoRa();

        void setupLoRa();

        bool receive(char *payload);

        void configPower(int8_t power);

        void writeBuf(unsigned char addr, unsigned char *value, unsigned char len);

        void txLoRa(unsigned char *frame, unsigned char len);


        const int CHANNEL = 0;
        char message[256];
        bool sx1272 = true;
        unsigned char receivedbytes;
        enum sf_t {
            SF7 = 7, SF8, SF9, SF10, SF11, SF12
        };

        /*******************************************************************************
         *
         * Configure these values!
         *
         *******************************************************************************/

        // SX1272 - Raspberry connections
        int ssPin = 6;
        int dio0 = 7;
        int RST = 0;

        // Set spreading factor (SF7 - SF12)
        sf_t sf = SF7;

        // Set center frequency
        uint32_t freq = 434000000; // in Mhz! (868.1)
    };
}

#endif //FLIGHTCOMPUTER_LORA_HPP
