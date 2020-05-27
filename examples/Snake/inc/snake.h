#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include "environment.h"
#include <QTimer>
#include <QKeyEvent>

#include <geneticnet.h>
#include <backpropnet.h>
#include <config.h>
#include "netvisu.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QLineSeries>
#include <QPixmap>

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
        void loadBackpropTrainignsData(std::vector<std::vector<double> > &inputs,std::vector<std::vector<double>  > &outputs);
        void modeReset();
        Direction getDirectionFromData(std::vector<double> inputs);
        Direction getDirectionFromData(std::vector<double> inputs,Direction moveDirection);
        void logGenom(std::vector<double> genom);
        double getScore(Player *player);
        double getScore(Score score);
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

        void on_record_start_pushButton_clicked(bool checked);

        void on_record_clearBuffer_pushButton_clicked();

        void on_toggleDisplay_pushbutton_clicked(bool checked);

    private:
        QString GetLastErrorStr();
        void saveError(QString error);
        void savePeroformanceData();
        void takescreenshot();
        void savescreenshot();
        Ui::Snake *ui;
        Environment *_environment;
        QPainter   *_painter;
        QTimer     *_updateTimer;
        QTimer     *_updateTimer2;

        std::vector<QPoint> _fieldOfView;

        GeneticNet *net;
        NetVisu *visu;

        QString _statsFilename;
        std::vector<double> _snakeScore;
        std::vector<double> _averageScoreList;
        std::vector<double> _maxScoreList;
        std::vector<double> _minScoreList;
        std::vector<double> _foodScore;
        std::vector<double> _stepScore;

        std::vector<double> _averageScoreList_smoth;
        std::vector<double> _maxScoreList_smoth;
        std::vector<double> _minScoreList_smoth;
        std::vector<double> _foodScore_smoth;
        std::vector<double> _stepScore_smoth;

        std::vector<double> _averageScoreList_smoth_tmp;
        std::vector<double> _maxScoreList_smoth_tmp;
        std::vector<double> _minScoreList_smoth_tmp;
        std::vector<double> _foodScore_smoth_tmp;
        std::vector<double> _stepScore_smoth_tmp;

        unsigned int _step;
        unsigned int generation;
        double _averageScore_smoth;

        bool _pause;


        bool _selfControl;
        bool _enableDisplay;
        double _genPerSecond;
        unsigned int _genPerSecCounter;
        double _calcPerSecond;
        long _calcPerSecCounter;
        double        _averageCalcPerSec;        //Test case
        std::vector<double>   _averageCalcPerSec_List;
        std::vector<double>  _averageEnviromentCycleTime;
        std::vector<double>  _averageNetCycleTime;
        std::vector<double>  _averageSnakeCycleTime;
        unsigned int _saveCounter;
        QString _calcPerSecFileName;

//#ifdef TESTMODE
        int _modus;
        unsigned int _selectedSnake;
        unsigned int _respawnAmount;

        Environment *_versusEnvironment;
        std::vector<struct Score> _versusBotScore;
        std::vector<struct Score> _versusBotAverageScore;
        std::vector<struct Score> _versusBotAverageScore_tmpBuffer;
        std::vector<struct Score> _versusBotAbsolutAverageScore;
        std::vector<struct Score> _versusPlayerScore;
        std::vector<struct Score> _versusPlayerAverageScore;
        std::vector<struct Score> _versusPlayerAverageScore_tmpBuffer;
        std::vector<struct Score> _versusPlayerAbsolutAverageScore;
        QString _versusSaveFileName;
        unsigned int _versusSaveScoreInterval;
        unsigned int _versusSaveScoreCount;

        Environment *_backpropTrainingEnvironment;
        BackpropNet *_backpropNet;
        std::vector<std::vector<double>    >_backpropTrainingInputs;
        std::vector<std::vector<double>    >_backpropTrainingOutputs;
        QString       _backprobTrainingsDataFileName;

        double        _botScore;
        unsigned int _botFood;
        unsigned int _botSteps;
        unsigned int _botDeaths;
        unsigned int _botKills;

        double        _playerScore;
        unsigned int _playerFood;
        unsigned int _playerSteps;
        unsigned int _playerDeaths;
        unsigned int _playerKills;

        bool _playerKeyInputDisabler;


        //statistics
        unsigned int _maxChartSize;

        double _stats_maxAverageScore;
        double _stats_maxCalcPerSec;
        double _stats_maxGenPerSec;
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

        unsigned int _record_imageIndex;
        unsigned int _record_generationIndex;
        unsigned int _record_generationInterval;
        unsigned int _record_stepInterval;
        bool         _record_enable;
        bool         _record_displayAutoEnabler;
        QString      _record_savePath;
        std::vector<QPixmap>_record_imageList;

//#endif

};

#endif // SNAKE_H
