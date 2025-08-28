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
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *prev_page_icon_page1;
    QLabel *label;
    QLabel *next_page_icon_page1;
    QWidget *page_3;
    QWidget *layoutWidget_page3_top;
    QHBoxLayout *horizontalLayout_page3;
    QLabel *label_2_page3;
    QSpacerItem *horizontalSpacer_page3;
    QWidget *layoutWidget_2;
    QGridLayout *m_gridLayout_2;
    MatrixLabel *label_09;
    MatrixLabel *label_10;
    MatrixLabel *label_11;
    MatrixLabel *label_12;
    MatrixLabel *label_13;
    MatrixLabel *label_14;
    MatrixLabel *label_15;
    MatrixLabel *label_16;
    MatrixLabel *label_17;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *prev_page_icon;
    QLabel *label_page3;
    QLabel *next_page_icon;
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
        layoutWidget->setGeometry(QRect(0, 24, 301, 156));
        m_gridLayout = new QGridLayout(layoutWidget);
        m_gridLayout->setObjectName(QString::fromUtf8("m_gridLayout"));
        m_gridLayout->setHorizontalSpacing(10);
        m_gridLayout->setVerticalSpacing(3);
        m_gridLayout->setContentsMargins(10, 2, 10, 0);
        label_01 = new MatrixLabel(layoutWidget);
        label_01->setObjectName(QString::fromUtf8("label_01"));
        label_01->setMinimumSize(QSize(50, 50));
        label_01->setMaximumSize(QSize(50, 50));
        label_01->setFrameShape(QFrame::Panel);
        label_01->setLineWidth(0);

        m_gridLayout->addWidget(label_01, 0, 1, 1, 1);

        label_08 = new MatrixLabel(layoutWidget);
        label_08->setObjectName(QString::fromUtf8("label_08"));
        label_08->setMinimumSize(QSize(50, 50));
        label_08->setMaximumSize(QSize(50, 50));
        label_08->setFrameShape(QFrame::Panel);
        label_08->setFrameShadow(QFrame::Plain);
        label_08->setLineWidth(0);

        m_gridLayout->addWidget(label_08, 2, 2, 1, 1);

        label_05 = new MatrixLabel(layoutWidget);
        label_05->setObjectName(QString::fromUtf8("label_05"));
        label_05->setMinimumSize(QSize(50, 50));
        label_05->setMaximumSize(QSize(50, 50));
        label_05->setFrameShape(QFrame::Panel);
        label_05->setFrameShadow(QFrame::Plain);
        label_05->setLineWidth(0);

        m_gridLayout->addWidget(label_05, 1, 2, 1, 1);

        label_07 = new MatrixLabel(layoutWidget);
        label_07->setObjectName(QString::fromUtf8("label_07"));
        label_07->setMinimumSize(QSize(50, 50));
        label_07->setMaximumSize(QSize(50, 50));
        label_07->setFrameShape(QFrame::Panel);
        label_07->setFrameShadow(QFrame::Plain);
        label_07->setLineWidth(0);

        m_gridLayout->addWidget(label_07, 2, 1, 1, 1);

        label_04 = new MatrixLabel(layoutWidget);
        label_04->setObjectName(QString::fromUtf8("label_04"));
        label_04->setMinimumSize(QSize(50, 50));
        label_04->setMaximumSize(QSize(50, 50));
        label_04->setFrameShape(QFrame::Panel);
        label_04->setFrameShadow(QFrame::Plain);
        label_04->setLineWidth(0);

        m_gridLayout->addWidget(label_04, 1, 1, 1, 1);

        label_06 = new MatrixLabel(layoutWidget);
        label_06->setObjectName(QString::fromUtf8("label_06"));
        label_06->setMinimumSize(QSize(50, 50));
        label_06->setMaximumSize(QSize(50, 50));
        label_06->setFrameShape(QFrame::Panel);
        label_06->setFrameShadow(QFrame::Plain);
        label_06->setLineWidth(0);

        m_gridLayout->addWidget(label_06, 2, 0, 1, 1);

        label_00 = new MatrixLabel(layoutWidget);
        label_00->setObjectName(QString::fromUtf8("label_00"));
        label_00->setMinimumSize(QSize(50, 50));
        label_00->setMaximumSize(QSize(50, 50));
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

        label_03 = new MatrixLabel(layoutWidget);
        label_03->setObjectName(QString::fromUtf8("label_03"));
        label_03->setMinimumSize(QSize(50, 50));
        label_03->setMaximumSize(QSize(50, 50));
        label_03->setFrameShape(QFrame::Panel);
        label_03->setFrameShadow(QFrame::Plain);
        label_03->setLineWidth(0);

        m_gridLayout->addWidget(label_03, 1, 0, 1, 1);

        label_02 = new MatrixLabel(layoutWidget);
        label_02->setObjectName(QString::fromUtf8("label_02"));
        label_02->setMinimumSize(QSize(50, 50));
        label_02->setMaximumSize(QSize(50, 50));
        label_02->setFrameShape(QFrame::Panel);
        label_02->setLineWidth(0);

        m_gridLayout->addWidget(label_02, 0, 2, 1, 1);

        layoutWidget1 = new QWidget(page_1);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 186, 302, 27));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        prev_page_icon_page1 = new QLabel(layoutWidget1);
        prev_page_icon_page1->setObjectName(QString::fromUtf8("prev_page_icon_page1"));
        prev_page_icon_page1->setMinimumSize(QSize(20, 20));
        prev_page_icon_page1->setMaximumSize(QSize(20, 20));
        prev_page_icon_page1->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        prev_page_icon_page1->setPixmap(QPixmap(QString::fromUtf8(":/icon/images/\344\270\212\344\270\200\351\241\265.png")));
        prev_page_icon_page1->setScaledContents(true);
        prev_page_icon_page1->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(prev_page_icon_page1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(12);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        next_page_icon_page1 = new QLabel(layoutWidget1);
        next_page_icon_page1->setObjectName(QString::fromUtf8("next_page_icon_page1"));
        next_page_icon_page1->setMinimumSize(QSize(20, 20));
        next_page_icon_page1->setMaximumSize(QSize(20, 20));
        next_page_icon_page1->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        next_page_icon_page1->setPixmap(QPixmap(QString::fromUtf8(":/icon/images/\344\270\213\344\270\200\351\241\265.png")));
        next_page_icon_page1->setScaledContents(true);
        next_page_icon_page1->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(next_page_icon_page1);

        stackedWidget->addWidget(page_1);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setMinimumSize(QSize(320, 240));
        page_3->setMaximumSize(QSize(320, 240));
        layoutWidget_page3_top = new QWidget(page_3);
        layoutWidget_page3_top->setObjectName(QString::fromUtf8("layoutWidget_page3_top"));
        layoutWidget_page3_top->setGeometry(QRect(0, 20, 302, 22));
        horizontalLayout_page3 = new QHBoxLayout(layoutWidget_page3_top);
        horizontalLayout_page3->setObjectName(QString::fromUtf8("horizontalLayout_page3"));
        horizontalLayout_page3->setContentsMargins(0, 0, 0, 0);
        label_2_page3 = new QLabel(layoutWidget_page3_top);
        label_2_page3->setObjectName(QString::fromUtf8("label_2_page3"));
        label_2_page3->setStyleSheet(QString::fromUtf8("#label_2_page3{color:rgb(85, 170, 0)}"));
        label_2_page3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_page3->addWidget(label_2_page3);

        horizontalSpacer_page3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_page3->addItem(horizontalSpacer_page3);

        layoutWidget_2 = new QWidget(page_3);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(0, 24, 301, 156));
        m_gridLayout_2 = new QGridLayout(layoutWidget_2);
        m_gridLayout_2->setObjectName(QString::fromUtf8("m_gridLayout_2"));
        m_gridLayout_2->setHorizontalSpacing(10);
        m_gridLayout_2->setVerticalSpacing(3);
        m_gridLayout_2->setContentsMargins(10, 2, 10, 0);
        label_09 = new MatrixLabel(layoutWidget_2);
        label_09->setObjectName(QString::fromUtf8("label_09"));
        label_09->setMinimumSize(QSize(50, 50));
        label_09->setMaximumSize(QSize(50, 50));
        label_09->setFrameShape(QFrame::Panel);
        label_09->setFrameShadow(QFrame::Plain);
        label_09->setLineWidth(0);

        m_gridLayout_2->addWidget(label_09, 0, 0, 1, 1);

        label_10 = new MatrixLabel(layoutWidget_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(50, 50));
        label_10->setMaximumSize(QSize(50, 50));
        label_10->setFrameShape(QFrame::Panel);
        label_10->setLineWidth(0);

        m_gridLayout_2->addWidget(label_10, 0, 1, 1, 1);

        label_11 = new MatrixLabel(layoutWidget_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(50, 50));
        label_11->setMaximumSize(QSize(50, 50));
        label_11->setFrameShape(QFrame::Panel);
        label_11->setLineWidth(0);

        m_gridLayout_2->addWidget(label_11, 0, 2, 1, 1);

        label_12 = new MatrixLabel(layoutWidget_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(50, 50));
        label_12->setMaximumSize(QSize(50, 50));
        label_12->setFrameShape(QFrame::Panel);
        label_12->setFrameShadow(QFrame::Plain);
        label_12->setLineWidth(0);

        m_gridLayout_2->addWidget(label_12, 1, 0, 1, 1);

        label_13 = new MatrixLabel(layoutWidget_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(50, 50));
        label_13->setMaximumSize(QSize(50, 50));
        label_13->setFrameShape(QFrame::Panel);
        label_13->setFrameShadow(QFrame::Plain);
        label_13->setLineWidth(0);

        m_gridLayout_2->addWidget(label_13, 1, 1, 1, 1);

        label_14 = new MatrixLabel(layoutWidget_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(50, 50));
        label_14->setMaximumSize(QSize(50, 50));
        label_14->setFrameShape(QFrame::Panel);
        label_14->setFrameShadow(QFrame::Plain);
        label_14->setLineWidth(0);

        m_gridLayout_2->addWidget(label_14, 1, 2, 1, 1);

        label_15 = new MatrixLabel(layoutWidget_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(50, 50));
        label_15->setMaximumSize(QSize(50, 50));
        label_15->setFrameShape(QFrame::Panel);
        label_15->setFrameShadow(QFrame::Plain);
        label_15->setLineWidth(0);

        m_gridLayout_2->addWidget(label_15, 2, 0, 1, 1);

        label_16 = new MatrixLabel(layoutWidget_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(50, 50));
        label_16->setMaximumSize(QSize(50, 50));
        label_16->setFrameShape(QFrame::Panel);
        label_16->setFrameShadow(QFrame::Plain);
        label_16->setLineWidth(0);

        m_gridLayout_2->addWidget(label_16, 2, 1, 1, 1);

        label_17 = new MatrixLabel(layoutWidget_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(50, 50));
        label_17->setMaximumSize(QSize(50, 50));
        label_17->setFrameShape(QFrame::Panel);
        label_17->setFrameShadow(QFrame::Plain);
        label_17->setLineWidth(0);

        m_gridLayout_2->addWidget(label_17, 2, 2, 1, 1);

        layoutWidget_3 = new QWidget(page_3);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(0, 186, 302, 27));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        prev_page_icon = new QLabel(layoutWidget_3);
        prev_page_icon->setObjectName(QString::fromUtf8("prev_page_icon"));
        prev_page_icon->setMinimumSize(QSize(20, 20));
        prev_page_icon->setMaximumSize(QSize(20, 20));
        prev_page_icon->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        prev_page_icon->setPixmap(QPixmap(QString::fromUtf8(":/icon/images/\344\270\212\344\270\200\351\241\265.png")));
        prev_page_icon->setScaledContents(true);
        prev_page_icon->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(prev_page_icon);

        label_page3 = new QLabel(layoutWidget_3);
        label_page3->setObjectName(QString::fromUtf8("label_page3"));
        label_page3->setFont(font1);
        label_page3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        label_page3->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_page3);

        next_page_icon = new QLabel(layoutWidget_3);
        next_page_icon->setObjectName(QString::fromUtf8("next_page_icon"));
        next_page_icon->setMinimumSize(QSize(20, 20));
        next_page_icon->setMaximumSize(QSize(20, 20));
        next_page_icon->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        next_page_icon->setPixmap(QPixmap(QString::fromUtf8(":/icon/images/\344\270\213\344\270\200\351\241\265.png")));
        next_page_icon->setScaledContents(true);
        next_page_icon->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(next_page_icon);

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
        prev_page_icon_page1->setText(QString());
        label->setText(QString());
        next_page_icon_page1->setText(QString());
        label_2_page3->setText(QString());
        label_09->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icon/images/\351\233\267\350\276\276\346\211\253\346\217\217.png\" /></p></body></html>", nullptr));
        label_10->setText(QString());
        label_11->setText(QString());
        label_12->setText(QString());
        label_13->setText(QString());
        label_14->setText(QString());
        label_15->setText(QString());
        label_16->setText(QString());
        label_17->setText(QString());
        prev_page_icon->setText(QString());
        label_page3->setText(QCoreApplication::translate("MainWindow", "\351\233\267\350\276\276\346\211\253\346\217\217", nullptr));
        next_page_icon->setText(QString());
        label_info->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; color:#ff7214;\">TextLabel3</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
