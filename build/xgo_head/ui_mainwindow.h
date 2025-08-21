/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <MatrixLabel.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_1;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_battery;
    QWidget *layoutWidget1;
    QGridLayout *m_gridLayout;
    MatrixLabel *label_01;
    MatrixLabel *label_08;
    MatrixLabel *label_05;
    MatrixLabel *label_07;
    MatrixLabel *label_04;
    MatrixLabel *label_06;
    MatrixLabel *label_00;
    MatrixLabel *label_03;
    MatrixLabel *label_02;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_3;
    QWidget *page_3;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout2;
    QLabel *label_09;
    QLabel *label_10;
    QLabel *label_4;
    QWidget *page_4;
    QLabel *label_info;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(320, 240);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(320, 240));
        MainWindow->setMaximumSize(QSize(320, 240));
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setMinimumSize(QSize(320, 240));
        stackedWidget->setMaximumSize(QSize(320, 240));
        page_1 = new QWidget();
        page_1->setObjectName(QString::fromUtf8("page_1"));
        page_1->setMinimumSize(QSize(320, 240));
        page_1->setMaximumSize(QSize(320, 240));
        layoutWidget = new QWidget(page_1);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 20, 302, 22));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("#label_2{color:rgb(85, 170, 0)}"));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout->addWidget(label_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_battery = new QLabel(layoutWidget);
        label_battery->setObjectName(QString::fromUtf8("label_battery"));
        label_battery->setStyleSheet(QString::fromUtf8("#label_battery{color:rgb(85, 0, 255)}"));
        label_battery->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);

        horizontalLayout->addWidget(label_battery);

        layoutWidget1 = new QWidget(page_1);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 40, 301, 134));
        m_gridLayout = new QGridLayout(layoutWidget1);
        m_gridLayout->setObjectName(QString::fromUtf8("m_gridLayout"));
        m_gridLayout->setContentsMargins(0, 0, 0, 0);
        label_01 = new MatrixLabel(layoutWidget1);
        label_01->setObjectName(QString::fromUtf8("label_01"));
        label_01->setMinimumSize(QSize(40, 40));
        label_01->setMaximumSize(QSize(40, 40));
        label_01->setFrameShape(QFrame::Panel);
        label_01->setLineWidth(0);

        m_gridLayout->addWidget(label_01, 0, 1, 1, 1);

        label_08 = new MatrixLabel(layoutWidget1);
        label_08->setObjectName(QString::fromUtf8("label_08"));
        label_08->setMinimumSize(QSize(40, 40));
        label_08->setMaximumSize(QSize(40, 40));
        label_08->setFrameShape(QFrame::Panel);
        label_08->setFrameShadow(QFrame::Plain);
        label_08->setLineWidth(0);

        m_gridLayout->addWidget(label_08, 2, 2, 1, 1);

        label_05 = new MatrixLabel(layoutWidget1);
        label_05->setObjectName(QString::fromUtf8("label_05"));
        label_05->setMinimumSize(QSize(40, 40));
        label_05->setMaximumSize(QSize(40, 40));
        label_05->setFrameShape(QFrame::Panel);
        label_05->setFrameShadow(QFrame::Plain);
        label_05->setLineWidth(0);

        m_gridLayout->addWidget(label_05, 1, 2, 1, 1);

        label_07 = new MatrixLabel(layoutWidget1);
        label_07->setObjectName(QString::fromUtf8("label_07"));
        label_07->setMinimumSize(QSize(40, 40));
        label_07->setMaximumSize(QSize(40, 40));
        label_07->setFrameShape(QFrame::Panel);
        label_07->setFrameShadow(QFrame::Plain);
        label_07->setLineWidth(0);

        m_gridLayout->addWidget(label_07, 2, 1, 1, 1);

        label_04 = new MatrixLabel(layoutWidget1);
        label_04->setObjectName(QString::fromUtf8("label_04"));
        label_04->setMinimumSize(QSize(40, 40));
        label_04->setMaximumSize(QSize(40, 40));
        label_04->setFrameShape(QFrame::Panel);
        label_04->setFrameShadow(QFrame::Plain);
        label_04->setLineWidth(0);

        m_gridLayout->addWidget(label_04, 1, 1, 1, 1);

        label_06 = new MatrixLabel(layoutWidget1);
        label_06->setObjectName(QString::fromUtf8("label_06"));
        label_06->setMinimumSize(QSize(40, 40));
        label_06->setMaximumSize(QSize(40, 40));
        label_06->setFrameShape(QFrame::Panel);
        label_06->setFrameShadow(QFrame::Plain);
        label_06->setLineWidth(0);

        m_gridLayout->addWidget(label_06, 2, 0, 1, 1);

        label_00 = new MatrixLabel(layoutWidget1);
        label_00->setObjectName(QString::fromUtf8("label_00"));
        label_00->setMinimumSize(QSize(40, 40));
        label_00->setMaximumSize(QSize(40, 40));
        QFont font;
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setStrikeOut(false);
        font.setKerning(true);
        font.setStyleStrategy(QFont::PreferDefault);
        label_00->setFont(font);
        label_00->setFrameShape(QFrame::Panel);
        label_00->setFrameShadow(QFrame::Plain);
        label_00->setLineWidth(0);
        label_00->setMidLineWidth(0);
        label_00->setWordWrap(false);

        m_gridLayout->addWidget(label_00, 0, 0, 1, 1);

        label_03 = new MatrixLabel(layoutWidget1);
        label_03->setObjectName(QString::fromUtf8("label_03"));
        label_03->setMinimumSize(QSize(40, 40));
        label_03->setMaximumSize(QSize(40, 40));
        label_03->setFrameShape(QFrame::Panel);
        label_03->setFrameShadow(QFrame::Plain);
        label_03->setLineWidth(0);

        m_gridLayout->addWidget(label_03, 1, 0, 1, 1);

        label_02 = new MatrixLabel(layoutWidget1);
        label_02->setObjectName(QString::fromUtf8("label_02"));
        label_02->setMinimumSize(QSize(40, 40));
        label_02->setMaximumSize(QSize(40, 40));
        label_02->setFrameShape(QFrame::Panel);
        label_02->setLineWidth(0);

        m_gridLayout->addWidget(label_02, 0, 2, 1, 1);

        layoutWidget2 = new QWidget(page_1);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 180, 302, 27));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(150, 25));
        label->setMaximumSize(QSize(150, 25));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(true);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("#label{color:rgb(0, 85, 255)}"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font2;
        font2.setPointSize(12);
        font2.setItalic(true);
        label_3->setFont(font2);
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setStyleSheet(QString::fromUtf8("#label_3{color:rgb(255, 200, 92)}"));
        label_3->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);

        horizontalLayout_2->addWidget(label_3);

        stackedWidget->addWidget(page_1);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setMinimumSize(QSize(320, 240));
        page_3->setMaximumSize(QSize(320, 240));
        gridLayoutWidget = new QWidget(page_3);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(9, 19, 291, 191));
        gridLayout2 = new QGridLayout(gridLayoutWidget);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        gridLayout2->setContentsMargins(0, 0, 0, 0);
        label_09 = new QLabel(gridLayoutWidget);
        label_09->setObjectName(QString::fromUtf8("label_09"));

        gridLayout2->addWidget(label_09, 0, 0, 1, 1);

        label_10 = new QLabel(gridLayoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout2->addWidget(label_10, 0, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout2->addWidget(label_4, 0, 2, 1, 1);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        label_info = new QLabel(page_4);
        label_info->setObjectName(QString::fromUtf8("label_info"));
        label_info->setGeometry(QRect(0, 10, 301, 51));
        stackedWidget->addWidget(page_4);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "ROS2 humble", nullptr));
        label_battery->setText(QCoreApplication::translate("MainWindow", "\347\224\265\346\261\240 80%", nullptr));
        label_01->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\347\233\256\346\240\207\350\277\275\350\270\252.png\" /></p></body></html>", nullptr));
        label_08->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\345\233\276\345\203\217\345\210\206\345\211\262.png\" /></p></body></html>", nullptr));
        label_05->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\345\271\264\351\276\204\350\257\206\345\210\253.png\" /></p></body></html>", nullptr));
        label_07->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\350\257\255\351\237\263\350\257\206\345\210\253.png\" /></p></body></html>", nullptr));
        label_04->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\345\205\263\344\272\216\350\256\276\345\244\207.png\" /></p></body></html>", nullptr));
        label_06->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\351\232\224\347\251\272\344\275\234\347\224\273.png\" /></p></body></html>", nullptr));
        label_00->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\346\211\213\345\212\277\350\257\206\345\210\253.png\" /></p></body></html>", nullptr));
        label_03->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\344\272\272\350\204\270\346\243\200\346\265\213.png\" /></p></body></html>", nullptr));
        label_02->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\344\272\272\344\275\223\350\257\206\345\210\253.png\" /></p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "label", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "XGO20250327", nullptr));
        label_09->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; color:#ff7214;\">TextLabel</span></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_info->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; color:#ff7214;\">TextLabel3</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
