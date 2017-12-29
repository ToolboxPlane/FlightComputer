#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbusInterface("de.toolbox-bodensee.plane", "/serial", "", QDBusConnection::sessionBus())
{
    ui->setupUi(this);

    if (!dbusInterface.isValid()) {
        qDebug() << "Interface is not valid";
        exit(1);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getData()));
    timer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonTransmit_clicked()
{
    QStringList lines = ui->textBrowserTransmit->toPlainText().split(QRegExp("[\n]"),QString::SkipEmptyParts);

    for(size_t c=0; c<lines.size(); c++) {
        QDBusReply<void> reply = dbusInterface.call("setChannel", (int)c, lines[c].toInt());
        if (!reply.isValid()) {
            qDebug() << reply.error();
        }
    }
}

void MainWindow::getData()
{
    QString text;

    QDBusReply<int> channelsReply = dbusInterface.call("getNumberOfChannels");
    if(!channelsReply.isValid()) {
        return;
    }

    for(int c=0; c<channelsReply.value(); c++) {
        QDBusReply<int> reply = dbusInterface.call("getChannel", c);
        if (reply.isValid()) {
            text += "Channel " + QString::number(c) + ": " + QString::number(reply.value()) + "<br>";
        } else {
            qDebug() << "Error:" << reply.error();
            return;
        }
    }

    ui->textReceive->setHtml(text);
}
