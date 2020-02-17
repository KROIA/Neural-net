/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QSlider *speed_slider;
    QSlider *mutation_slider;
    QLabel *error_label;
    QSlider *zoom_slider;
    QLabel *error_label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QFrame *frame;
    QLabel *label_6;
    QSlider *color_slider;
    QSlider *colorOffset_slider;
    QLabel *label_7;
    QLabel *label_8;
    QFrame *frame_2;
    QComboBox *comboBox;
    QLabel *label_2;
    QPushButton *randomGenom_pushButton;
    QLabel *label;
    QPushButton *pushButton;
    QSpinBox *hiddenY_spinBox;
    QSpinBox *hiddenX_spinBox;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1217, 751);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        speed_slider = new QSlider(centralWidget);
        speed_slider->setObjectName(QString::fromUtf8("speed_slider"));
        speed_slider->setGeometry(QRect(10, 279, 16, 211));
        speed_slider->setMinimum(1);
        speed_slider->setMaximum(10000);
        speed_slider->setOrientation(Qt::Vertical);
        mutation_slider = new QSlider(centralWidget);
        mutation_slider->setObjectName(QString::fromUtf8("mutation_slider"));
        mutation_slider->setGeometry(QRect(30, 299, 16, 191));
        mutation_slider->setMinimum(1);
        mutation_slider->setMaximum(1000);
        mutation_slider->setOrientation(Qt::Vertical);
        error_label = new QLabel(centralWidget);
        error_label->setObjectName(QString::fromUtf8("error_label"));
        error_label->setGeometry(QRect(50, 10, 41, 16));
        zoom_slider = new QSlider(centralWidget);
        zoom_slider->setObjectName(QString::fromUtf8("zoom_slider"));
        zoom_slider->setGeometry(QRect(50, 319, 16, 171));
        zoom_slider->setMinimum(0);
        zoom_slider->setMaximum(100000);
        zoom_slider->setPageStep(1);
        zoom_slider->setValue(50000);
        zoom_slider->setSliderPosition(50000);
        zoom_slider->setOrientation(Qt::Vertical);
        error_label_2 = new QLabel(centralWidget);
        error_label_2->setObjectName(QString::fromUtf8("error_label_2"));
        error_label_2->setGeometry(QRect(10, 10, 41, 16));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 260, 47, 13));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 280, 47, 13));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 300, 47, 13));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 50, 91, 211));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 0, 47, 13));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        color_slider = new QSlider(frame);
        color_slider->setObjectName(QString::fromUtf8("color_slider"));
        color_slider->setGeometry(QRect(10, 40, 16, 160));
        color_slider->setMinimum(1);
        color_slider->setMaximum(1000);
        color_slider->setValue(1000);
        color_slider->setOrientation(Qt::Vertical);
        colorOffset_slider = new QSlider(frame);
        colorOffset_slider->setObjectName(QString::fromUtf8("colorOffset_slider"));
        colorOffset_slider->setGeometry(QRect(60, 40, 16, 160));
        colorOffset_slider->setMinimum(0);
        colorOffset_slider->setMaximum(4000);
        colorOffset_slider->setValue(2000);
        colorOffset_slider->setOrientation(Qt::Vertical);
        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 20, 47, 13));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(7);
        label_7->setFont(font);
        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 20, 47, 13));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);
        label_8->setFont(font);
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 500, 111, 181));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        comboBox = new QComboBox(frame_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 150, 72, 22));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 110, 47, 21));
        randomGenom_pushButton = new QPushButton(frame_2);
        randomGenom_pushButton->setObjectName(QString::fromUtf8("randomGenom_pushButton"));
        randomGenom_pushButton->setGeometry(QRect(10, 40, 80, 21));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 80, 47, 21));
        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 80, 21));
        hiddenY_spinBox = new QSpinBox(frame_2);
        hiddenY_spinBox->setObjectName(QString::fromUtf8("hiddenY_spinBox"));
        hiddenY_spinBox->setGeometry(QRect(10, 110, 43, 22));
        hiddenY_spinBox->setMaximum(100);
        hiddenX_spinBox = new QSpinBox(frame_2);
        hiddenX_spinBox->setObjectName(QString::fromUtf8("hiddenX_spinBox"));
        hiddenX_spinBox->setGeometry(QRect(10, 80, 43, 22));
        hiddenX_spinBox->setMaximum(100);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1217, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        error_label->setText(QCoreApplication::translate("MainWindow", "sad", nullptr));
        error_label_2->setText(QCoreApplication::translate("MainWindow", "Error", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Speed", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Mutation", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Zoom", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Color ", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Faktor", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Offset", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Gaussian", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Sigmoid", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Linear", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Relu", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "HiddenY", nullptr));
        randomGenom_pushButton->setText(QCoreApplication::translate("MainWindow", "rand", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "HiddenX", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "pause", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
