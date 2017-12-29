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

    QDBusConnection::sessionBus().registerObject("/serial", &serial, QDBusConnection::ExportAllSlots);

    return a.exec();
}

