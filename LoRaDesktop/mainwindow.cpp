#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDBus/QtDBus>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read()
{
    QDBusInterface iface("de.toolbox-bodensee.plane", "/flightcontroller", "", QDBusConnection::sessionBus());
    QString text;

    if (iface.isValid()) {
        for(int c=0; c<8; c++) {
            QDBusReply<bool> reply = iface.call("getChannel", c);
            if (reply.isValid()) {
             //   qDebug() << "Reply:" << reply.value();
                text += "Channel " + QString::number(c) + ": " + reply.value();
            } else {
                qDebug() << "Error:" << reply.error();
                return;
            }
        }
    }
}

void MainWindow::on_btnConnect_clicked()
{
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return;
    }
    ui->btnConnect->setEnabled(false);
    ui->buttonTransmit->setEnabled(true);
}

void MainWindow::on_buttonTransmit_clicked()
{
    QStringList lines = ui->textBrowserTransmit->toPlainText().split(QRegExp("[\n]"),QString::SkipEmptyParts);

    QDBusInterface iface("de.toolbox-bodensee.plane", "/flightcontroller", "", QDBusConnection::sessionBus());

    if (!iface.isValid()) {
        qDebug() << "Interface is not valid";
        return;
    }

    for(size_t c=0; c<lines.size(); c++) {
        QDBusReply<void> reply = iface.call("setChannel", (int)c, lines[c].toInt());
        if (!reply.isValid()) {
            qDebug() << reply.error();
        }
    }
}
