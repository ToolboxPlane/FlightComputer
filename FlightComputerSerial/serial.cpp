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
        for(int c=0; c<pkgOut.getChannelCount(); c++) {
            pkgOut.setChannel(c, 1023);
        }

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(send()));
        timer->start(500);
    } else {
        serial->close();
        qDebug() << "Cannot connect to serial port " << port;
    }
}

int Serial::getChannel(int channel)
{
    return pkgIn.getChannel(channel);
}

void Serial::setChannel(int channel, int val)
{
    pkgOut.setChannel(channel, val);
}

int Serial::getNumberOfChannels()
{
    return pkgIn.getChannelCount();
}

void Serial::read()
{
    static rcLib::Package pkgInNew;
    QByteArray data = serial->readAll();

    for(int c=0; c<data.length(); c++){
        if(pkgInNew.decode(data[c])){
            qDebug() << "New Package from" << pkgInNew.getDeviceId();
            pkgIn = pkgInNew;
        }
    }
}

void Serial::send()
{
    int length = pkgOut.encode();
    serial->write((char*)pkgOut.getEncodedData(), length);
}

