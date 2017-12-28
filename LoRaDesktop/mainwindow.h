#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtDBus/QtDBus>


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
    void read();

    void on_buttonTransmit_clicked();

private:
    Ui::MainWindow *ui;
    QDBusInterface dbusInterface;
};

#endif // MAINWINDOW_H
