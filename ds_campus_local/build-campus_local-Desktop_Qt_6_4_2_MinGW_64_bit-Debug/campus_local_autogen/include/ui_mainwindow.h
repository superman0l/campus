/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextEdit *rgs_id;
    QTextEdit *rgs_pswd;
    QPushButton *Register;
    QTextEdit *rgs_name;
    QTextEdit *rgs_class;
    QTextEdit *login_id;
    QTextEdit *login_pswd;
    QPushButton *login;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        rgs_id = new QTextEdit(centralwidget);
        rgs_id->setObjectName("rgs_id");
        rgs_id->setGeometry(QRect(70, 90, 131, 31));
        rgs_pswd = new QTextEdit(centralwidget);
        rgs_pswd->setObjectName("rgs_pswd");
        rgs_pswd->setGeometry(QRect(70, 140, 131, 31));
        Register = new QPushButton(centralwidget);
        Register->setObjectName("Register");
        Register->setGeometry(QRect(240, 120, 71, 21));
        rgs_name = new QTextEdit(centralwidget);
        rgs_name->setObjectName("rgs_name");
        rgs_name->setGeometry(QRect(70, 190, 131, 31));
        rgs_class = new QTextEdit(centralwidget);
        rgs_class->setObjectName("rgs_class");
        rgs_class->setGeometry(QRect(70, 240, 131, 31));
        login_id = new QTextEdit(centralwidget);
        login_id->setObjectName("login_id");
        login_id->setGeometry(QRect(380, 90, 111, 31));
        login_pswd = new QTextEdit(centralwidget);
        login_pswd->setObjectName("login_pswd");
        login_pswd->setGeometry(QRect(380, 150, 111, 31));
        login = new QPushButton(centralwidget);
        login->setObjectName("login");
        login->setGeometry(QRect(520, 120, 71, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Register->setText(QCoreApplication::translate("MainWindow", "Register", nullptr));
        login->setText(QCoreApplication::translate("MainWindow", "login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
