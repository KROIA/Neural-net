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
        void closeEvent(QCloseEvent *event);


        void on_pause_pushButton_clicked();
        void on_saveStats_pushbutton_clicked();
        void on_kill_pushButton_clicked();
        void on_toggleDisplay_pushbutton_clicked();
        void on_versusBot_pushButton_clicked();

        void onSnakeKilled(unsigned int killer,unsigned int victim);

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

        vector<float> _averageScoreList_smoth;
        vector<float> _maxScoreList_smoth;
        vector<float> _minScoreList_smoth;
        vector<float> _foodScore_smoth;
        vector<float> _stepScore_smoth;

        vector<float> _averageScoreList_smoth_tmp;
        vector<float> _maxScoreList_smoth_tmp;
        vector<float> _minScoreList_smoth_tmp;
        vector<float> _foodScore_smoth_tmp;
        vector<float> _stepScore_smoth_tmp;

        unsigned int generation;
        float _averageScore_smoth;

        bool _pause;


        bool _selfControl;
        bool _enableDisplay;
        float _calculationPerSecond;
        unsigned int _calculationCounter;
        unsigned int _saveCounter;

//#ifdef TESTMODE
        bool _versusBot;
        Environment *_versusEnvironment;

        float        _botScore;
        unsigned int _botFood;
        unsigned int _botSteps;
        unsigned int _botDeaths;
        unsigned int _botKills;

        float        _playerScore;
        unsigned int _playerFood;
        unsigned int _playerSteps;
        unsigned int _playerDeaths;
        unsigned int _playerKills;
//#endif

};

#endif // SNAKE_H
