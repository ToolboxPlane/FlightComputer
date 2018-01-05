#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtDBus/QtDBus>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void dbusSignalReceived(QString);

    void on_buttonPause_clicked();

private:
    Ui::MainWindow *ui;

    const Qt::GlobalColor CHANNEL_COLORS[8] = {
        Qt::blue, Qt::green, Qt::red, Qt::yellow,
        Qt::black, Qt::cyan, Qt::lightGray, Qt::magenta
    };
    int channelCount = 0;
    QVector<int> channelData;
    QVector<QVector<int> > channelDataBuffer;

    int resolution = 0;
    const int OFFSET = 40;

    int samplesOnScreen = 20;

    bool pause = false;
};

#endif // MAINWINDOW_H
