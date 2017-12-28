#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include "RadioControlProtocol/rcLib.hpp"

class Serial : public QObject
{
    Q_OBJECT
public:
    Serial(QString port, int baud);

signals:

public slots:
    Q_SCRIPTABLE int getChannel(int channel);
    Q_SCRIPTABLE void setChannel(int channel, int val);

private slots:
    void read();
    void send();

private:
    QSerialPort* serial;
    rcLib::Package pkgIn, pkgOut;
};

#endif // SERIAL_H
