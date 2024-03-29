//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_LORA_HPP
#define FLIGHTCOMPUTER_LORA_HPP

#include <unistd.h>

#include "../../Framework/InputChannel.hpp"
#include "../../Framework/Node.hpp"
#include "../../Framework/OutputChannel.hpp"
#include "../rcLib/RadioControlProtocolCpp/rcLib.hpp"

namespace device {
    class LoRa : public Node {
      public:
        LoRa();

        OutputChannel<rcLib::Package> &getChannelOut();

        InputChannel<rcLib::Package> &getChannelIn();

      private:
        InputChannel<rcLib::Package> in;
        OutputChannel<rcLib::Package> out;

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

        void writeBuf(unsigned char addr, const unsigned char *value, unsigned char len);

        void txLoRa(const unsigned char *frame, unsigned char len);


        const int CHANNEL = 0;
        char message[256];
        bool sx1272 = true;
        unsigned char receivedbytes;
        enum sf_t { SF7 = 7, SF8, SF9, SF10, SF11, SF12 };

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
        sf_t sf = SF12;

        // Set center frequency
        uint32_t freq = 434000000; // in Mhz! (868.1)
    };
} // namespace device

#endif // FLIGHTCOMPUTER_LORA_HPP
