#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

class Serial : public QObject
{
    Q_OBJECT
public:
    Serial(QString port, int baud);

signals:

public slots:
    Q_SCRIPTABLE QString test(const QString &arg);
    Q_SCRIPTABLE int getChannel(int channel);
    Q_SCRIPTABLE bool setChannel(int channel, int val);

private slots:
    void read();

private:
    QSerialPort* serial;

};

#endif // SERIAL_H
