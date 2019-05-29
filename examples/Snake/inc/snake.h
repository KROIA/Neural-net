#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include "environment.h"
#include <QTimer>
#include <QKeyEvent>
#include <geneticnet.h>



namespace Ui {
    class Snake;
}

class Snake : public QMainWindow
{
        Q_OBJECT

    public:
        explicit Snake(QWidget *parent = nullptr);
        ~Snake();

        void handleNet();
        void setupFieldOfView();

    protected:
        void keyPressEvent(QKeyEvent *e);
        void keyReleaseEvent(QKeyEvent *e);

    public slots:
        void timerEvent();
        void timerEvent2();
    private slots:
        void paintEvent(QPaintEvent *e);


        void on_pause_pushButton_clicked();

        void on_saveStats_pushbutton_clicked();

        void on_kill_pushButton_clicked();

        void on_toggleDisplay_pushbutton_clicked();

    private:
        Ui::Snake *ui;
        Environment *_environment;
        QPainter   *_painter;
        QTimer     *_updateTimer;
        QTimer     *_updateTimer2;

        vector<QPoint> _fieldOfView;

        GeneticNet *net;

        string _statsFilename;
        vector<float> _snakeScore;
        vector<float> _averageScoreList;
        vector<float> _maxScoreList;
        vector<float> _minScoreList;
        vector<float> _foodScore;
        vector<float> _stepScore;

        unsigned int generation;

        bool _pause;


        bool _selfControl;
        bool _enableDisplay;
        float _calculationPerSecond;
        unsigned int _calculationCounter;


};

#endif // SNAKE_H
