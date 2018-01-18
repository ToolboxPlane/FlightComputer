/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QGridLayout *gridLayout;
    QLabel *labelID;
    QSpinBox *inputID;
    QCheckBox *checkBoxMesh;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *inputTTL;
    QLabel *label_5;
    QTextBrowser *textBrowserTransmit;
    QPushButton *buttonTransmit;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *spinTransmitterIdFilter;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpinBox *spinUID;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_8;
    QSpinBox *spinChannelCount;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_9;
    QSpinBox *spinResolution;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_10;
    QCheckBox *checkError;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QCheckBox *checkIsMesh;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_12;
    QSpinBox *spinRoutingLength;
    QTextBrowser *textReceive;
    QFrame *line_2;
    QTextBrowser *textBrowser;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(584, 409);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout->addWidget(label_6);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelID = new QLabel(centralWidget);
        labelID->setObjectName(QStringLiteral("labelID"));

        gridLayout->addWidget(labelID, 0, 0, 1, 1);

        inputID = new QSpinBox(centralWidget);
        inputID->setObjectName(QStringLiteral("inputID"));
        inputID->setMaximum(255);
        inputID->setValue(38);

        gridLayout->addWidget(inputID, 0, 1, 1, 1);

        checkBoxMesh = new QCheckBox(centralWidget);
        checkBoxMesh->setObjectName(QStringLiteral("checkBoxMesh"));

        gridLayout->addWidget(checkBoxMesh, 1, 1, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        inputTTL = new QSpinBox(centralWidget);
        inputTTL->setObjectName(QStringLiteral("inputTTL"));
        inputTTL->setMaximum(7);
        inputTTL->setValue(7);

        gridLayout->addWidget(inputTTL, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setTextFormat(Qt::PlainText);

        verticalLayout->addWidget(label_5);

        textBrowserTransmit = new QTextBrowser(centralWidget);
        textBrowserTransmit->setObjectName(QStringLiteral("textBrowserTransmit"));
        textBrowserTransmit->setReadOnly(false);

        verticalLayout->addWidget(textBrowserTransmit);

        buttonTransmit = new QPushButton(centralWidget);
        buttonTransmit->setObjectName(QStringLiteral("buttonTransmit"));
        buttonTransmit->setEnabled(true);
        buttonTransmit->setFlat(false);

        verticalLayout->addWidget(buttonTransmit);


        horizontalLayout_8->addLayout(verticalLayout);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_8->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_2->addWidget(label_7);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        spinTransmitterIdFilter = new QSpinBox(centralWidget);
        spinTransmitterIdFilter->setObjectName(QStringLiteral("spinTransmitterIdFilter"));
        spinTransmitterIdFilter->setMinimum(-1);
        spinTransmitterIdFilter->setMaximum(255);
        spinTransmitterIdFilter->setValue(-1);

        horizontalLayout_2->addWidget(spinTransmitterIdFilter);


        verticalLayout_2->addLayout(horizontalLayout_2);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        spinUID = new QSpinBox(centralWidget);
        spinUID->setObjectName(QStringLiteral("spinUID"));
        spinUID->setReadOnly(true);

        horizontalLayout->addWidget(spinUID);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_3->addWidget(label_8);

        spinChannelCount = new QSpinBox(centralWidget);
        spinChannelCount->setObjectName(QStringLiteral("spinChannelCount"));
        spinChannelCount->setEnabled(true);
        spinChannelCount->setReadOnly(true);

        horizontalLayout_3->addWidget(spinChannelCount);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_4->addWidget(label_9);

        spinResolution = new QSpinBox(centralWidget);
        spinResolution->setObjectName(QStringLiteral("spinResolution"));
        spinResolution->setReadOnly(true);

        horizontalLayout_4->addWidget(spinResolution);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_5->addWidget(label_10);

        checkError = new QCheckBox(centralWidget);
        checkError->setObjectName(QStringLiteral("checkError"));

        horizontalLayout_5->addWidget(checkError);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_6->addWidget(label_11);

        checkIsMesh = new QCheckBox(centralWidget);
        checkIsMesh->setObjectName(QStringLiteral("checkIsMesh"));

        horizontalLayout_6->addWidget(checkIsMesh);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_7->addWidget(label_12);

        spinRoutingLength = new QSpinBox(centralWidget);
        spinRoutingLength->setObjectName(QStringLiteral("spinRoutingLength"));
        spinRoutingLength->setReadOnly(true);

        horizontalLayout_7->addWidget(spinRoutingLength);


        verticalLayout_2->addLayout(horizontalLayout_7);

        textReceive = new QTextBrowser(centralWidget);
        textReceive->setObjectName(QStringLiteral("textReceive"));

        verticalLayout_2->addWidget(textReceive);


        horizontalLayout_8->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_8);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_2);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setMaximumSize(QSize(16777215, 200));

        verticalLayout_3->addWidget(textBrowser);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "RcLibMonitor", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><h3>Transmit</h3></body></html>", Q_NULLPTR));
        labelID->setText(QApplication::translate("MainWindow", "ID of this Computer", Q_NULLPTR));
        checkBoxMesh->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Mesh-Enabled", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "TTL", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Channel data:", Q_NULLPTR));
        textBrowserTransmit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">17</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">38</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">45</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">90</p>\n"
"<p style=\" margin-top:0px; m"
                        "argin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">67</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">123</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">789</p></body></html>", Q_NULLPTR));
        buttonTransmit->setText(QApplication::translate("MainWindow", "Transmit", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "<html><head/><body><h3>Receive</h3></body></html>", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Transmitter-ID Filter", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "UID", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Channel Count", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Resolution", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Error", Q_NULLPTR));
        checkError->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "Is Mesh", Q_NULLPTR));
        checkIsMesh->setText(QString());
        label_12->setText(QApplication::translate("MainWindow", "Routing Length", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
