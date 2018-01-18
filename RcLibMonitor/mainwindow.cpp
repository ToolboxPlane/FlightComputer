#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbusInterface("de.toolboxbodensee.plane", "/serial", "", QDBusConnection::systemBus())
{
    ui->setupUi(this);

    /*if (!dbusInterface.isValid()) {
        qDebug() << "Interface is not valid";
        exit(1);
    }*/

    QDBusConnection::systemBus().connect("", "/rcdata", "de.toolboxbodensee.plane", "onRcPackageReceived", this, SLOT(dbusSignalReceived(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonTransmit_clicked()
{
   QStringList lines = ui->textBrowserTransmit->toPlainText().split(QRegExp("[\n]"),QString::SkipEmptyParts);

    /*for(size_t c=0; c<lines.size(); c++) {
        QDBusReply<void> reply = dbusInterface.call("setChannel", (int)c, lines[c].toInt());
        if (!reply.isValid()) {
            qDebug() << reply.error();
        }
    } */
}


/*
{
    "Header":{
        "Uid":1,
        "TransmitterId":0
    },
    "Config":{
       "ChannelCount":8,
        "Resolution":5,
        "Error":false,
        "IsMeshPackage":false,
        "RoutingLength":0
    },
    "Channel":[
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8
    ]
}
 */
void MainWindow::dbusSignalReceived(QString value)
{
    //value = "{\"Header\":{\"Uid\":1,\"TransmitterId\":0},\"Config\":{\"ChannelCount\":8,\"Resolution\":5,\"Error\":false,\"IsMeshPackage\":false,\"Mesh\":0,\"RoutingLength\":0},\"Channel\":[1,2,3,4,5,6,7,8]}";

    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(value.toLocal8Bit(), &err);

    if (!document.isObject()) {
        qDebug() << "Document is not an object";
        return;
    }
    QJsonObject object = document.object();
    QJsonObject header = object.value("Header").toObject();
    QJsonObject config = object.value("Config").toObject();
    QJsonValue channel = object.value("Channel");

    int uid = header.value("Uid").toInt(-1);
    int tid = header.value("TransmitterId").toInt(-1);

    ui->textBrowser->append("New Package from " + QString::number(tid) +
                            " with UID: " + QString::number(uid));


    if(ui->spinTransmitterIdFilter->value() == -1 || ui->spinTransmitterIdFilter->value() == tid) {
        int channelCount = config.value("ChannelCount").toInt(-1);
        int resolution = config.value("Resolution").toInt(-1);
        bool error = config.value("Error").toBool(true);
        bool isMesh = config.value("IsMeshPackage").toBool(false);
        int routingLength = config.value("RoutingLength").toInt(-1);

        ui->spinChannelCount->setValue(channelCount);
        ui->spinResolution->setValue(resolution);
        ui->checkError->setEnabled(error);
        ui->checkBoxMesh->setEnabled(isMesh);
        ui->spinRoutingLength->setValue(routingLength);

        QJsonArray channels = channel.toArray();
        QString text;
        for(int c=0; c<channelCount; c++) {
            text += "Channel " + QString::number(c) + ": " + QString::number(channels.at(c).toDouble(-1)) + "<br>";
        }

        ui->textReceive->setHtml(text);
    }
}
