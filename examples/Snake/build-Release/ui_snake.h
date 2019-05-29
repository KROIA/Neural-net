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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
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
    QPushButton *pause_pushButton;
    QPushButton *saveStats_pushbutton;
    QLabel *info_label;
    QPushButton *kill_pushButton;
    QPushButton *toggleDisplay_pushbutton;
    QLabel *food_label_3;
    QLabel *genPerSec_label;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Snake)
    {
        if (Snake->objectName().isEmpty())
            Snake->setObjectName(QStringLiteral("Snake"));
        Snake->resize(1279, 815);
        centralWidget = new QWidget(Snake);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 71, 21));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        score_label = new QLabel(centralWidget);
        score_label->setObjectName(QStringLiteral("score_label"));
        score_label->setGeometry(QRect(90, 40, 71, 21));
        score_label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 71, 21));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);
        food_label = new QLabel(centralWidget);
        food_label->setObjectName(QStringLiteral("food_label"));
        food_label->setGeometry(QRect(90, 70, 71, 21));
        food_label->setFont(font1);
        food_label_2 = new QLabel(centralWidget);
        food_label_2->setObjectName(QStringLiteral("food_label_2"));
        food_label_2->setGeometry(QRect(20, 90, 71, 21));
        food_label_2->setFont(font1);
        steps_label = new QLabel(centralWidget);
        steps_label->setObjectName(QStringLiteral("steps_label"));
        steps_label->setGeometry(QRect(90, 90, 71, 21));
        steps_label->setFont(font1);
        speed_slider = new QSlider(centralWidget);
        speed_slider->setObjectName(QStringLiteral("speed_slider"));
        speed_slider->setGeometry(QRect(30, 290, 22, 160));
        speed_slider->setMinimum(50);
        speed_slider->setMaximum(500);
        speed_slider->setSingleStep(5);
        speed_slider->setValue(400);
        speed_slider->setOrientation(Qt::Vertical);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 450, 31, 20));
        food_slider = new QSlider(centralWidget);
        food_slider->setObjectName(QStringLiteral("food_slider"));
        food_slider->setGeometry(QRect(80, 290, 22, 160));
        food_slider->setMinimum(1);
        food_slider->setMaximum(100);
        food_slider->setSingleStep(1);
        food_slider->setValue(50);
        food_slider->setOrientation(Qt::Vertical);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(80, 450, 31, 20));
        pause_pushButton = new QPushButton(centralWidget);
        pause_pushButton->setObjectName(QStringLiteral("pause_pushButton"));
        pause_pushButton->setGeometry(QRect(10, 160, 121, 21));
        saveStats_pushbutton = new QPushButton(centralWidget);
        saveStats_pushbutton->setObjectName(QStringLiteral("saveStats_pushbutton"));
        saveStats_pushbutton->setGeometry(QRect(10, 200, 121, 21));
        info_label = new QLabel(centralWidget);
        info_label->setObjectName(QStringLiteral("info_label"));
        info_label->setGeometry(QRect(10, 0, 431, 31));
        QFont font2;
        font2.setPointSize(7);
        info_label->setFont(font2);
        kill_pushButton = new QPushButton(centralWidget);
        kill_pushButton->setObjectName(QStringLiteral("kill_pushButton"));
        kill_pushButton->setGeometry(QRect(10, 180, 121, 21));
        toggleDisplay_pushbutton = new QPushButton(centralWidget);
        toggleDisplay_pushbutton->setObjectName(QStringLiteral("toggleDisplay_pushbutton"));
        toggleDisplay_pushbutton->setGeometry(QRect(10, 220, 121, 21));
        food_label_3 = new QLabel(centralWidget);
        food_label_3->setObjectName(QStringLiteral("food_label_3"));
        food_label_3->setGeometry(QRect(20, 110, 71, 21));
        QFont font3;
        font3.setPointSize(8);
        font3.setBold(false);
        font3.setWeight(50);
        food_label_3->setFont(font3);
        genPerSec_label = new QLabel(centralWidget);
        genPerSec_label->setObjectName(QStringLiteral("genPerSec_label"));
        genPerSec_label->setGeometry(QRect(90, 110, 71, 21));
        genPerSec_label->setFont(font3);
        Snake->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Snake);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1279, 21));
        Snake->setMenuBar(menuBar);
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
        pause_pushButton->setText(QApplication::translate("Snake", "Pause", nullptr));
        saveStats_pushbutton->setText(QApplication::translate("Snake", "save stats", nullptr));
        info_label->setText(QString());
        kill_pushButton->setText(QApplication::translate("Snake", "Kill", nullptr));
        toggleDisplay_pushbutton->setText(QApplication::translate("Snake", "display", nullptr));
        food_label_3->setText(QApplication::translate("Snake", "Gen per Sec.", nullptr));
        genPerSec_label->setText(QApplication::translate("Snake", "Gen per Sec.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Snake: public Ui_Snake {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
