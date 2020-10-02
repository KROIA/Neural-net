#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define qmlVisu

#include <QMainWindow>
#include <backpropnet.h>
#include <QTimer>
#include "netvisu.h"
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QImage>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void timeout();
    void onError(unsigned int netID,Error &e);

    void on_pushButton_clicked();

    void on_mutation_slider_valueChanged(int value);

    void on_speed_slider_valueChanged(int value);

    void on_randomGenom_pushButton_clicked();

    void on_hiddenX_spinBox_valueChanged(int arg1);

    void on_hiddenY_spinBox_valueChanged(int arg1);


    void on_comboBox_currentIndexChanged(int index);
    void trainNet();
    private:

    void testNet();
    double mapD(double x, double in_min, double in_max, double out_min, double out_max);
    QColor getValueColor(double value);
    void setupNet();
    void setupTrainingSet();
    double qFunc(double x);

#ifdef qmlVisu
    NetVisu *visu;
#endif
    Ui::MainWindow *ui;
    BackpropNet *net;
    QTimer *timer;
    QTimer *learnTimer;
    QImage *trainImage;
    QImage *testImage;
    QImage *sollImage;
    QLabel *trainPicLabel;
    QLabel *testLabel;
    QLabel *sollLabel;

    unsigned int input;
    unsigned int hiddenX;
    unsigned int hiddenY;
    unsigned int output;
    Activation activation;

    std::vector<std::vector<double> >trainingsSet;
    std::vector<std::vector<double> >outputSet;
    std::vector<double>  outputList;
    unsigned int setPos;
    unsigned int sycles;
    double averageError;
    unsigned int displayInterval;

    bool learnMode;
    int learnSize;
    int testingSize;

};

#endif // MAINWINDOW_H
