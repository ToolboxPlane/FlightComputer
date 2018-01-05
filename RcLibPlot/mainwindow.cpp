#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDBusConnection::sessionBus().connect("", "/rcdata", "de.toolboxbodensee.plane", "onRcPackageReceived", this, SLOT(dbusSignalReceived(QString)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    int viewPortHeight = this->size().height()-OFFSET;
    int viewPortWidth = this->size().width();

    // Background
    painter.fillRect(0,OFFSET,viewPortWidth,viewPortHeight, Qt::white);

    // Update Data
    if(!pause) {
        for(int c=0; c<channelCount; c++) {
            for(int s=1; s<samplesOnScreen; s++) {
                channelDataBuffer[c][s-1] = channelDataBuffer[c][s];
            }
            channelDataBuffer[c][samplesOnScreen-1] = channelData[c];
        }
    }

    // Draw Data
    for(int c=0; c<channelCount; c++) {
        painter.setPen(QPen(CHANNEL_COLORS[c%8], 2));
        for(int s=1; s<samplesOnScreen; s++) {
            painter.drawLine((s-1) * (viewPortWidth) / samplesOnScreen,
                             OFFSET + viewPortHeight - channelDataBuffer[c][s-1] * viewPortHeight / resolution,
                             s * (viewPortWidth) / samplesOnScreen,
                             OFFSET + viewPortHeight - channelDataBuffer[c][s] * viewPortHeight / resolution);
        }
    }

    // Axis
    painter.setPen(QPen(Qt::black, 4));
    painter.drawLine(0, OFFSET, 0, OFFSET + viewPortHeight);
    for(int x=0; x<10; x++) {
        painter.drawLine(0, OFFSET + viewPortHeight - x * (viewPortHeight) / 10,
                         10,OFFSET + viewPortHeight - x * (viewPortHeight) / 10);
        painter.drawText(12,OFFSET + viewPortHeight - x * (viewPortHeight) / 10,
                QString::number(x/10.0 * resolution));
    }

    painter.drawLine(0, this->size().height(), this->size().width(),this->size().height());
    for(int y=0; y<10; y++) {
        painter.drawLine(viewPortWidth - y * (viewPortWidth) / 10, OFFSET + viewPortHeight,
                         viewPortWidth - y * (viewPortWidth) / 10, OFFSET + viewPortHeight-10);
        painter.drawText(viewPortWidth - y * (viewPortWidth) / 10, OFFSET + viewPortHeight-12,
                QString::number(-y/10.0 * samplesOnScreen));
    }

    for(int c=0; c<channelCount; c++) {
        painter.setPen(QPen(CHANNEL_COLORS[c%8], 2));
        painter.drawText(viewPortWidth-72, OFFSET + (c + 1) * 12,
                "Channel: " + QString::number(c));
    }
}

void MainWindow::dbusSignalReceived(QString value)
{
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(value.toLocal8Bit(), &err);

    if (!document.isObject()) {
        qDebug() << "Document is not an object";
        return;
    }
    QJsonObject object = document.object();
    QJsonObject header = object.value("Header").toObject();
    QJsonObject config = object.value("Config").toObject();
    QJsonArray channel = object.value("Channel").toArray();

    int tid = header.value("TransmitterId").toInt(-1);

    if(ui->spinTransmitterIdFilter->value() == tid) {
        channelCount = config.value("ChannelCount").toInt(-1);
        resolution = 1024;//config.value("Resolution").toInt(-1); //@TODO Change to Resolution

        channelData.resize(channelCount);
        channelDataBuffer.resize(channelCount);

        samplesOnScreen = ui->spinSamplesOnScreen->value();
        for(int c=0; c<channelCount; c++) {
            channelData[c] = channel.at(c).toInt(-1);
            channelDataBuffer[c].resize(samplesOnScreen);
        }
    }
}

void MainWindow::on_buttonPause_clicked()
{
    pause = !pause;
    ui->buttonPause->setText(pause?"Resume":"Pause");
}
