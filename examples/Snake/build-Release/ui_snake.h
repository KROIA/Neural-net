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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Snake
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *score_label;
    QLabel *label_2;
    QLabel *food_label;
    QLabel *food_label_2;
    QLabel *steps_label;
    QSlider *speed_slider;
    QLabel *label_3;
    QSlider *food_slider;
    QLabel *label_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Snake)
    {
        if (Snake->objectName().isEmpty())
            Snake->setObjectName(QStringLiteral("Snake"));
        Snake->resize(1056, 768);
        centralWidget = new QWidget(Snake);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(690, 10, 71, 21));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        score_label = new QLabel(centralWidget);
        score_label->setObjectName(QStringLiteral("score_label"));
        score_label->setGeometry(QRect(760, 10, 71, 21));
        score_label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(690, 40, 71, 21));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);
        food_label = new QLabel(centralWidget);
        food_label->setObjectName(QStringLiteral("food_label"));
        food_label->setGeometry(QRect(760, 40, 71, 21));
        food_label->setFont(font1);
        food_label_2 = new QLabel(centralWidget);
        food_label_2->setObjectName(QStringLiteral("food_label_2"));
        food_label_2->setGeometry(QRect(690, 60, 71, 21));
        food_label_2->setFont(font1);
        steps_label = new QLabel(centralWidget);
        steps_label->setObjectName(QStringLiteral("steps_label"));
        steps_label->setGeometry(QRect(760, 60, 71, 21));
        steps_label->setFont(font1);
        speed_slider = new QSlider(centralWidget);
        speed_slider->setObjectName(QStringLiteral("speed_slider"));
        speed_slider->setGeometry(QRect(960, 30, 22, 160));
        speed_slider->setMinimum(50);
        speed_slider->setMaximum(1000);
        speed_slider->setSingleStep(5);
        speed_slider->setValue(700);
        speed_slider->setOrientation(Qt::Vertical);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(960, 190, 31, 20));
        food_slider = new QSlider(centralWidget);
        food_slider->setObjectName(QStringLiteral("food_slider"));
        food_slider->setGeometry(QRect(1010, 30, 22, 160));
        food_slider->setMinimum(1);
        food_slider->setMaximum(30);
        food_slider->setSingleStep(1);
        food_slider->setValue(10);
        food_slider->setOrientation(Qt::Vertical);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(1010, 190, 31, 20));
        Snake->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Snake);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1056, 21));
        Snake->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Snake);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Snake->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Snake);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Snake->setStatusBar(statusBar);

        retranslateUi(Snake);

        QMetaObject::connectSlotsByName(Snake);
    } // setupUi

    void retranslateUi(QMainWindow *Snake)
    {
        Snake->setWindowTitle(QApplication::translate("Snake", "Snake", nullptr));
        label->setText(QApplication::translate("Snake", "Score", nullptr));
        score_label->setText(QApplication::translate("Snake", "Score", nullptr));
        label_2->setText(QApplication::translate("Snake", "Food", nullptr));
        food_label->setText(QApplication::translate("Snake", "Food", nullptr));
        food_label_2->setText(QApplication::translate("Snake", "Steps", nullptr));
        steps_label->setText(QApplication::translate("Snake", "Steps", nullptr));
        label_3->setText(QApplication::translate("Snake", "Speed", nullptr));
        label_4->setText(QApplication::translate("Snake", "Food", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Snake: public Ui_Snake {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
