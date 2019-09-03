/********************************************************************************
** Form generated from reading UI file 'snake.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAKE_H
#define UI_SNAKE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
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
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *botKills_label;
    QLabel *botFood_label;
    QLabel *botDeaths_label;
    QLabel *botSteps_label;
    QLabel *playerKills_label;
    QLabel *playerFood_label;
    QLabel *playerDeaths_label;
    QLabel *playerSteps_label;
    QLabel *label_11;
    QLabel *playerScore_label;
    QLabel *botScore_label;
    QCheckBox *mapinfo_checkbox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *geneticTraining_radioButton;
    QRadioButton *versusAI_radioButton;
    QRadioButton *backpropTraining_radioButton;
    QPushButton *backpropTraining_pushButton;
    QSlider *selectedSnake_slider;
    QLabel *label_12;
    QLabel *label_13;
    QSlider *maxError_slider;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Snake)
    {
        if (Snake->objectName().isEmpty())
            Snake->setObjectName(QString::fromUtf8("Snake"));
        Snake->resize(1279, 815);
        centralWidget = new QWidget(Snake);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 40, 71, 21));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        score_label = new QLabel(centralWidget);
        score_label->setObjectName(QString::fromUtf8("score_label"));
        score_label->setGeometry(QRect(90, 40, 71, 21));
        score_label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 71, 21));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);
        food_label = new QLabel(centralWidget);
        food_label->setObjectName(QString::fromUtf8("food_label"));
        food_label->setGeometry(QRect(90, 70, 71, 21));
        food_label->setFont(font1);
        food_label_2 = new QLabel(centralWidget);
        food_label_2->setObjectName(QString::fromUtf8("food_label_2"));
        food_label_2->setGeometry(QRect(20, 90, 71, 21));
        food_label_2->setFont(font1);
        steps_label = new QLabel(centralWidget);
        steps_label->setObjectName(QString::fromUtf8("steps_label"));
        steps_label->setGeometry(QRect(90, 90, 71, 21));
        steps_label->setFont(font1);
        speed_slider = new QSlider(centralWidget);
        speed_slider->setObjectName(QString::fromUtf8("speed_slider"));
        speed_slider->setGeometry(QRect(20, 290, 22, 160));
        speed_slider->setMinimum(350);
        speed_slider->setMaximum(500);
        speed_slider->setSingleStep(5);
        speed_slider->setValue(400);
        speed_slider->setOrientation(Qt::Vertical);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 450, 31, 20));
        food_slider = new QSlider(centralWidget);
        food_slider->setObjectName(QString::fromUtf8("food_slider"));
        food_slider->setGeometry(QRect(60, 290, 22, 160));
        food_slider->setMinimum(1);
        food_slider->setMaximum(100);
        food_slider->setSingleStep(1);
        food_slider->setValue(50);
        food_slider->setOrientation(Qt::Vertical);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 450, 31, 20));
        pause_pushButton = new QPushButton(centralWidget);
        pause_pushButton->setObjectName(QString::fromUtf8("pause_pushButton"));
        pause_pushButton->setGeometry(QRect(10, 160, 121, 21));
        saveStats_pushbutton = new QPushButton(centralWidget);
        saveStats_pushbutton->setObjectName(QString::fromUtf8("saveStats_pushbutton"));
        saveStats_pushbutton->setGeometry(QRect(10, 200, 121, 21));
        info_label = new QLabel(centralWidget);
        info_label->setObjectName(QString::fromUtf8("info_label"));
        info_label->setGeometry(QRect(10, 0, 431, 31));
        QFont font2;
        font2.setPointSize(7);
        info_label->setFont(font2);
        kill_pushButton = new QPushButton(centralWidget);
        kill_pushButton->setObjectName(QString::fromUtf8("kill_pushButton"));
        kill_pushButton->setGeometry(QRect(10, 180, 121, 21));
        toggleDisplay_pushbutton = new QPushButton(centralWidget);
        toggleDisplay_pushbutton->setObjectName(QString::fromUtf8("toggleDisplay_pushbutton"));
        toggleDisplay_pushbutton->setGeometry(QRect(10, 220, 121, 21));
        food_label_3 = new QLabel(centralWidget);
        food_label_3->setObjectName(QString::fromUtf8("food_label_3"));
        food_label_3->setGeometry(QRect(20, 110, 71, 21));
        QFont font3;
        font3.setPointSize(8);
        font3.setBold(false);
        font3.setWeight(50);
        food_label_3->setFont(font3);
        genPerSec_label = new QLabel(centralWidget);
        genPerSec_label->setObjectName(QString::fromUtf8("genPerSec_label"));
        genPerSec_label->setGeometry(QRect(90, 110, 71, 21));
        genPerSec_label->setFont(font3);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(60, 540, 47, 20));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(120, 540, 47, 20));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 580, 41, 20));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 600, 41, 20));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 640, 41, 20));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 620, 41, 20));
        botKills_label = new QLabel(centralWidget);
        botKills_label->setObjectName(QString::fromUtf8("botKills_label"));
        botKills_label->setGeometry(QRect(60, 620, 41, 20));
        botFood_label = new QLabel(centralWidget);
        botFood_label->setObjectName(QString::fromUtf8("botFood_label"));
        botFood_label->setGeometry(QRect(60, 580, 41, 20));
        botDeaths_label = new QLabel(centralWidget);
        botDeaths_label->setObjectName(QString::fromUtf8("botDeaths_label"));
        botDeaths_label->setGeometry(QRect(60, 640, 41, 20));
        botSteps_label = new QLabel(centralWidget);
        botSteps_label->setObjectName(QString::fromUtf8("botSteps_label"));
        botSteps_label->setGeometry(QRect(60, 600, 41, 20));
        playerKills_label = new QLabel(centralWidget);
        playerKills_label->setObjectName(QString::fromUtf8("playerKills_label"));
        playerKills_label->setGeometry(QRect(120, 620, 41, 20));
        playerFood_label = new QLabel(centralWidget);
        playerFood_label->setObjectName(QString::fromUtf8("playerFood_label"));
        playerFood_label->setGeometry(QRect(120, 580, 41, 20));
        playerDeaths_label = new QLabel(centralWidget);
        playerDeaths_label->setObjectName(QString::fromUtf8("playerDeaths_label"));
        playerDeaths_label->setGeometry(QRect(120, 640, 41, 20));
        playerSteps_label = new QLabel(centralWidget);
        playerSteps_label->setObjectName(QString::fromUtf8("playerSteps_label"));
        playerSteps_label->setGeometry(QRect(120, 600, 41, 20));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 560, 41, 20));
        playerScore_label = new QLabel(centralWidget);
        playerScore_label->setObjectName(QString::fromUtf8("playerScore_label"));
        playerScore_label->setGeometry(QRect(120, 560, 41, 20));
        botScore_label = new QLabel(centralWidget);
        botScore_label->setObjectName(QString::fromUtf8("botScore_label"));
        botScore_label->setGeometry(QRect(60, 560, 41, 20));
        mapinfo_checkbox = new QCheckBox(centralWidget);
        mapinfo_checkbox->setObjectName(QString::fromUtf8("mapinfo_checkbox"));
        mapinfo_checkbox->setGeometry(QRect(10, 240, 70, 17));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 670, 141, 80));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        geneticTraining_radioButton = new QRadioButton(verticalLayoutWidget);
        geneticTraining_radioButton->setObjectName(QString::fromUtf8("geneticTraining_radioButton"));
        geneticTraining_radioButton->setChecked(true);

        verticalLayout->addWidget(geneticTraining_radioButton);

        versusAI_radioButton = new QRadioButton(verticalLayoutWidget);
        versusAI_radioButton->setObjectName(QString::fromUtf8("versusAI_radioButton"));

        verticalLayout->addWidget(versusAI_radioButton);

        backpropTraining_radioButton = new QRadioButton(verticalLayoutWidget);
        backpropTraining_radioButton->setObjectName(QString::fromUtf8("backpropTraining_radioButton"));

        verticalLayout->addWidget(backpropTraining_radioButton);

        backpropTraining_pushButton = new QPushButton(centralWidget);
        backpropTraining_pushButton->setObjectName(QString::fromUtf8("backpropTraining_pushButton"));
        backpropTraining_pushButton->setGeometry(QRect(10, 750, 121, 21));
        selectedSnake_slider = new QSlider(centralWidget);
        selectedSnake_slider->setObjectName(QString::fromUtf8("selectedSnake_slider"));
        selectedSnake_slider->setGeometry(QRect(100, 290, 22, 160));
        selectedSnake_slider->setMinimum(1);
        selectedSnake_slider->setMaximum(100);
        selectedSnake_slider->setSingleStep(1);
        selectedSnake_slider->setPageStep(1);
        selectedSnake_slider->setValue(1);
        selectedSnake_slider->setOrientation(Qt::Vertical);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(100, 450, 31, 20));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(150, 750, 41, 20));
        maxError_slider = new QSlider(centralWidget);
        maxError_slider->setObjectName(QString::fromUtf8("maxError_slider"));
        maxError_slider->setGeometry(QRect(160, 639, 22, 111));
        maxError_slider->setMinimum(1);
        maxError_slider->setMaximum(1000);
        maxError_slider->setSingleStep(1);
        maxError_slider->setPageStep(1);
        maxError_slider->setValue(1000);
        maxError_slider->setOrientation(Qt::Vertical);
        Snake->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Snake);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1279, 21));
        Snake->setMenuBar(menuBar);
        statusBar = new QStatusBar(Snake);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Snake->setStatusBar(statusBar);

        retranslateUi(Snake);

        QMetaObject::connectSlotsByName(Snake);
    } // setupUi

    void retranslateUi(QMainWindow *Snake)
    {
        Snake->setWindowTitle(QCoreApplication::translate("Snake", "Snake", nullptr));
        label->setText(QCoreApplication::translate("Snake", "Score", nullptr));
        score_label->setText(QCoreApplication::translate("Snake", "Score", nullptr));
        label_2->setText(QCoreApplication::translate("Snake", "Food", nullptr));
        food_label->setText(QCoreApplication::translate("Snake", "Food", nullptr));
        food_label_2->setText(QCoreApplication::translate("Snake", "Steps", nullptr));
        steps_label->setText(QCoreApplication::translate("Snake", "Steps", nullptr));
        label_3->setText(QCoreApplication::translate("Snake", "Speed", nullptr));
        label_4->setText(QCoreApplication::translate("Snake", "Food", nullptr));
        pause_pushButton->setText(QCoreApplication::translate("Snake", "Pause", nullptr));
        saveStats_pushbutton->setText(QCoreApplication::translate("Snake", "save stats", nullptr));
        info_label->setText(QString());
        kill_pushButton->setText(QCoreApplication::translate("Snake", "Kill", nullptr));
        toggleDisplay_pushbutton->setText(QCoreApplication::translate("Snake", "display", nullptr));
        food_label_3->setText(QCoreApplication::translate("Snake", "Gen per Sec.", nullptr));
        genPerSec_label->setText(QCoreApplication::translate("Snake", "Gen per Sec.", nullptr));
        label_5->setText(QCoreApplication::translate("Snake", "Bot", nullptr));
        label_6->setText(QCoreApplication::translate("Snake", "Player", nullptr));
        label_7->setText(QCoreApplication::translate("Snake", "food", nullptr));
        label_8->setText(QCoreApplication::translate("Snake", "steps", nullptr));
        label_9->setText(QCoreApplication::translate("Snake", "deaths", nullptr));
        label_10->setText(QCoreApplication::translate("Snake", "kills", nullptr));
        botKills_label->setText(QCoreApplication::translate("Snake", "kills", nullptr));
        botFood_label->setText(QCoreApplication::translate("Snake", "food", nullptr));
        botDeaths_label->setText(QCoreApplication::translate("Snake", "deaths", nullptr));
        botSteps_label->setText(QCoreApplication::translate("Snake", "steps", nullptr));
        playerKills_label->setText(QCoreApplication::translate("Snake", "kills", nullptr));
        playerFood_label->setText(QCoreApplication::translate("Snake", "food", nullptr));
        playerDeaths_label->setText(QCoreApplication::translate("Snake", "deaths", nullptr));
        playerSteps_label->setText(QCoreApplication::translate("Snake", "steps", nullptr));
        label_11->setText(QCoreApplication::translate("Snake", "score", nullptr));
        playerScore_label->setText(QCoreApplication::translate("Snake", "score", nullptr));
        botScore_label->setText(QCoreApplication::translate("Snake", "score", nullptr));
        mapinfo_checkbox->setText(QCoreApplication::translate("Snake", "Mapinfo", nullptr));
        geneticTraining_radioButton->setText(QCoreApplication::translate("Snake", "geneticTraining", nullptr));
        versusAI_radioButton->setText(QCoreApplication::translate("Snake", "versus AI", nullptr));
        backpropTraining_radioButton->setText(QCoreApplication::translate("Snake", "backprop training", nullptr));
        backpropTraining_pushButton->setText(QCoreApplication::translate("Snake", "training", nullptr));
        label_12->setText(QCoreApplication::translate("Snake", "Snake", nullptr));
        label_13->setText(QCoreApplication::translate("Snake", "maxErr", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Snake: public Ui_Snake {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
