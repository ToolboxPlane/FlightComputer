#include "mainwindow.h"
#include "ui_mainwindow.h"

#define _USE_DBUS

#ifdef _USE_DBUS
#include <QtDBus/QtDBus>
#endif

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
    static rcLib::Package receivePackage;
    QByteArray data = serial->readAll();

    for(int c=0; c<data.length(); c++){
        //qDebug() << QString::number((unsigned char)data[c]) << "\t" << QString::number((unsigned char)data[c], 2);
        if(receivePackage.decode(data[c])){
            ui->textBrowser->append("New Package received (Transmitter-ID: "+
                                    QString::number(receivePackage.getDeviceId())+")");
            if(!receivePackage.isChecksumCorrect()) {
                ui->textBrowser->append("Checksum incorrect");
            }

            QString text;
            for(size_t c=0; c<8; c++) {
               text += "Channel " + QString::number(c) + ":" + QString::number(receivePackage.getChannel(c)) + "<br>";
            }
            ui->textReceive->setHtml(text);
        }
    }
}

void MainWindow::on_btnConnect_clicked()
{
#ifdef _USE_DBUS
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return;
    }
    ui->btnConnect->setEnabled(false);
    ui->buttonTransmit->setEnabled(true);
#else
    serial = new QSerialPort(ui->portInput->text());

    if(serial->open(QIODevice::ReadWrite)) {
        serial->setBaudRate(ui->baudInput->value());
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(read()));

        ui->btnConnect->setEnabled(false);
        ui->buttonTransmit->setEnabled(true);
    } else {
        serial->close();
        ui->textBrowser->append("Cannot connect to serial port");
    }
#endif
}

void MainWindow::on_buttonTransmit_clicked()
{
#ifdef _USE_DBUS
    QDBusInterface iface("de.toolbox-bodensee.plane", "/flightcontroller", "", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusReply<bool> reply = iface.call("setChannel", 1, 2);
        if (reply.isValid()) {
            qDebug() << "Reply:" << reply.value();
        } else {
            qDebug() << "Error:" << reply.error();
        }
    }
#else
    QStringList lines = ui->textBrowserTransmit->toPlainText().split(QRegExp("[\n]"),QString::SkipEmptyParts);

    rcLib::Package package(1024, 8);

    for(size_t c=0; c<lines.size(); c++) {
       // qDebug() << lines[c].toInt();
        package.setChannel(c, lines[c].toInt());
    }

    uint8_t l = package.encode();
    serial->write((char*)package.getEncodedData(), l);
#endif
}
