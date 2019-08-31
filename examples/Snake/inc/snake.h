#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include "environment.h"
#include <QTimer>
#include <QKeyEvent>

#include <geneticnet.h>
#include <backpropnet.h>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QLineSeries>

#include <ctime>
#include <ratio>
#include <chrono>

QT_CHARTS_USE_NAMESPACE

enum Modus{
    geneticTraining = 0,
    versusAI = 1,
    backpropTraining = 2
};

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
        void saveBackpropTrainingsData();
        void saveVersusData();
        void loadBackpropTrainignsData(vector<vector<float> > &inputs,vector<vector<float>  > &outputs);
        void modeReset();
        Direction getDirectionFromData(vector<float> inputs);
        void logGenom(vector<float> genom);
        float getScore(Player *player);
        float getScore(Score score);
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

        void onSnakeKilled(unsigned int killer,unsigned int victim);



        void on_mapinfo_checkbox_stateChanged(int arg1);
        void on_geneticTraining_radioButton_clicked(bool checked);
        void on_versusAI_radioButton_clicked(bool checked);
        void on_backpropTraining_radioButton_clicked(bool checked);

        void on_backpropTraining_pushButton_clicked();

        void on_selectedSnake_slider_valueChanged(int value);

    private:
        string GetLastErrorStdStr();
        void saveError(std::string error);
        void savePeroformanceData();
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
        float _genPerSecond;
        unsigned int _genPerSecCounter;
        float _calcPerSecond;
        long _calcPerSecCounter;
        float        _averageCalcPerSec;        //Test case
        std::vector<float>  _averageCalcPerSec_List;
        std::vector<double>  _averageEnviromentCycleTime;
        std::vector<double>  _averageNetCycleTime;
        std::vector<double>  _averageSnakeCycleTime;
        unsigned int _saveCounter;

//#ifdef TESTMODE
        int _modus;
        unsigned int _selectedSnake;
        unsigned int _respawnAmount;

        Environment *_versusEnvironment;
        vector<struct Score> _versusBotScore;
        vector<struct Score> _versusBotAverageScore;
        vector<struct Score> _versusBotAverageScore_tmpBuffer;
        vector<struct Score> _versusBotAbsolutAverageScore;
        vector<struct Score> _versusPlayerScore;
        vector<struct Score> _versusPlayerAverageScore;
        vector<struct Score> _versusPlayerAverageScore_tmpBuffer;
        vector<struct Score> _versusPlayerAbsolutAverageScore;
        string _versusSaveFileName;
        unsigned int _versusSaveScoreInterval;
        unsigned int _versusSaveScoreCount;

        Environment *_backpropTrainingEnvironment;
        BackpropNet *_backpropNet;
        vector<vector<float>    >_backpropTrainingInputs;
        vector<vector<float>    >_backpropTrainingOutputs;
        string       _backprobTrainingsDataFileName;

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

        bool _playerKeyInputDisabler;


        //statistics
        unsigned int _maxChartSize;

        float _stats_maxAverageScore;
        float _stats_maxCalcPerSec;
        float _stats_maxGenPerSec;
        QLineSeries *_stats_averageScore_LineSeries;
        QLineSeries *_stats_Score_LineSeries;
        QLineSeries *_stats_calcPerSec_Lineseries;
        QLineSeries *_stats_genPerSec_Lineseries;

        QChart  *_stats_score_Chart;
        QChart  *_stats_performanceChart;

        QChartView  *_stats_score_chartview;
        QChartView  *_stats_performance_chartview;

        unsigned int _fullSycleTimeDebugCount;
        double       _fullSycleTime;
        std::chrono::high_resolution_clock::time_point _fullSycleTimeStart;
        std::chrono::high_resolution_clock::time_point _fullSycleTimeEnd;
        std::chrono::duration<double> _fullSycleTime_span;

//#endif

};

#endif // SNAKE_H
