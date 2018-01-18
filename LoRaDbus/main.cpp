#include <QCoreApplication>
#include "lorahal.h"
#include "RadioControlProtocol/rcLib.hpp"

LoRaHal lora;
rcLib::Package pkgIn;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    while(true) {
        lora.waitForPackage();
        for(size_t c=0; c<lora.getLength(); c++) {
            if(pkgIn.decode(lora.getData()[c])) {
                qDebug << "New Package";
                QDBusMessage msg = QDBusMessage::createSignal(objectpath,name, realSignalName);
                QVariant content(pkgIn.getChannel(0)+" "+pkgIn.getChannel(1));
                msg << content;
                QDBusConnection::systemBus().send(msg);
            }
        }
    }

    return a.exec();
}

