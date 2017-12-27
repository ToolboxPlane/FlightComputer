#include <QCoreApplication>
#include <QtDBus/QtDBus>

#include <stdio.h>
#include "serial.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
       fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
               "To start it, run:\n"
               "\teval `dbus-launch --auto-syntax`\n");
       return 1;
    }

    if (!QDBusConnection::sessionBus().registerService("de.toolbox-bodensee.plane")) {
        fprintf(stderr, "%s\n",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }

    Serial serial("/dev/ttyACM0", 9600);
    QDBusConnection::sessionBus().registerObject("/flightcontroller", &serial, QDBusConnection::ExportAllSlots);


   /* QDBusInterface iface("de.toolbox-bodensee.plane", "/", "", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusMessage msg;
       /*QDBusReply<QString> reply = iface.call("ping", argc > 1 ? argv[1] : "");
       if (reply.isValid()) {
           printf("Reply was: %s\n", qPrintable(reply.value()));
           return 0;
       }

       fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
       return 1;
    }

    fprintf(stderr, "%s\n",
           qPrintable(QDBusConnection::sessionBus().lastError().message()));*/

    return a.exec();
}

