#include "serial.h"

#include <stdio.h>

Serial::Serial(QString port, int baud)
{
    serial = new QSerialPort(port);

    if(serial->open(QIODevice::ReadWrite)) {
        serial->setBaudRate(baud);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(read()));

        //ui->btnConnect->setEnabled(false);
        //ui->buttonTransmit->setEnabled(true);
    } else {
        serial->close();
        //ui->textBrowser->append("Cannot connect to serial port");
    }
}

QString Serial::test(const QString &arg)
{
    return "true";
}

int Serial::getChannel(int channel)
{
    return channel * channel;
}

bool Serial::setChannel(int channel, int val)
{
    printf("Channel %d set to %d\n", channel, val);
    return true;
}

void Serial::read()
{

}

