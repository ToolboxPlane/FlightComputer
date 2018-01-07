#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtDBus>
#include <QCommandLineParser>
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
    for(int c=3; c<separated.size()-1; c++) {
        ret.data += separated[c] + ",";
    }
    ret.data += separated[separated.size()-1];
    return ret;
}

void sendData(RecordingLine data, QString name, QString objectpath, bool debug) {
    if(debug) {
        qDebug() << "Sending: " << data.timestamp << data.sender << data.signalName << data.data;
    }
    QStringList signalSep = data.signalName.split(".");
    QString realSignalName = signalSep[signalSep.size()-1];
    QDBusMessage msg = QDBusMessage::createSignal(objectpath,name, realSignalName);
    QVariant content(data.data);
    msg << content;
    QDBusConnection::sessionBus().send(msg);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("SerialReplay");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Replays a recording");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("input", "One or more recording files");
    parser.addOptions({
        {{"n", "norealtime"}, "Replay all signals imediatly"},
        {{"d", "debug"}, "Print more debug information"}
    });

    parser.process(a);

    QFile inputFile(parser.positionalArguments()[0]);
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

    sendData(begin, name, objectpath, parser.isSet("d"));

    while (!in.atEnd())
    {
        RecordingLine currLine = parseLine(in.readLine());

        qint64 recordingDiff = currLine.timestamp - recordingStartTime;
        qint64 realDiff = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - startTime;
        while (!parser.isSet("n") && realDiff < recordingDiff){
            realDiff = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - startTime;
        }
        sendData(currLine, name, objectpath, parser.isSet("d"));
    }
    inputFile.close();
    qDebug() << "Replay finished!";
    return 0;
}

