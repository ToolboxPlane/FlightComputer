#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtDBus>
#include <QDebug>

struct RecordingLine {
    qint64 timestamp;
    QString sender;
    QString signalName;
    QString data;
};

RecordingLine parseLine(QString line) {
    RecordingLine ret;
    QStringList separated = line.split(',');
    ret.timestamp = separated[0].toLong();
    ret.sender = separated[1];
    ret.signalName = separated[2];
    for(int c=3; c<separated.size(); c++) {
        ret.data += separated[c];
    }
    return ret;
}

void sendData(RecordingLine data, QString name, QString objectpath) {
    qDebug() << "Sending: " << data.timestamp << data.sender << data.signalName << data.data;
    QStringList signalSep = data.signalName.split(".");
    QString realSignalName = signalSep[signalSep.size()-1];
    QDBusMessage msg = QDBusMessage::createSignal(objectpath,name, realSignalName);
    QDBusConnection::sessionBus().send(msg);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile inputFile("../SerialReplay/testrecording");
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening File!";
    }

    QTextStream in(&inputFile);
    QString name = in.readLine();
    QString objectpath = in.readLine();
    qDebug() << "Name: " << name << "\tObjectPath: " << objectpath;

    QString line = in.readLine();

    RecordingLine begin = parseLine(line);

    qint64 startTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
    qint64 recordingStartTime = begin.timestamp;

    sendData(begin, name, objectpath);

    while (!in.atEnd())
    {
        RecordingLine currLine = parseLine(in.readLine());

        qint64 recordingDiff = currLine.timestamp - recordingStartTime;
        qint64 realDiff = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - startTime;
        while (realDiff < recordingDiff){
            realDiff = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - startTime;
        }
        sendData(currLine, name, objectpath);
    }
    inputFile.close();
    qDebug() << "Recording finished!";
    return a.exec();
}

