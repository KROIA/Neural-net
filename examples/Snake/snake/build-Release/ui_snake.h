/********************************************************************************
** Form generated from reading UI file 'snake.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAKE_H
#define UI_SNAKE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Snake
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Snake)
    {
        if (Snake->objectName().isEmpty())
            Snake->setObjectName(QStringLiteral("Snake"));
        Snake->resize(400, 300);
        menuBar = new QMenuBar(Snake);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        Snake->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Snake);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Snake->addToolBar(mainToolBar);
        centralWidget = new QWidget(Snake);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Snake->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Snake);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Snake->setStatusBar(statusBar);

        retranslateUi(Snake);

        QMetaObject::connectSlotsByName(Snake);
    } // setupUi

    void retranslateUi(QMainWindow *Snake)
    {
        Snake->setWindowTitle(QApplication::translate("Snake", "Snake", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Snake: public Ui_Snake {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
