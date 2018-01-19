#include <QCoreApplication>
#include <QtDBus>
#include <QDebug>
#include "LoRaHal.h"
#include "RadioControlProtocol/rcLib.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Starting";

    LoRaHal lora;
    rcLib::Package pkgIn;

    while(true) {
        lora.waitForPackage();
        for(size_t c=0; c<lora.getLength(); c++) {
            if(pkgIn.decode(lora.getData()[c])) {
                qDebug() << "New Package " << pkgIn.getChannel(0) << pkgIn.getChannel(1) << pkgIn.getChannel(2);
                QDBusMessage msg = QDBusMessage::createSignal(
                    "/loraData", "de.toolboxbodensee.plane", "onRcPackageReceived");
                QVariant content(pkgIn.getChannel(0)+" "+pkgIn.getChannel(1));
                msg << content;
                QDBusConnection::systemBus().send(msg);
            }
        }
    }

    return a.exec();
}

