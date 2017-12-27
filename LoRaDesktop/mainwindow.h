#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QtSerialPort/QSerialPort"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSerialPort* serial;

private slots:
    void read();

    void on_btnConnect_clicked();

    void on_buttonTransmit_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
