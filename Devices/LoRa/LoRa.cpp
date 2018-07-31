//
// Created by paul on 15.03.18.
//

#include "LoRa.hpp"

#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/ioctl.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <chrono>
#include <thread>

// #############################################
// #############################################

#define REG_FIFO                    0x00
#define REG_OPMODE                  0x01
#define REG_FIFO_ADDR_PTR           0x0D
#define REG_FIFO_TX_BASE_AD         0x0E
#define REG_FIFO_RX_BASE_AD         0x0F
#define REG_RX_NB_BYTES             0x13
#define REG_FIFO_RX_CURRENT_ADDR    0x10
#define REG_IRQ_FLAGS               0x12
#define REG_DIO_MAPPING_1           0x40
#define REG_DIO_MAPPING_2           0x41
#define REG_MODEM_CONFIG            0x1D
#define REG_MODEM_CONFIG2           0x1E
#define REG_MODEM_CONFIG3           0x26
#define REG_SYMB_TIMEOUT_LSB  		0x1F
#define REG_PKT_SNR_VALUE			0x19
#define REG_PAYLOAD_LENGTH          0x22
#define REG_IRQ_FLAGS_MASK          0x11
#define REG_MAX_PAYLOAD_LENGTH 		0x23
#define REG_HOP_PERIOD              0x24
#define REG_SYNC_WORD				0x39
#define REG_VERSION	  				0x42

#define PAYLOAD_LENGTH              0x40

// LOW NOISE AMPLIFIER
#define REG_LNA                     0x0C
#define LNA_MAX_GAIN                0x23
#define LNA_OFF_GAIN                0x00
#define LNA_LOW_GAIN		    	0x20

#define RegDioMapping1                             0x40 // common
#define RegDioMapping2                             0x41 // common

#define RegPaConfig                                0x09 // common
#define RegPaRamp                                  0x0A // common
#define RegPaDac                                   0x5A // common

#define SX72_MC2_FSK                0x00
#define SX72_MC2_SF7                0x70
#define SX72_MC2_SF8                0x80
#define SX72_MC2_SF9                0x90
#define SX72_MC2_SF10               0xA0
#define SX72_MC2_SF11               0xB0
#define SX72_MC2_SF12               0xC0

#define SX72_MC1_LOW_DATA_RATE_OPTIMIZE  0x01 // mandated for SF11 and SF12

// sx1276 RegModemConfig1
#define SX1276_MC1_BW_125                0x70
#define SX1276_MC1_BW_250                0x80
#define SX1276_MC1_BW_500                0x90
#define SX1276_MC1_CR_4_5            0x02
#define SX1276_MC1_CR_4_6            0x04
#define SX1276_MC1_CR_4_7            0x06
#define SX1276_MC1_CR_4_8            0x08

#define SX1276_MC1_IMPLICIT_HEADER_MODE_ON    0x01

// sx1276 RegModemConfig2
#define SX1276_MC2_RX_PAYLOAD_CRCON        0x04

// sx1276 RegModemConfig3
#define SX1276_MC3_LOW_DATA_RATE_OPTIMIZE  0x08
#define SX1276_MC3_AGCAUTO                 0x04

// preamble for lora networks (nibbles swapped)
#define LORA_MAC_PREAMBLE                  0x34

#define RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG1 0x0A
#ifdef LMIC_SX1276
#define RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG2 0x70
#elif LMIC_SX1272
#define RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG2 0x74
#endif

// FRF
#define        REG_FRF_MSB              0x06
#define        REG_FRF_MID              0x07
#define        REG_FRF_LSB              0x08

#define        FRF_MSB                  0xD9 // 868.1 Mhz
#define        FRF_MID                  0x06
#define        FRF_LSB                  0x66

// ----------------------------------------
// Constants for radio registers
#define OPMODE_LORA      0x80
#define OPMODE_MASK      0x07
#define OPMODE_SLEEP     0x00
#define OPMODE_STANDBY   0x01
#define OPMODE_FSTX      0x02
#define OPMODE_TX        0x03
#define OPMODE_FSRX      0x04
#define OPMODE_RX        0x05
#define OPMODE_RX_SINGLE 0x06
#define OPMODE_CAD       0x07

// ----------------------------------------
// Bits masking the corresponding IRQs from the radio
#define IRQ_LORA_RXTOUT_MASK 0x80
#define IRQ_LORA_RXDONE_MASK 0x40
#define IRQ_LORA_CRCERR_MASK 0x20
#define IRQ_LORA_HEADER_MASK 0x10
#define IRQ_LORA_TXDONE_MASK 0x08
#define IRQ_LORA_CDDONE_MASK 0x04
#define IRQ_LORA_FHSSCH_MASK 0x02
#define IRQ_LORA_CDDETD_MASK 0x01

// DIO function mappings                D0D1D2D3
#define MAP_DIO0_LORA_RXDONE   0x00  // 00------
#define MAP_DIO0_LORA_TXDONE   0x40  // 01------
#define MAP_DIO1_LORA_RXTOUT   0x00  // --00----
#define MAP_DIO1_LORA_NOP      0x30  // --11----
#define MAP_DIO2_LORA_NOP      0xC0  // ----11--


void LoRa::selectReceiver()
{
    digitalWrite(ssPin, LOW);
}

void LoRa::unselectReceiver()
{
    digitalWrite(ssPin, HIGH);
}

unsigned char LoRa::readRegister(unsigned char addr)
{
    unsigned char spibuf[2];

    selectReceiver();
    spibuf[0] = addr & 0x7F;
    spibuf[1] = 0x00;
    wiringPiSPIDataRW(CHANNEL, spibuf, 2);
    unselectReceiver();

    return spibuf[1];
}

void LoRa::writeRegister(unsigned char addr, unsigned char value)
{
    unsigned char spibuf[2];

    spibuf[0] = addr | 0x80;
    spibuf[1] = value;
    selectReceiver();
    wiringPiSPIDataRW(CHANNEL, spibuf, 2);

    unselectReceiver();
}

void LoRa::setOpMode (uint8_t mode) {
    writeRegister(REG_OPMODE, (readRegister(REG_OPMODE) & ~OPMODE_MASK) | mode);
}

void LoRa::setOpModeLoRa() {
    uint8_t u = OPMODE_LORA;
    if (sx1272 == false)
        u |= 0x8;   // TBD: sx1276 high freq
    writeRegister(REG_OPMODE, u);
}

void LoRa::setupLoRa()
{

    digitalWrite(RST, HIGH);
    delay((unsigned int)100);
    digitalWrite(RST, LOW);
    delay((unsigned int)100);

    unsigned char version = readRegister(REG_VERSION);

    if (version == 0x22) {
        // sx1272
        sx1272 = true;
    } else {
        // sx1276?
        digitalWrite(RST, LOW);
        delay((unsigned int)100);
        digitalWrite(RST, HIGH);
        delay((unsigned int)100);
        version = readRegister(REG_VERSION);
        if (version == 0x12) {
            // sx1276
            sx1272 = false;
        } else {
            printf("Unrecognized transceiver.\n");
            //printf("Version: 0x%x\n",version);
            exit(1);
        }
    }

    setOpMode(OPMODE_SLEEP);

    // set frequency
    uint64_t frf = ((uint64_t)freq << 19) / 32000000;
    writeRegister(REG_FRF_MSB, (uint8_t)(frf>>16) );
    writeRegister(REG_FRF_MID, (uint8_t)(frf>> 8) );
    writeRegister(REG_FRF_LSB, (uint8_t)(frf>> 0) );

    writeRegister(REG_SYNC_WORD, 0x34); // LoRaWAN public sync word

    if (sx1272) {
        if (sf == SF11 || sf == SF12) {
            writeRegister(REG_MODEM_CONFIG,0x0B);
        } else {
            writeRegister(REG_MODEM_CONFIG,0x0A);
        }
        writeRegister(REG_MODEM_CONFIG2,(sf<<4) | 0x04);
    } else {
        if (sf == SF11 || sf == SF12) {
            writeRegister(REG_MODEM_CONFIG3,0x0C);
        } else {
            writeRegister(REG_MODEM_CONFIG3,0x04);
        }
        writeRegister(REG_MODEM_CONFIG,0x72);
        writeRegister(REG_MODEM_CONFIG2,(sf<<4) | 0x04);
    }

    if (sf == SF10 || sf == SF11 || sf == SF12) {
        writeRegister(REG_SYMB_TIMEOUT_LSB,0x05);
    } else {
        writeRegister(REG_SYMB_TIMEOUT_LSB,0x08);
    }
    writeRegister(REG_MAX_PAYLOAD_LENGTH,0x80);
    writeRegister(REG_PAYLOAD_LENGTH,PAYLOAD_LENGTH);
    writeRegister(REG_HOP_PERIOD,0xFF);
    writeRegister(REG_FIFO_ADDR_PTR, readRegister(REG_FIFO_RX_BASE_AD));

    writeRegister(REG_LNA, LNA_MAX_GAIN);

}

bool LoRa::receive(char *payload) {
    // clear rxDone
    writeRegister(REG_IRQ_FLAGS, 0x40);

    int irqflags = readRegister(REG_IRQ_FLAGS);

    //  payload crc: 0x20
    if((irqflags & 0x20) == 0x20)
    {
        printf("CRC error\n");
        writeRegister(REG_IRQ_FLAGS, 0x20);
        return false;
    } else {

        unsigned char currentAddr = readRegister(REG_FIFO_RX_CURRENT_ADDR);
        unsigned char receivedCount = readRegister(REG_RX_NB_BYTES);
        receivedbytes = receivedCount;

        writeRegister(REG_FIFO_ADDR_PTR, currentAddr);

        for(int i = 0; i < receivedCount; i++)
        {
            payload[i] = (char)readRegister(REG_FIFO);
        }
    }
    return true;
}

void LoRa::configPower (int8_t pw) {
    if (sx1272 == false) {
        // no boost used for now
        if(pw >= 17) {
            pw = 15;
        } else if(pw < 2) {
            pw = 2;
        }
        // check board type for BOOST pin
        writeRegister(RegPaConfig, (uint8_t)(0x80|(pw&0xf)));
        writeRegister(RegPaDac, readRegister(RegPaDac)|0x4);

    } else {
        // set PA config (2-17 dBm using PA_BOOST)
        if(pw > 17) {
            pw = 17;
        } else if(pw < 2) {
            pw = 2;
        }
        writeRegister(RegPaConfig, (uint8_t)(0x80|(pw-2)));
    }
}


void LoRa::writeBuf(unsigned char addr, unsigned char* value, unsigned char len) {
    unsigned char spibuf[256];
    spibuf[0] = addr | 0x80;
    for (int i = 0; i < len; i++) {
        spibuf[i + 1] = value[i];
    }
    selectReceiver();
    wiringPiSPIDataRW(CHANNEL, spibuf, len + 1);
    unselectReceiver();
}

void LoRa::txLoRa(unsigned char *frame, unsigned char datalen) {
    // set the IRQ mapping DIO0=TxDone DIO1=NOP DIO2=NOP
    writeRegister(RegDioMapping1, MAP_DIO0_LORA_TXDONE|MAP_DIO1_LORA_NOP|MAP_DIO2_LORA_NOP);
    // clear all radio IRQ flags
    writeRegister(REG_IRQ_FLAGS, 0xFF);
    // mask all IRQs but TxDone
    writeRegister(REG_IRQ_FLAGS_MASK, ~IRQ_LORA_TXDONE_MASK);

    // initialize the payload size and address pointers
    writeRegister(REG_FIFO_TX_BASE_AD, 0x00);
    writeRegister(REG_FIFO_ADDR_PTR, 0x00);
    writeRegister(REG_PAYLOAD_LENGTH, datalen);

    // download buffer to the radio FIFO
    writeBuf(REG_FIFO, frame, datalen);
    // now we actually start the transmission
    setOpMode(OPMODE_TX);
}


LoRa::LoRa() : Filter(){
    wiringPiSetup();
    pinMode(ssPin, OUTPUT);
    pinMode(dio0, INPUT);
    pinMode(RST, OUTPUT);

    wiringPiSPISetup(CHANNEL, 500000);

    setupLoRa();

    setOpModeLoRa();
    setOpMode(OPMODE_STANDBY);

    writeRegister(RegPaRamp, (readRegister(RegPaRamp) & 0xF0) | 0x08); // set PA ramp-up time 50 uSec

    configPower(23);

    setOpMode(OPMODE_RX);

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
    long int SNR;
    int rssicorr;
    while(true) {
        if(digitalRead(dio0) == 1)
        {
            if(receive(message)) {
                unsigned char value = readRegister(REG_PKT_SNR_VALUE);
                if( value & 0x80 ) // The SNR sign bit is 1
                {
                    // Invert and divide by 4
                    value = ( ( ~value + 1 ) & 0xFF ) >> 2;
                    SNR = -value;
                }
                else
                {
                    // Divide by 4
                    SNR = ( value & 0xFF ) >> 2;
                }

                if (sx1272) {
                    rssicorr = 139;
                } else {
                    rssicorr = 157;
                }

                //printf("Packet RSSI: %d, ", readRegister(0x1A)-rssicorr);
                //printf("RSSI: %d, ", readRegister(0x1B)-rssicorr);
                //printf("SNR: %li, ", SNR);
                //printf("Length: %i", (int)receivedbytes);
                //printf("\n");

                for(auto c=0; c<receivedbytes; c++) {
                    if(pkgIn.decode(message[c])) {
                        out.put(pkgIn);
                    }
                }

            } 
        } 

/*        if(in.get(pkgOut, false)) {
//            setOpMode(OPMODE_STANDBY);
            auto len = pkgOut.encode();
            txLoRa(pkgOut.getEncodedData(), len);
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            std::cout << "Sending..." << std::endl;
//            setOpMode(OPMODE_RX);
        }*/
    }
}
