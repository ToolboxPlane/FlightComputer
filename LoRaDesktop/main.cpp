#include "mainwindow.h"
#include <QApplication>

#include <QtDBus/QtDBus>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Cannot connect to the D-Bus session bus.\n"
                       << "To start it, run:\n"
                       << "\teval `dbus-launch --auto-syntax`\n";

        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
