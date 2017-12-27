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

        pkgOut = rcLib::Package(1024, 8);
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(send()));
        timer->start(20);

    } else {
        serial->close();
        qDebug() << "Cannot connect to serial port";
    }
}

int Serial::getChannel(int channel)
{
    return pkgIn.getChannel(channel);
    printf("getChannel(%d)\n", channel);
}

void Serial::setChannel(int channel, int val)
{
    pkgOut.setChannel(channel, val);
    printf("setChannel(%d, %d)\n", channel, val);
}

/*
 * This is just an example
 */
int Serial::getRoll()
{
    return pkgIn.getChannel(2);
}

void Serial::read()
{
    static rcLib::Package pkgInNew;
    QByteArray data = serial->readAll();

    for(int c=0; c<data.length(); c++){
        if(pkgInNew.decode(data[c])){
            pkgIn = pkgInNew;
        }
    }
}

void Serial::send()
{
    serial->write((char*)pkgOut.getEncodedData(), pkgOut.encode());
}

