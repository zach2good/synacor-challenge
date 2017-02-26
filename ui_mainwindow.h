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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *labelPC;
    QListWidget *listMemory;
    QLabel *label;
    QLabel *label_2;
    QListWidget *listRegisters;
    QLabel *label_3;
    QLabel *labelCount;
    QLabel *label_4;
    QListWidget *listStack;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEdit;
    QTextBrowser *textBrowser;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1097, 623);
        MainWindow->setMinimumSize(QSize(2, 9));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        labelPC = new QLabel(centralWidget);
        labelPC->setObjectName(QStringLiteral("labelPC"));
        labelPC->setGeometry(QRect(20, 40, 55, 16));
        listMemory = new QListWidget(centralWidget);
        listMemory->setObjectName(QStringLiteral("listMemory"));
        listMemory->setGeometry(QRect(130, 40, 181, 561));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(130, 20, 55, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 20, 91, 16));
        listRegisters = new QListWidget(centralWidget);
        listRegisters->setObjectName(QStringLiteral("listRegisters"));
        listRegisters->setGeometry(QRect(320, 40, 111, 251));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(320, 20, 55, 16));
        labelCount = new QLabel(centralWidget);
        labelCount->setObjectName(QStringLiteral("labelCount"));
        labelCount->setGeometry(QRect(20, 90, 55, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 70, 101, 16));
        listStack = new QListWidget(centralWidget);
        listStack->setObjectName(QStringLiteral("listStack"));
        listStack->setGeometry(QRect(320, 320, 111, 281));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(330, 300, 55, 16));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(440, 20, 55, 16));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(440, 540, 55, 16));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(440, 570, 621, 31));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(440, 40, 621, 491));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 570, 93, 28));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 120, 93, 28));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 430, 101, 28));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(10, 460, 101, 28));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(10, 400, 101, 28));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(40, 190, 31, 41));
        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(10, 230, 31, 41));
        pushButton_8 = new QPushButton(centralWidget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(70, 230, 31, 41));
        pushButton_9 = new QPushButton(centralWidget);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(40, 270, 31, 41));
        pushButton_10 = new QPushButton(centralWidget);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(10, 340, 93, 28));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Synacor VM", Q_NULLPTR));
        labelPC->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Memory", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Prog. Counter", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Registers", Q_NULLPTR));
        labelCount->setText(QApplication::translate("MainWindow", "Count", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Inst. Counter", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Stack", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Output", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Input", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Step", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "Hack Teleporter", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "Autoplay 2", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "Autoplay 1", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MainWindow", "N", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("MainWindow", "W", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("MainWindow", "E", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("MainWindow", "S", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("MainWindow", "Dump Output", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
