#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtDBus/QtDBus>
#include <QJsonObject>
#include <QJsonDocument>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonTransmit_clicked();

    void dbusSignalReceived(QString);

private:
    Ui::MainWindow *ui;
    QDBusInterface dbusInterface;
};

#endif // MAINWINDOW_H
