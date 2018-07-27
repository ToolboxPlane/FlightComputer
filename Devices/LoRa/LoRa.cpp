//
// Created by paul on 15.03.18.
//

#include "LoRa.hpp"
#include <iostream>

#define BOARD_DRAGINO_PIHAT

#include "RasPiBoards.h"

// Our RFM95 Configuration
#define RF_FREQUENCY  434.00
#define RF_NODE_ID    1

LoRa::LoRa() : Filter(), rf95(RF_CS_PIN, RF_IRQ_PIN){
    if (!bcm2835_init()) {
        fprintf( stderr, "bcm2835_init() Failed\n\n");
        return;
    }

    printf( "RF95 CS=GPIO%d", RF_CS_PIN);


#ifdef RF_IRQ_PIN
    printf( ", IRQ=GPIO%d", RF_IRQ_PIN );
    // IRQ Pin input/pull down
    pinMode(RF_IRQ_PIN, INPUT);
    bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
    // Now we can enable Rising edge detection
    bcm2835_gpio_ren(RF_IRQ_PIN);
#endif

#ifdef RF_RST_PIN
    printf( ", RST=GPIO%d", RF_RST_PIN );
    // Pulse a reset on module
    pinMode(RF_RST_PIN, OUTPUT);
    digitalWrite(RF_RST_PIN, LOW );
    bcm2835_delay(150);
    digitalWrite(RF_RST_PIN, HIGH );
    bcm2835_delay(100);
#endif


    if(!rf95.init()) {
        fprintf( stderr, "\nRF95 module init failed, Please verify wiring/module\n" );
        return;
    }

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    //  driver.setTxPower(23, false);
    // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
    // transmitter RFO pins and not the PA_BOOST pins
    // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true.
    // Failure to do that will result in extremely low transmit powers.
    // rf95.setTxPower(14, true);


    // RF95 Modules don't have RFO pin connected, so just use PA_BOOST
    // check your country max power useable, in EU it's +14dB
    rf95.setTxPower(14, false);

    // You can optionally require this module to wait until Channel Activity
    // Detection shows no activity on the channel before transmitting by setting
    // the CAD timeout to non-zero:
    //rf95.setCADTimeout(10000);

    // Adjust Frequency
    rf95.setFrequency(RF_FREQUENCY);

    // If we need to send something
    rf95.setThisAddress(RF_NODE_ID);
    rf95.setHeaderFrom(RF_NODE_ID);

    // Be sure to grab all node packet
    // we're sniffing to display, it's a demo
    rf95.setPromiscuous(true);

    // We're ready to listen for incoming message
    rf95.setModeRx();

    buf = (uint8_t*)malloc(RH_RF95_MAX_MESSAGE_LEN);

    this->start();
}

MultipleOutputChannel<rcLib::PackageExtended> &LoRa::getChannelOut() {
    return out;
}

Channel<rcLib::PackageExtended> &LoRa::getChannelIn() {
    return in;
}

void LoRa::run() {
    rcLib::PackageExtended pkgIn, pkgOut;
    while(true) {
#ifdef RF_IRQ_PIN
        // We have a IRQ pin ,pool it instead reading
        // Modules IRQ registers from SPI in each loop

        // Rising edge fired ?
        if (bcm2835_gpio_eds(RF_IRQ_PIN)) {
            // Now clear the eds flag by setting it to 1
            bcm2835_gpio_set_eds(RF_IRQ_PIN);
            //printf("Packet Received, Rising event detect for pin GPIO%d\n", RF_IRQ_PIN);
#endif

            if (rf95.available()) {
                std::cout << "available" << std::endl;
                // Should be a message for us now
                uint8_t from = rf95.headerFrom();
                uint8_t to   = rf95.headerTo();
                uint8_t id   = rf95.headerId();
                uint8_t flags= rf95.headerFlags();;
                int8_t rssi  = rf95.lastRssi();

                len = RH_RF95_MAX_MESSAGE_LEN;;
                if (rf95.recv(buf, &len)) {
                    for(auto c=0; c<len; c++) {
                        if(pkgOut.decode(buf[c])) {
                            out.put(pkgOut);
                        }
                    }
                }
            }

#ifdef RF_IRQ_PIN
        }
#endif

        // Let OS doing other tasks
        // For timed critical appliation you can reduce or delete
        // this delay, but this will charge CPU usage, take care and monitor
        bcm2835_delay(5);

        if(in.get(pkgIn, false)) {
            size_t len = pkgIn.encode();
            rf95.send(pkgIn.getEncodedData(), len);
        }
    }
}
