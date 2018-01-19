#include <QCoreApplication>
#include <QtDBus>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
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
                QJsonObject json;
                QJsonObject header, config;
                QJsonArray channel;
                header["Uid"] = 0;
                header["TransmitterId"] = pkgIn.getDeviceId();

                config["ChannelCount"] = pkgIn.getChannelCount();
                config["Resolution"] = pkgIn.getResolution();
                config["Error"] = false;
                config["IsMeshPackage"] = pkgIn.isMesh();
                config["Mesh"] = 0;
                config["RoutingLength"] = 0;

                for(size_t d=0; d<pkgIn.getChannelCount(); d++) {
                    channel.append( pkgIn.getChannel(d));
                }

                json["Header"] = header;
                json["Config"] = config;
                json["Channel"] = channel;

                QJsonDocument doc(json);
                QString payload(doc.toJson(QJsonDocument::Compact));

                QDBusMessage msg = QDBusMessage::createSignal(
                    "/loraData", "de.toolboxbodensee.plane", "onRcPackageReceived");
                QVariant content(payload);
                msg << content;
                QDBusConnection::systemBus().send(msg);
            }
        }
    }

    return a.exec();
}

