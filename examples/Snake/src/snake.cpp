#include "snake.h"
#include "ui_snake.h"
#include <QDebug>

//#define GLOBALVIEW
//#define DEBUG_PRINTS


Snake::Snake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);

    QString version = "01.01.02";
    QString datum   = "29.05.2020";

    char cwd[MAX_PATH+1];
    _getcwd(cwd,MAX_PATH);
    qDebug("pfad %s", cwd);

    QString infoLabelText  = QString(cwd) + "\n";
            infoLabelText += "Snake AI © by Alex Krieg\n";
            infoLabelText += "               v"+version+"\n";
            infoLabelText += "               "+datum;
    ui->info_label->setText(infoLabelText);




    // setupFieldOfView();



    // comfigParam
    bool enableKillreward   = false;
    unsigned int hiddenX    = 1;
    unsigned int hiddenY    = 2;
    unsigned int animals    = 100;
    _respawnAmount          = 3;
    int mapSizeX            = 100;
    int mapSizeY            = 100;
    QString netFileName     = "snake";
    double net_mutationFactor = 0.05;
    double net_mutationChangeWeight = 0.1;
    unsigned int enviromentTileSize = 6;
    unsigned int enviromentTileSpace = 0;

    unsigned int versusEnviromentTileSize = 20;
    unsigned int versusEnviromentTileSpace = 2;

    //end comfigParam
    Config config("snakeConfig.txt");
    config.read();
    config.parameterUInt("net_hiddenX",hiddenX,"rw");
    config.parameterUInt("net_hiddenY",hiddenY,"rw");
    config.parameterDouble("net_mutationFactor",net_mutationFactor,"rw");
    config.parameterDouble("net_mutationChangeWeight",net_mutationChangeWeight,"rw");
    config.parameterUInt("animals",animals,"rw");

    config.parameterUInt("respawnAmountPerRound",_respawnAmount,"rw");
    config.parameterBool("enableKillreward",enableKillreward,"rw");

    config.parameterInt("mapsize_X",mapSizeX,"rw");
    config.parameterInt("mapsize_Y",mapSizeY,"rw");
    config.parameterUInt("enviromentTileSize",enviromentTileSize,"rw");
    config.parameterUInt("enviromentTileSpace",enviromentTileSpace,"rw");
    config.parameterUInt("versusEnviromentTileSize",versusEnviromentTileSize,"rw");
    config.parameterUInt("versusEnviromentTileSpace",versusEnviromentTileSpace,"rw");

   // QString valueGrabber = "stats.csv";
    //config.parameter("stats_Filename",valueGrabber,"rw"); _statsFilename = valueGrabber.toStdString();
   // valueGrabber = "versusSave.csv";
   // config.parameter("versusStats_Filename",valueGrabber,"rw"); _statsFilename = valueGrabber.toStdString();
    config.parameter("net_Filename",netFileName,"rw");
    config.save();

    _statsFilename      = netFileName+"_stats.csv";
    _versusSaveFileName = netFileName+"_versusStats.csv";
    _calcPerSecFileName = netFileName+"_calcPerSec.csv";

    //config.parameter()

    _versusSaveScoreInterval = 2;
    QSize mapsize{mapSizeX,mapSizeY};
    //----------------NEURAL NET------------------


#ifdef GLOBALVIEW
    unsigned int inputs = /*_fieldOfView.size()*/4 * (mapsize.width() * mapsize.height()) + 4; //3 Sensor Layer (food,snake,obsticle)
#else
    unsigned int inputs = /*_fieldOfView.size()*/3 * (3 + 2); //3 Sensor Layer (food,snake,obsticle)
#endif

    unsigned int outputs = 3;
    net = new GeneticNet(animals,inputs,hiddenX,hiddenY,outputs,true,false,Activation::Gaussian);
    net->set_bias(true);
    net->loadFromNetFile(netFileName,"net");
    double gen = 0;
    try {
        net->get_ptr_saveNet()->get_ExtraParam("generation",gen);
    } catch (...) {

    }

    generation = (unsigned int) gen;
    net->set_mutationFactor(net_mutationFactor);
    net->set_mutationChangeWeight(net_mutationChangeWeight);

    _backpropNet = new BackpropNet(animals+10,
                                   net->get_inputNeurons(),
                                   net->get_hiddenNeuronsX(),
                                   net->get_hiddenNeuronsY(),
                                   net->get_outputNeurons(),
                                   net->get_bias(),
                                   net->get_enableAverage(),
                                   net->get_activationFunction(),
                                   this);
    _backpropNet->set_netFileName("backPropNet");
    _backpropNet->set_netFileEnding("net");
    ui->selectedSnake_slider->setRange(0,net->get_animals()-1);
    _selectedSnake = ui->selectedSnake_slider->value();

    net->updateNetConfiguration();

    _backpropNet->updateNetConfiguration();

    //-------additional connections
 /*   try{
    net->connectNeuronViaID(0,0,false);
    net->connectNeuronViaID(20,1,false);
    net->connectNeuronViaID(21,2,false);
    net->connectNeuronViaID(0,19,false);
    net->connectNeuronViaID(19,0,false);
        NeuronID newNeuronID;
        if(net->costumNeurons() == 0)
        {
            newNeuronID = net->addNeuron();
            //std::cout << Neuron::neuronIDString(newNeuronID);
            qDebug() << QString::fromStdString(Neuron::neuronIDString(newNeuronID));
            net->connectNeuronViaID(newNeuronID.ID,newNeuronID.ID,ConnectionDirection::backward);
            net->connectNeuronViaID(0,newNeuronID.ID,ConnectionDirection::forward);
            net->connectNeuronViaID(newNeuronID.ID,net->outputNeuron(0,0)->ID().ID,ConnectionDirection::forward);
        }
    net->update_ptr_genomList();
    }catch(std::runtime_error &e)
    {
        qDebug() << "can't connect: "<< e.what();
        saveError(QString("can't connect: ")+e.what());
    }*/
    //net->saveToNetFile();



    _painter = new QPainter(this);
//#ifdef TESTMODE
    _versusEnvironment = new Environment(this,_painter,2);
    _versusEnvironment->mapsize(QSize(30,30));
    _versusEnvironment->scale(1);
    _versusEnvironment->tileSize(versusEnviromentTileSize);
    _versusEnvironment->tileSpace(versusEnviromentTileSpace);
    _versusEnvironment->drawPos(QPoint(420,15));

    _versusEnvironment->player(0)->globalView(true);
    _versusEnvironment->player(0)->standardColor(QColor(0,100,200));
    _versusEnvironment->player(1)->standardColor(QColor(0,255,100));
    connect(_versusEnvironment,SIGNAL(playerKill(unsigned int,unsigned int)),this,SLOT(onSnakeKilled(unsigned int, unsigned int)));


    _botScore = 0;
    _botFood = 0;
    _botSteps = 0;
    _botDeaths = 0;
    _botKills = 0;

    _playerScore = 0;
    _playerFood = 0;
    _playerSteps = 0;
    _playerDeaths = 0;
    _playerKills = 0;


    _versusSaveScoreCount = 0;
//#else
    _environment = new Environment(this,_painter,net->get_animals());
    _environment->mapsize(mapsize);
    _environment->scale(1);
    _environment->tileSize(enviromentTileSize);
    _environment->tileSpace(enviromentTileSpace);

    _environment->drawPos(_versusEnvironment->drawPos());

    _backpropTrainingEnvironment = new Environment(this,_painter,1);
    _backpropTrainingEnvironment->mapsize(QSize(30,30));
    _backpropTrainingEnvironment->tileSize(10);
    _backpropTrainingEnvironment->tileSpace(2);
    _backpropTrainingEnvironment->scale(3);
    _backpropTrainingEnvironment->drawPos(_environment->drawPos());

    _backprobTrainingsDataFileName = "backpropNetTrainigData.txt";
//#endif


    for(unsigned int a=0; a<_environment->player()->size(); a++)
    {
        _environment->player(a)->killreward(enableKillreward);
    }
    for(unsigned int a=0; a<_versusEnvironment->player()->size(); a++)
    {
        _versusEnvironment->player(a)->killreward(enableKillreward);
    }
    for(unsigned int a=0; a<_backpropTrainingEnvironment->player()->size(); a++)
    {
        _backpropTrainingEnvironment->player(a)->killreward(enableKillreward);
    }


    _environment->mapInit();
    _versusEnvironment->mapInit();
    _backpropTrainingEnvironment->mapInit();


    _selfControl = false;
    _pause = true;
    _enableDisplay = true;

    _updateTimer = new QTimer(this);
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    if(!_pause)
     _updateTimer->start(200);

    _updateTimer2 = new QTimer(this);
    connect(_updateTimer2,SIGNAL(timeout()),this,SLOT(timerEvent2()));
    _updateTimer2->start(1000);
    _genPerSecond   = 0;
    _genPerSecCounter     = 0;
    _calcPerSecond = 0;
    _calcPerSecCounter = 0;
    _playerKeyInputDisabler = false;
    _modus = Modus::geneticTraining;

    //stats
    _maxChartSize  = 100;
    _stats_maxAverageScore = 1;
    _stats_maxCalcPerSec = 1;
    _stats_maxGenPerSec = 1;
    _stats_averageScore_LineSeries = new QLineSeries();
    _stats_Score_LineSeries        = new QLineSeries();
    _stats_calcPerSec_Lineseries   = new QLineSeries();
    _stats_genPerSec_Lineseries    = new QLineSeries();

    _stats_score_Chart             = new QChart();
    _stats_performanceChart        = new QChart();

    _stats_score_chartview         = new QChartView(_stats_score_Chart);
    _stats_performance_chartview   = new QChartView(_stats_performanceChart);

    _stats_averageScore_LineSeries->setColor(QColor(0,50,255));
    _stats_Score_LineSeries->setColor(QColor(200,100,0));
    _stats_calcPerSec_Lineseries->setColor(QColor(0,200,80));
    _stats_genPerSec_Lineseries->setColor(QColor(200,30,30));

    _stats_Score_LineSeries->setName("score");
    _stats_averageScore_LineSeries->setName("average Score");
    _stats_calcPerSec_Lineseries->setName("Calculations/Second");
    _stats_genPerSec_Lineseries->setName("Generations/Second");


    _stats_averageScore_LineSeries->append(0,0);
    _stats_Score_LineSeries->append(0,0);
    _stats_calcPerSec_Lineseries->append(0,0);
    _stats_genPerSec_Lineseries->append(0,0);



    //_stats_score_Chart->legend()->hide();
    _stats_score_Chart->addSeries(_stats_averageScore_LineSeries);
    _stats_score_Chart->addSeries(_stats_Score_LineSeries);
    _stats_score_Chart->createDefaultAxes();
    _stats_score_Chart->setTitle("Score");


    //_stats_performanceChart->legend()->hide();
    _stats_performanceChart->addSeries(_stats_calcPerSec_Lineseries);
    _stats_performanceChart->addSeries(_stats_genPerSec_Lineseries);
    _stats_performanceChart->createDefaultAxes();
    _stats_performanceChart->setTitle("Performance");



    _stats_score_chartview->setRenderHint(QPainter::Antialiasing);
    _stats_performance_chartview->setRenderHint(QPainter::Antialiasing);




    _stats_performance_chartview->setParent(this);
    _stats_performance_chartview->setGeometry(10,480,400,200);
    _stats_performance_chartview->show();


    _stats_score_chartview->setParent(this);
    _stats_score_chartview->setGeometry(10,680,400,200);
    _stats_score_chartview->show();

    _stats_score_Chart->axisX()->setRange(0,_maxChartSize);
    _stats_performanceChart->axisX()->setRange(0,_maxChartSize);

    _fullSycleTimeDebugCount = 0;
    _fullSycleTime           = 0;
    _averageCalcPerSec       = 0;
    _averageScore_smoth      = 0;

    _step = 0;

    _record_enable = false;
    _record_displayAutoEnabler = false;
    _record_imageIndex = 0;
    _record_generationInterval = ui->record_generationInterval_spinbox->value();
    _record_stepInterval = ui->record_stepInterval_spinbox->value();
    _record_generationIndex = generation;
    _record_imageList.reserve(1000);

    visu = new NetVisu(net->get_netList_ptr());

    visu->loadNetInUi(ui->net_widget);
    visu->showWindow();

    qDebug() << "Setup done";
}


Snake::~Snake()
{
    delete _stats_averageScore_LineSeries;
    delete _stats_Score_LineSeries;
    delete _stats_calcPerSec_Lineseries;
    delete _stats_genPerSec_Lineseries;
    delete _stats_score_Chart;
    delete _stats_performanceChart;
    delete _stats_score_chartview;
    delete _stats_performance_chartview;
    delete ui;
}
void Snake::paintEvent(QPaintEvent *e)
{


    //_stats_performanceChart->removeSeries(_stats_calcPerSec_Lineseries);
    //_stats_performanceChart->removeSeries(_stats_genPerSec_Lineseries);
    //_stats_score_Chart->removeSeries(_stats_averageScore_LineSeries);

    //_stats_performanceChart->addSeries(_stats_calcPerSec_Lineseries);
    //_stats_performanceChart->addSeries(_stats_genPerSec_Lineseries);
    //_stats_score_Chart->addSeries(_stats_averageScore_LineSeries);
   // _stats_score_Chart->update();
   // _stats_performanceChart->update();



   // _stats_score_Chart->axisX()->setMax(_stats_averageScore_LineSeries->count()-1);
   // _stats_performanceChart->axisX()->setMax(_stats_genPerSec_Lineseries->count()-1);
  //  _stats_score_Chart->axisX()->setMin(0);
 //   _stats_performanceChart->axisX()->setMin(0);
    if(!_enableDisplay)
        return;
    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);
    _painter->setRenderHints(QPainter::RenderHints(0x04),true);

    switch(_modus)
    {
        case Modus::versusAI:
        {
            _versusEnvironment->drawMap();
            break;
        }
        case Modus::geneticTraining:
        {
            _environment->drawMap();
            break;
        }
        case Modus::backpropTraining:
        {
            _backpropTrainingEnvironment->drawMap();
            break;
        }
    }
    _painter->end();
}
void Snake::timerEvent()
{
    //----------------SPEEDCONTROLL
    try{
        _fullSycleTimeEnd = std::chrono::high_resolution_clock::now();
        _fullSycleTime_span = std::chrono::duration_cast<std::chrono::microseconds>(_fullSycleTimeEnd - _fullSycleTimeStart);
        _fullSycleTimeStart = _fullSycleTimeEnd;
        _fullSycleTime = 0.5*_fullSycleTime + 0.5*_fullSycleTime_span.count();
        if(_fullSycleTimeDebugCount > 1000)
        {
#ifdef DEBUG_PRINTS
            qDebug() << "sycletime: " <<_fullSycleTime;
#endif
            _fullSycleTimeDebugCount = 0;
        }
       /* if(_fullSycleTime_span.count() > 100)
        {
            _fullSycleTime = 0;
        }*/
        _fullSycleTimeDebugCount++;
    }catch(std::runtime_error &e)
    {
        qDebug() << "ERROR  sycletime "<< e.what();
        saveError("ERROR  sycletime "+QString(e.what()));
    }catch(...)
    {
        qDebug() << "ERROR  sycletime -> unnkown "<<GetLastErrorStr();
        saveError("ERROR  sycletime -> unnkown "+GetLastErrorStr());
    }
    //------------------------
    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            try{
                _environment->update();
            }catch(std::runtime_error &e)
            {
                qDebug() << "ERROR  _environment->update() "<< e.what();
                saveError("ERROR  _environment->update() "+QString(e.what()));
            }catch(...)
            {
                qDebug() << "ERROR  _environment->update() -> unnkown "<<GetLastErrorStr();
                saveError("ERROR  _environment->update() -> unnkown "+GetLastErrorStr());
            }
            break;
        }
        case Modus::versusAI:
        {
            _versusEnvironment->update();

         /*   ui->playerFood_label->setText(QString::number(_playerFood+_versusEnvironment->player(0)->food()));
            ui->playerSteps_label->setText(QString::number(_playerSteps+_versusEnvironment->player(0)->steps()));
            ui->playerDeaths_label->setText(QString::number(_playerDeaths));
            ui->playerKills_label->setText(QString::number(_playerKills));
            ui->playerScore_label->setText(QString::number(_playerScore));

            ui->botFood_label->setText(QString::number(_botFood+_versusEnvironment->player(1)->food()));
            ui->botSteps_label->setText(QString::number(_botSteps+_versusEnvironment->player(1)->steps()));
            ui->botDeaths_label->setText(QString::number(_botDeaths));
            ui->botKills_label->setText(QString::number(_botKills));
            ui->botScore_label->setText(QString::number(_botScore));*/
           /* ui->playerFood_label->setText(QString::number(_versusEnvironment->player(0)->averageScore().food));
            ui->playerSteps_label->setText(QString::number(_versusEnvironment->player(0)->averageScore().steps));
            ui->playerDeaths_label->setText(QString::number(_versusEnvironment->player(0)->deathCount()));
            ui->playerKills_label->setText(QString::number(_playerKills));
            ui->playerScore_label->setText(QString::number(getScore(_versusEnvironment->player(0))));

            ui->botFood_label->setText(QString::number(_versusEnvironment->player(1)->averageScore().food));
            ui->botSteps_label->setText(QString::number(_versusEnvironment->player(1)->averageScore().steps));
            ui->botDeaths_label->setText(QString::number(_versusEnvironment->player(1)->deathCount()));
            ui->botKills_label->setText(QString::number(_botKills));
            ui->botScore_label->setText(QString::number(getScore(_versusEnvironment->player(1))));*/
            break;
        }
        case Modus::backpropTraining:
        {
            _backpropTrainingEnvironment->update();
            break;
        }
    }

    if(_enableDisplay)
      this->update();
    if(_record_enable && _enableDisplay)
    {
        takescreenshot();
        if(_record_imageList.size() >= 100)
            savescreenshot();
    }
    try{
        handleNet();
    }catch(std::runtime_error &e)
    {
        //qDebug() << "ERROR  setSnakeOnMap("<<a<<") "<< e.what();
        QString errorMessage = "ERROR: handleNet())\n";
        errorMessage+= QString(e.what())+"\n";
        //errorMessage+= "playersize: "+std::to_string(_environment->)+"\n";
        qDebug() << errorMessage;
        FILE *file = fopen("error_e.txt","a");
        if(file)
        {
            fprintf(file,"%s\n",errorMessage.toStdString().c_str());
            fclose(file);
        }
    }catch(...)
    {
        qDebug() << "ERROR  this->update() -> unnkown "<<GetLastErrorStr();
        saveError("ERROR  this->update() -> unnkown "+GetLastErrorStr());
    }

    switch(_modus)
    {
        case Modus::geneticTraining:
        {

            break;
        }
        case Modus::versusAI:
        {
            _playerKeyInputDisabler = false;
            break;
        }
        case Modus::backpropTraining:
        {
            _playerKeyInputDisabler = false;
            break;
        }
    }


    _step++;
}
void Snake::timerEvent2()
{
        QStringList data = net->toStringList();
        for(int a=0; a<data.size(); a++)
        {
            //qDebug() << data[a];
        }
        double filter = 0.9;
        _genPerSecond =(double)  filter*_genPerSecond + (1-filter)*(1000*_genPerSecCounter/(double)_updateTimer2->interval());
        ui->genPerSec_label->setText(QString::fromStdString(to_string(_genPerSecond)));
        _genPerSecCounter = 0;
        _stats_genPerSec_Lineseries->append(_stats_genPerSec_Lineseries->at(_stats_genPerSec_Lineseries->count()-1).x()+1,_genPerSecond);


        _stats_performanceChart->axisX()->setMax(_stats_genPerSec_Lineseries->at(_stats_genPerSec_Lineseries->count()-1).x());
        //qDebug() << "min"<<_stats_genPerSec_Lineseries->at(0).x() << " max"<<_stats_genPerSec_Lineseries->at(_stats_genPerSec_Lineseries->count()-1).x()+1;
        if(_stats_genPerSec_Lineseries->count()>_maxChartSize)
        {

            _stats_genPerSec_Lineseries->remove(0);
          //  _stats_performanceChart->scroll(_stats_genPerSec_Lineseries->at(_stats_genPerSec_Lineseries->count()-1).x(),0);
            //_stats_performanceChart->axisX()->setMax(_stats_genPerSec_Lineseries->points().size());
           // _stats_performanceChart->axisX()->setMin(_stats_genPerSec_Lineseries->points().size()-_maxChartSize);
        }
        _stats_performanceChart->axisX()->setMin(_stats_genPerSec_Lineseries->at(0).x());
        if(_genPerSecond >_stats_maxGenPerSec)
        {
            _stats_maxGenPerSec = _genPerSecond;

        }

        _calcPerSecond =(double) /*_genPerSecond*0.9*/ + 1000*_calcPerSecCounter/(double)_updateTimer2->interval();
        if(_fullSycleTime != 0)
            _averageCalcPerSec = filter*_averageCalcPerSec + (1-filter)* 1/_fullSycleTime;

        ui->calcPerSec_label->setText(QString::fromStdString(to_string(/*_calcPerSecond*/_averageCalcPerSec)));
        _averageCalcPerSec_List.push_back(_averageCalcPerSec);
        _averageEnviromentCycleTime.push_back(_environment->cycleTime());
        _averageNetCycleTime.push_back(net->get_runtime());
        _averageSnakeCycleTime.push_back(_fullSycleTime);
        _calcPerSecCounter = 0;
        _stats_calcPerSec_Lineseries->append(_stats_calcPerSec_Lineseries->at(_stats_calcPerSec_Lineseries->count()-1).x()+1,_averageCalcPerSec);


        if(_stats_calcPerSec_Lineseries->count()>_maxChartSize)
        {
            _stats_calcPerSec_Lineseries->remove(0);
            //_stats_performanceChart->scroll((_stats_performanceChart->axisX(_stats_calcPerSec_Lineseries) - _stats_performanceChart->axisX()->min()),0);
            //_stats_calcPerSec_Lineseries->set
        }
        if(_averageCalcPerSec >_stats_maxCalcPerSec)
        {
            _stats_maxCalcPerSec = _averageCalcPerSec;
        }
        if(_stats_maxCalcPerSec>_stats_maxGenPerSec)
        {
            _stats_performanceChart->axisY()->setMax(_stats_maxCalcPerSec);
        }else{
            _stats_performanceChart->axisY()->setMax(_stats_maxGenPerSec);
        }

        switch(_modus)
        {
            case Modus::geneticTraining:
            {
               // _environment->update();
                break;
            }
            case Modus::versusAI:
            {
               // _versusEnvironment->update();

             /*   ui->playerFood_label->setText(QString::number(_playerFood+_versusEnvironment->player(0)->food()));
                ui->playerSteps_label->setText(QString::number(_playerSteps+_versusEnvironment->player(0)->steps()));
                ui->playerDeaths_label->setText(QString::number(_playerDeaths));
                ui->playerKills_label->setText(QString::number(_playerKills));
                ui->playerScore_label->setText(QString::number(_playerScore));

                ui->botFood_label->setText(QString::number(_botFood+_versusEnvironment->player(1)->food()));
                ui->botSteps_label->setText(QString::number(_botSteps+_versusEnvironment->player(1)->steps()));
                ui->botDeaths_label->setText(QString::number(_botDeaths));
                ui->botKills_label->setText(QString::number(_botKills));
                ui->botScore_label->setText(QString::number(_botScore));*/
                ui->playerFood_label->setText(QString::number(_versusEnvironment->player(0)->averageScore().food));
                ui->playerSteps_label->setText(QString::number(_versusEnvironment->player(0)->averageScore().steps));
                ui->playerDeaths_label->setText(QString::number(_versusEnvironment->player(0)->deathCount()));
                ui->playerKills_label->setText(QString::number(_playerKills));
                ui->playerScore_label->setText(QString::number(getScore(_versusEnvironment->player(0))));

                ui->botFood_label->setText(QString::number(_versusEnvironment->player(1)->averageScore().food));
                ui->botSteps_label->setText(QString::number(_versusEnvironment->player(1)->averageScore().steps));
                ui->botDeaths_label->setText(QString::number(_versusEnvironment->player(1)->deathCount()));
                ui->botKills_label->setText(QString::number(_botKills));
                ui->botScore_label->setText(QString::number(getScore(_versusEnvironment->player(1))));
                break;
            }
            case Modus::backpropTraining:
            {
               // _backpropTrainingEnvironment->update();
                break;
            }
        }

        if(_updateTimer->interval() != ui->speed_slider->value())
        {
            _updateTimer->setInterval(ui->speed_slider->maximum() - ui->speed_slider->value());
        }
        if(_environment->foodAmount() != ui->food_slider->value())
        {
            _environment->foodAmount(ui->food_slider->value());
            _versusEnvironment->foodAmount(ui->food_slider->value());
            _backpropTrainingEnvironment->foodAmount(ui->food_slider->value());
        }
        ui->score_label->setText(QString::number(_averageScore_smoth));

        ui->food_label->setText(QString::number(_environment->player(0)->food()));
        ui->steps_label->setText(QString::number(generation));
        ui->record_imagesInBuffer_label->setText(QString::number(_record_imageList.size()));
}

void Snake::handleNet()
{
    //qDebug() << "handle Net";
    unsigned int deathCounter = 0;

    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            for(unsigned int animal=0; animal<net->get_animals(); animal++)
            {
                if(!_environment->player(animal)->isAlive())
                {
                    if(_environment->player(animal)->deathCount() < _respawnAmount)
                    {
                        _environment->player(animal)->revive();
                    }
                    deathCounter++;
                    continue;
                }
                //std::vector<std::vector<double>    > netInput = _environment->AI_mapData(animal,_fieldOfView);
#ifdef GLOBALVIEW
                std::vector<std::vector<double>    > netInput = _environment->AI_mapData(animal);
#else
                std::vector<std::vector<double>    > netInput = _environment->AI_mapData_simple(animal);
#endif

                unsigned int inputIndex = 0;
                for(unsigned int b=0; b<netInput.size(); b++)
                {
                    for(unsigned int c=0; c<netInput[b].size(); c++)
                    {
                        net->set_input(animal,inputIndex,netInput[b][c]);
                        inputIndex++;
                    }
                }
               // net->run(animal);
             }
            net->run();

            //_calcPerSecCounter++;
            for(unsigned int animal=0; animal<net->get_animals(); animal++)
            {
                if(!_environment->player(animal)->isAlive())
                {
                    continue;
                }
                std::vector<double> output = net->get_output(animal);

                //----------------Control the Snakes-------------------
               /* if(animal == 0 && _selfControl)
                {
                    qDebug() << "continue";
                    continue;
                }*/
                /*if(output[0] > 0.2f && output[1] > 0.2f)
                {
                    if(output[0] > output[1])
                    {
                        _environment->controlSnakeDirection(animal,Direction::_left);
                    }else
                    {
                        _environment->controlSnakeDirection(animal,Direction::_right);
                    }
                }*/

#ifdef GLOBALVIEW
                _environment->controlSnakeDirection(animal,getDirectionFromData(output,_environment->player(animal)->direction()));
#else
                _environment->controlSnakeDirection(animal,getDirectionFromData(output));
#endif

            }

            if(deathCounter == net->get_animals())
            {
                if(_record_enable)
                    savescreenshot();
#ifdef DEBUG_PRINTS
                qDebug() << "all death";
#endif
                _snakeScore.clear();
                double averageScore = 0;
                double maxScore = 0;
                double minScore = 0;
                double averageFoodScore = 0;
                double averageStepScore = 0;
                for(unsigned int animal=0; animal<net->get_animals(); animal++)
                {
#ifdef DEBUG_PRINTS
                    if(animal == 0)
                    {
                        qDebug() << "score: food:"<<_environment->player(animal)->averageScore().food<< " steps: "<< _environment->player(animal)->averageScore().steps;
                        qDebug() << "-----------------";
                        for(int b=0; b<_environment->player(animal)->score().size(); b++)
                        {
                            qDebug() << "--: food:"<<_environment->player(animal)->score(b).food<< " steps: "<< _environment->player(animal)->score(b).steps;
                        }
                    }
#endif
                    _snakeScore.push_back(getScore(_environment->player(animal)));
                    if(animal == 0)
                    {
                        maxScore = _snakeScore[_snakeScore.size()-1];
                        minScore = _snakeScore[_snakeScore.size()-1];
                    }else {
                        if(_snakeScore[_snakeScore.size()-1] < minScore)
                            minScore = _snakeScore[_snakeScore.size()-1];
                        if(_snakeScore[_snakeScore.size()-1] > maxScore)
                            maxScore = _snakeScore[_snakeScore.size()-1];
                    }
                    averageScore        += _snakeScore[_snakeScore.size()-1] / net->get_animals();
                    averageFoodScore    += _environment->player(animal)->averageScore().food / net->get_animals();
                    averageStepScore    += (double)_environment->player(animal)->averageScore().steps / net->get_animals();
                }
#ifdef DEBUG_PRINTS
                qDebug() << "average score: "<<averageScore;
#endif
                _minScoreList.push_back(minScore);
                _maxScoreList.push_back(maxScore);
                _averageScoreList.push_back(averageScore);
                _foodScore.push_back(averageFoodScore);
                _stepScore.push_back(averageStepScore);



                _averageScoreList_smoth_tmp.push_back(averageScore);
                if(_averageScoreList_smoth_tmp.size() > 20){_averageScoreList_smoth_tmp.erase(_averageScoreList_smoth_tmp.begin());}
                _maxScoreList_smoth_tmp.push_back(maxScore);
                if(_maxScoreList_smoth_tmp.size() > 20){_maxScoreList_smoth_tmp.erase(_maxScoreList_smoth_tmp.begin());}
                _minScoreList_smoth_tmp.push_back(minScore);
                if(_minScoreList_smoth_tmp.size() > 20){_minScoreList_smoth_tmp.erase(_minScoreList_smoth_tmp.begin());}
                _foodScore_smoth_tmp.push_back(averageFoodScore);
                if(_foodScore_smoth_tmp.size() > 20){_foodScore_smoth_tmp.erase(_foodScore_smoth_tmp.begin());}
                _stepScore_smoth_tmp.push_back(averageStepScore);
                if(_stepScore_smoth_tmp.size() > 20){_stepScore_smoth_tmp.erase(_stepScore_smoth_tmp.begin());}

                averageScore = 0;
                maxScore = 0;
                minScore = 0;
                averageFoodScore = 0;
                averageStepScore = 0;
                for(unsigned int a=0; a<_averageScoreList_smoth_tmp.size(); a++)
                {
                    averageScore += _averageScoreList_smoth_tmp[a];
                    maxScore += _maxScoreList_smoth_tmp[a];
                    minScore += _minScoreList_smoth_tmp[a];
                    averageFoodScore += _foodScore_smoth_tmp[a];
                    averageStepScore += _stepScore_smoth_tmp[a];
                }
                _averageScoreList_smoth.push_back(averageScore/_averageScoreList_smoth_tmp.size());
                _averageScore_smoth = _averageScoreList_smoth[_averageScoreList_smoth.size()-1];
                _maxScoreList_smoth.push_back(maxScore/_maxScoreList_smoth_tmp.size());
                _minScoreList_smoth.push_back(minScore/_minScoreList_smoth_tmp.size());
                _foodScore_smoth.push_back(averageFoodScore/_foodScore_smoth_tmp.size());
                _stepScore_smoth.push_back(averageStepScore/_stepScore_smoth_tmp.size());

                if(/*_averageScoreList_smoth[_averageScoreList_smoth.size()-1] >_stats_maxAverageScore ||*/ _averageScoreList_smoth[_averageScoreList_smoth.size()-1] > _stats_maxAverageScore)
                {
                    /*if(_averageScoreList_smoth[_averageScoreList_smoth.size()-1] > _stats_maxAverageScore)
                        _stats_maxAverageScore = _averageScoreList_smoth[_averageScoreList_smoth.size()-1];
                    else*/ {
                        _stats_maxAverageScore = _averageScoreList_smoth[_averageScoreList_smoth.size()-1];
                    }
                    _stats_score_Chart->axisY()->setMax(_stats_maxAverageScore);
                }
                _stats_averageScore_LineSeries->append(_stats_averageScore_LineSeries->at(_stats_averageScore_LineSeries->count()-1).x()+1,_averageScoreList_smoth[_averageScoreList_smoth.size()-1]);
               // _stats_Score_LineSeries->append(_stats_Score_LineSeries->at(_stats_Score_LineSeries->count()-1).x()+1,averageScore);

                _stats_score_Chart->axisX()->setMax(_stats_averageScore_LineSeries->at(_stats_averageScore_LineSeries->count()-1).x());
                if(_stats_averageScore_LineSeries->count()>_maxChartSize)
                {
                    _stats_averageScore_LineSeries->remove(0);
                }
                if(_stats_Score_LineSeries->count()>_maxChartSize)
                {
                    _stats_Score_LineSeries->remove(0);
                }
                _stats_score_Chart->axisX()->setMin(_stats_averageScore_LineSeries->at(0).x());
#ifdef DEBUG_PRINTS
                qDebug() << "learn";
#endif
                net->learn(_snakeScore);
#ifdef DEBUG_PRINTS
                qDebug() << "learnEnd";
#endif
                generation++;
                _genPerSecCounter++;


                for(unsigned int animal=0; animal<net->get_animals(); animal++)
                {
                    _environment->player(animal)->resetDeathCount();
                    _environment->player(animal)->resetScore();
                    _environment->player(animal)->revive();
                }
                _environment->obsticleReplace();
   /*             _saveCounter++;
                if(_saveCounter > 500)
                {
                    _saveCounter = 0;
                    on_saveStats_pushbutton_clicked();
                }*/
                _step = 0;
               // on_saveStats_pushbutton_clicked();
                if(_record_enable &&
                   _record_displayAutoEnabler &&
                   (generation%_record_generationInterval == 0 ||
                    (generation-1)%_record_generationInterval == 0))
                {
                    //if(_record_generationInterval > 1)
                    {
                        if(generation%_record_generationInterval == 0 && !_enableDisplay)
                        {
                            qDebug() << "toggleDisplay on";
                            on_toggleDisplay_pushbutton_clicked();
                        }else if((generation-1)%_record_generationInterval == 0 && _enableDisplay)
                        {
                            qDebug() << "toggleDisplay off";
                            on_toggleDisplay_pushbutton_clicked();
                        }
                    }


                }
            }
            break;
        }
        case Modus::versusAI:
        {
            if(!_versusEnvironment->player(0)->isAlive() && !_versusEnvironment->player(1)->isAlive())
            {
                _versusEnvironment->obsticleReplace();
            }
            if(!_versusEnvironment->player(0)->isAlive())// player
            {
                _versusEnvironment->player(0)->revive();
            }
            if(!_versusEnvironment->player(1)->isAlive())// bot
            {
                _versusEnvironment->player(1)->revive();
            }
            _versusEnvironment->AI_mapData_simple(0);
#ifdef GLOBALVIEW
                std::vector<std::vector<double>    > netInput = _environment->AI_mapData(1);
#else
           std::vector<std::vector<double>    > netInput = _versusEnvironment->AI_mapData_simple(1);
#endif
            unsigned int inputIndex = 0;
            for(unsigned int b=0; b<netInput.size(); b++)
            {
                for(unsigned int c=0; c<netInput[b].size(); c++)
                {
                    net->set_input(_selectedSnake,inputIndex,netInput[b][c]);
                    inputIndex++;
                }
            }
            net->run(_selectedSnake);

            std::vector<double> output = net->get_output(_selectedSnake);
#ifdef GLOBALVIEW
            _environment->controlSnakeDirection(1,getDirectionFromData(output,_environment->player(animal)->direction()));
#else
            _versusEnvironment->controlSnakeDirection(1,getDirectionFromData(output));
#endif



            _versusSaveScoreCount++;
            if(_versusSaveScoreCount % _versusSaveScoreInterval == 0)
            {
                unsigned int averageSize = 10;
                _versusPlayerScore.push_back(_versusEnvironment->player(0)->currentScore());
                _versusPlayerAbsolutAverageScore.push_back(_versusEnvironment->player(0)->averageScore(_versusPlayerScore));
                _versusPlayerAverageScore_tmpBuffer.push_back(_versusPlayerScore[_versusPlayerScore.size()-1]);
                if(_versusPlayerAverageScore_tmpBuffer.size() > averageSize)
                {
                    _versusPlayerAverageScore_tmpBuffer.erase(_versusPlayerAverageScore_tmpBuffer.begin());
                }
                _versusPlayerAverageScore.push_back(_versusEnvironment->player(0)->averageScore(_versusPlayerAverageScore_tmpBuffer));

                _versusBotScore.push_back(_versusEnvironment->player(1)->currentScore());
                _versusBotAbsolutAverageScore.push_back(_versusEnvironment->player(1)->averageScore(_versusBotScore));
                _versusBotAverageScore_tmpBuffer.push_back(_versusBotScore[_versusBotScore.size()-1]);
                if(_versusBotAverageScore_tmpBuffer.size() > averageSize)
                {
                    _versusBotAverageScore_tmpBuffer.erase(_versusBotAverageScore_tmpBuffer.begin());
                }
                _versusBotAverageScore.push_back(_versusEnvironment->player(1)->averageScore(_versusBotAverageScore_tmpBuffer));
#ifdef DEBUG_PRINTS
                qDebug() << "player: F: "<<_versusPlayerScore[_versusPlayerScore.size()-1].food << " S: "<<_versusPlayerScore[_versusPlayerScore.size()-1].steps << " A: " <<_versusPlayerAverageScore[_versusPlayerAverageScore.size()-1].food << " : " <<_versusPlayerAverageScore[_versusPlayerAverageScore.size()-1].steps;
                qDebug() << "Bot   : F: "<<_versusBotScore[_versusBotScore.size()-1].food << " S: "<<_versusBotScore[_versusBotScore.size()-1].steps<< " A: " <<_versusBotAverageScore[_versusBotAverageScore.size()-1].food << " : " <<_versusBotAverageScore[_versusBotAverageScore.size()-1].steps;
#endif
            }
            break;
        }
        case Modus::backpropTraining:
        {
            //_versusEnvironment->AI_mapData_simple(0);
            std::vector<std::vector<double>    > netInput = _backpropTrainingEnvironment->AI_mapData_simple(0);
            std::vector<double> inputVec;
            unsigned int inputIndex = 0;
            for(unsigned int b=0; b<netInput.size(); b++)
            {
                for(unsigned int c=0; c<netInput[b].size(); c++)
                {
                    inputVec.push_back(netInput[b][c]);
                    inputIndex++;
                }
            }
            _backpropTrainingInputs.push_back(inputVec);
            _backpropTrainingOutputs.push_back(std::vector<double>{2,0});

            net->set_input(_selectedSnake,inputVec);
            net->run(_selectedSnake);
            _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1] = net->get_output(_selectedSnake);
            _backpropTrainingEnvironment->controlSnakeDirection(0,getDirectionFromData(_backpropTrainingOutputs[_backpropTrainingOutputs.size()-1]));
            //net->run(0);

            //std::vector<double> output = net->output(0);
           /* if(output.size() != 2)
            {
                qDebug() << "error: net outputSize wrong: "<< output.size();
            }
            else
            {
                if(output[0] > 0.2f && output[1] > 0.2f)
                {
                    if(output[0] > output[1])
                    {
                        _versusEnvironment->controlSnakeDirection(1,Direction::_left);
                    }else
                    {
                        _versusEnvironment->controlSnakeDirection(1,Direction::_right);
                    }
                }
            }*/

            if(!_backpropTrainingEnvironment->player(0)->isAlive())
            {
                _backpropTrainingEnvironment->obsticleReplace();

                struct Score playerScore = _backpropTrainingEnvironment->player(0)->averageScore();
                _playerFood += playerScore.food;
                _playerSteps += playerScore.steps;
                _playerFood/=2;
                _playerSteps/=2;
                _playerDeaths++;

                _playerScore += getScore(_backpropTrainingEnvironment->player(0));
                _playerScore/=2;


                _backpropTrainingEnvironment->player(0)->revive();
                saveBackpropTrainingsData();

            }
            break;
        }
    }
}
void Snake::saveBackpropTrainingsData()
{
    if(_backpropTrainingInputs.size() == 0)
    {
#ifdef DEBUG_PRINTS
        qDebug() << "saveBackpropTrainingsData no data to save";
#endif
    }
    if(_backpropTrainingInputs.size() != _backpropTrainingOutputs.size())
    {
        qDebug() << "ERROR: saveBackpropTrainingsData size != size" << _backpropTrainingInputs.size() << " != " << _backpropTrainingOutputs.size();
    }
    //_backpropTrainingInputs.erase(_backpropTrainingInputs.begin());
    FILE *_file = fopen(_backprobTrainingsDataFileName.toStdString().c_str(),"a");
    if(_file)
    {
        for(unsigned int a=0; a<_backpropTrainingInputs.size(); a++)
        {
            string outputLine = "";
            for(unsigned int b=0; b<_backpropTrainingInputs[a].size(); b++)
            {
                outputLine += to_string(_backpropTrainingInputs[a][b]) + ";";
            }
            outputLine += ";";
            for(unsigned int b=0; b<_backpropTrainingOutputs[a].size(); b++)
            {
                outputLine += to_string(_backpropTrainingOutputs[a][b]) + ";";
            }
            fprintf(_file,"%s\n",outputLine.c_str());
        }
        fclose(_file);
    }
    _backpropTrainingInputs.clear();
    _backpropTrainingOutputs.clear();
}
void Snake::saveVersusData()
{
    if(_versusPlayerScore.size() != _versusBotScore.size() || _versusBotScore.size() == 0 || _versusPlayerScore.size() == 0)
        return;
    FILE *_versusSaveFile = fopen(_versusSaveFileName.toStdString().c_str(),"w");
    if(_versusSaveFile)
    {
        fprintf(_versusSaveFile,"PlayerFood;PlayerSteps;PlayerScore;PlayerAverageScore;PlayerAbsolutAverageScore;;BotFood;BotSteps;BotScore;BotAverageScore;BotAbsolutAverageScore;\n");
        for(unsigned int a=0; a<_versusPlayerScore.size(); a++)
        {
            fprintf(_versusSaveFile,"%.3f;%u;%.3f;%.3f;%.3f;;%.3f;%u;%.3f;%.3f;%.3f;\n",_versusPlayerScore[a].food,_versusPlayerScore[a].steps,getScore(_versusPlayerScore[a]),getScore(_versusPlayerAverageScore[a]),getScore(_versusPlayerAbsolutAverageScore[a]),_versusBotScore[a].food,_versusBotScore[a].steps,getScore(_versusBotScore[a]),getScore(_versusBotAverageScore[a]),getScore(_versusBotAbsolutAverageScore[a]));
        }
        _versusPlayerScore.clear();
        _versusPlayerAverageScore.clear();
        _versusPlayerAbsolutAverageScore.clear();
        _versusBotScore.clear();
        _versusBotAverageScore.clear();
        _versusBotAbsolutAverageScore.clear();
        fclose(_versusSaveFile);
    }
}
void Snake::loadBackpropTrainignsData(std::vector<std::vector<double> > &inputs,std::vector<std::vector<double>  > &outputs)
{
    FILE *_file = fopen(_backprobTrainingsDataFileName.toStdString().c_str(),"r");
    if(!_file)
    {
        qDebug() << "File: "<<_backprobTrainingsDataFileName << " not found";
        return;
    }
    std::vector<string> fileBuffer;
    while(!feof(_file))
    {
        char list[255];
        fgets(list,255,_file);
        fileBuffer.push_back(string(list));
    }
    inputs.clear();
    outputs.clear();
    for(unsigned int a=0; a<fileBuffer.size(); a++)
    {
        string inputList = fileBuffer[a].substr(0,fileBuffer[a].find(";;")+1);
        string outputList = fileBuffer[a].substr(fileBuffer[a].find(";;")+2,fileBuffer[a].size());
        std::vector<double> tmpInput;
        std::vector<double> tmpOutput;

        do
        {
            tmpInput.push_back(stof(inputList.substr(0,inputList.find(";"))));
            inputList = inputList.substr(inputList.find(";")+1,inputList.size());
        }while(inputList.find(";") != -1);
        do
        {
            tmpOutput.push_back(stof(outputList.substr(0,outputList.find(";"))));
            outputList = outputList.substr(outputList.find(";")+1,outputList.size());
        }while(outputList.find(";") != -1);
#ifdef DEBUG_PRINTS
        qDebug() << tmpInput << tmpOutput;
#endif
        inputs.push_back(tmpInput);
        outputs.push_back(tmpOutput);
    }

    fclose(_file);
}
Direction Snake::getDirectionFromData(std::vector<double> inputs)
{
    Direction dir = Direction::_up;
    double maximum = inputs[0];
    unsigned int index = 0;
    for(unsigned int a=1; a<inputs.size(); a++)
    {
        if(maximum < inputs[a])
        {
            maximum = inputs[a];
            index = a;
        }
    }
    switch(index)
    {
        case 0:
        {
            dir = Direction::_up;
            break;
        }
        case 1:
        {
            dir = Direction::_left;
            break;
        }
        case 2:
        {
            dir = Direction::_right;
            break;
        }
        default:
        {
            qDebug() << "getDirectionFromData(std::vector<double> inputs) " << index;
        }
    }
    /*if(inputs[0] > 0.2f || inputs[1] > 0.2f)
    {
        if(inputs[0] > inputs[1])
        {
            dir = Direction::_left;
        }else
        {
            dir = Direction::_right;
        }
    }*/
    return dir;
}
Direction Snake::getDirectionFromData(std::vector<double> inputs,Direction moveDirection)
{
    int dir = getDirectionFromData(inputs);
    //qDebug() << "dir: "<<dir<<" moving: "<<(int)moveDirection << " now: "<<(int)Direction((dir + moveDirection)%4);
    return Direction((dir + moveDirection)%4);
}
double Snake::getScore(Player *player)
{
    if(player == nullptr)
        return 0.f;
    return getScore(player->averageScore());
}
double Snake::getScore(Score score)
{
    double scoreFoodFactor = 0.2f;
    double scoreStepFactor = 0.1f;
    return score.food * scoreFoodFactor + score.food * scoreStepFactor;
}

void Snake::keyPressEvent(QKeyEvent *e)
{
    if((_modus != Modus::versusAI && _modus != Modus::backpropTraining) || _playerKeyInputDisabler)
    {
        return;
    }
    switch(char(e->key()))
    {
        //CONTROL 1
        case 'W'://up
        {
            //_selfControl = !_selfControl;

            _playerKeyInputDisabler = true;
            switch(_modus)
            {
                case Modus::versusAI:
                {
                    _versusEnvironment->controlSnakeDirection(0,Direction::_up);
                    break;
                }
                case Modus::backpropTraining:
                {
                    std::vector<double> tmpOutput;
                    tmpOutput.push_back(0);
                    tmpOutput.push_back(0);
                    _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1] = tmpOutput;
                    break;
                }
            }
            break;
        }
        case 'A'://left
        {

            _playerKeyInputDisabler = true;
            switch(_modus)
            {
                case Modus::versusAI:
                {
                    _versusEnvironment->controlSnakeDirection(0,Direction::_left);
                    break;
                }
                case Modus::backpropTraining:
                {
                    std::vector<double> tmpOutput;
                    tmpOutput.push_back(1);
                    tmpOutput.push_back(0);
                    _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1] = tmpOutput;
                    break;
                }
            }
            break;
        }
        case 'S'://down
        {

            _playerKeyInputDisabler = true;
            switch(_modus)
            {
                case Modus::versusAI:
                {
                    _versusEnvironment->controlSnakeDirection(0,Direction::_down);
                    break;
                }
                case Modus::backpropTraining:
                {
                    std::vector<double> tmpOutput;
                    tmpOutput.push_back(0);
                    tmpOutput.push_back(0);
                    _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1] = tmpOutput;
                    break;
                }
            }
            break;
        }
        case 'D'://right
        {

            _playerKeyInputDisabler = true;
            switch(_modus)
            {
                case Modus::versusAI:
                {
                    _versusEnvironment->controlSnakeDirection(0,Direction::_right);
                    break;
                }
                case Modus::backpropTraining:
                {
                    std::vector<double> tmpOutput;
                    tmpOutput.push_back(0);
                    tmpOutput.push_back(1);
                    _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1] = tmpOutput;
                    break;
                }
            }
            break;
        }
        //CONTOL 2
        case 'I'://up
        {
            _environment->controlSnakeDirection(1,Direction::_up);
            break;
        }
        case 'J'://left
        {
            _environment->controlSnakeDirection(1,Direction::_left);
            break;
        }
        case 'K'://down
        {
            _environment->controlSnakeDirection(1,Direction::_down);
            break;
        }
        case 'L'://right
        {
            _environment->controlSnakeDirection(1,Direction::_right);
            break;
        }
        //------------------
        case 'F':
        {
            _environment->controlSnakeAddSize(0,1);
            break;
        }
        case 'G':
        {
            _environment->controlSnakeAddSize(0,-1);
            break;
        }
        case 'M':
        {
            _environment->controlSnakeDeath(0, false);
            _environment->controlSnakeDeath(1, false);
            break;
        }
        case 'V':
        {
            //_environment->controlSnakeAddFood(0,500);
            timerEvent();

            break;
        }
    }
    if(_modus == Modus::backpropTraining)
    {

        /*if(_backpropTrainingOutputs[_backpropTrainingOutputs.size()-1][0] > 0.2f || _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1][1] > 0.2f)
        {
            if(_backpropTrainingOutputs[_backpropTrainingOutputs.size()-1][0] > _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1][1])
            {
                qDebug() << _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1];
                _backpropTrainingEnvironment->controlSnakeDirection(0,Direction::_left);
            }else
            {
                qDebug() << _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1];
                _backpropTrainingEnvironment->controlSnakeDirection(0,Direction::_right);
            }
        }*/
        _backpropTrainingEnvironment->controlSnakeDirection(0,getDirectionFromData(_backpropTrainingOutputs[_backpropTrainingOutputs.size()-1]));
    }
}
void Snake::keyReleaseEvent(QKeyEvent *e)
{

}

void Snake::on_pause_pushButton_clicked()
{
    _pause = !_pause;
    if(_pause)
        _updateTimer->stop();
    else
    {
        _fullSycleTimeStart = std::chrono::high_resolution_clock::now();
        _updateTimer->start();
    }
}
void Snake::on_saveStats_pushbutton_clicked()
{
    qDebug() << "save";
    try{
        saveVersusData();
        net->get_ptr_saveNet()->set_ExtraParam("generation",(double)generation);
        net->saveToNetFile();
        savescreenshot();
    }catch(std::runtime_error &e)
    {
        qDebug() << "error: "<< e.what();
    }
    bool fileExists = false;
    FILE *statsFile = fopen(_statsFilename.toStdString().c_str(),"r");
    if(statsFile)
    {
        fileExists = true;
        fclose(statsFile);
    }
    statsFile = fopen(_statsFilename.toStdString().c_str(),"a");
    if(!statsFile)
        return;

    if(!fileExists)
    {
        fprintf(statsFile,"minScore;maxScore;foodScoreAverage;stepScoreAverage;;minScore_smoth;maxScore_smoth;foodScoreAverage_smoth;stepScoreAverage_smoth;;average;average_smoth;\n");
    }
    for(unsigned int a=0; a<_averageScoreList.size(); a++)
    {
        fprintf(statsFile,"%.8f;%.8f;%.8f;%.8f;;%.8f;%.8f;%.8f;%.8f;;%.8f;%.8f;\n",
                _minScoreList[a],_maxScoreList[a],_foodScore[a],_stepScore[a],
                _minScoreList_smoth[a],_maxScoreList_smoth[a],_foodScore_smoth[a],_stepScore_smoth[a],_averageScoreList[a],_averageScoreList_smoth[a]);
    }
    fclose(statsFile);

    //-------------------------
    savePeroformanceData();
    //-------------------------



    _minScoreList.clear();
    _maxScoreList.clear();
    _foodScore.clear();
    _stepScore.clear();
    _averageScoreList.clear();
    _stepScore_smoth.clear();
    _foodScore_smoth.clear();
    _minScoreList_smoth.clear();
    _maxScoreList_smoth.clear();
    _averageScoreList_smoth.clear();
    qDebug() << "save done";
}
void Snake::savePeroformanceData()
{
    FILE *statsFile = fopen(_calcPerSecFileName.toStdString().c_str(),"r");
    bool fileExists = false;
    if(statsFile)
    {
        fileExists = true;
        fclose(statsFile);
    }
    statsFile = fopen(_calcPerSecFileName.toStdString().c_str(),"a");
    if(!statsFile)
        return;

    if(!fileExists)
    {
        fprintf(statsFile,"Calculations Per Second;AverageEnviromentCycleTime [ms];AverageNetCycleTime [ms];AverageSnakeCycleTime [ms];\n");
    }
    for(unsigned int a=0; a<_averageCalcPerSec_List.size(); a++)
    {
        fprintf(statsFile,"%.8f;%.8f;%.8f;%.8f\n",(double)_averageCalcPerSec_List[a],
                                                  _averageEnviromentCycleTime[a]*1000,
                                                  _averageNetCycleTime[a]*1000,
                                                  _averageSnakeCycleTime[a]*1000);
    }
    fclose(statsFile);
    _averageCalcPerSec_List.clear();
}
void Snake::on_kill_pushButton_clicked()
{
    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            for(unsigned int a=0; a<net->get_animals(); a++)
            {
                _environment->player(a)->kill();
            }
            break;
        }
        case Modus::versusAI:
        {
            for(unsigned int a=0; a<_versusEnvironment->playerAmount(); a++)
            {
                _versusEnvironment->player(a)->kill();
            }
            break;
        }
        case Modus::backpropTraining:
        {
            for(unsigned int a=0; a<_backpropTrainingEnvironment->playerAmount(); a++)
            {
                _backpropTrainingEnvironment->player(a)->kill();
            }
            break;
        }
    }
}
void Snake::on_toggleDisplay_pushbutton_clicked(bool checked)
{
    _record_displayAutoEnabler = !_record_displayAutoEnabler;
}
void Snake::on_toggleDisplay_pushbutton_clicked()
{
    _enableDisplay = !_enableDisplay;
    _environment->drawEnable(_enableDisplay);
    _versusEnvironment->drawEnable(_enableDisplay);
    _backpropTrainingEnvironment->drawEnable(_enableDisplay);


    if(!_enableDisplay)
    {
        _versusEnvironment->showInfoText(false);
        _environment->showInfoText(false);
        _backpropTrainingEnvironment->showInfoText(false);
        visu->stopUpdateSlot();
        return;
    }


    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            _environment->showInfoText(ui->mapinfo_checkbox->isChecked());
            _versusEnvironment->showInfoText(false);
            _backpropTrainingEnvironment->showInfoText(false);
            visu->startUpdateSlot();
            break;
        }
        case Modus::versusAI:
        {
            _environment->showInfoText(false);
            _backpropTrainingEnvironment->showInfoText(false);
            _versusEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
            visu->startUpdateSlot();
            break;
        }
        case Modus::backpropTraining:
        {
            _environment->showInfoText(false);
            _backpropTrainingEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
            _versusEnvironment->showInfoText(false);
            visu->startUpdateSlot();
            break;
        }
    }
}
void Snake::on_mapinfo_checkbox_stateChanged(int arg1)
{
    if(!_enableDisplay)
        return;
    _versusEnvironment->showInfoText((bool)arg1);
    _environment->showInfoText((bool)arg1);
}
void Snake::on_geneticTraining_radioButton_clicked(bool checked)
{
    _modus = Modus::geneticTraining;

    modeReset();
    _environment->showInfoText(ui->mapinfo_checkbox->isChecked());
}
void Snake::on_versusAI_radioButton_clicked(bool checked)
{
    _modus = Modus::versusAI;


    for(unsigned int a=0; a<_versusEnvironment->playerAmount(); a++)
    {
        _versusEnvironment->controlSnakeDeath(a,true);
        _versusEnvironment->player(a)->revive();
    }


    modeReset();
    _versusEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
}
void Snake::on_backpropTraining_radioButton_clicked(bool checked)
{
    _modus = Modus::backpropTraining;
    modeReset();
    _backpropTrainingEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
}
void Snake::logGenom(std::vector<double> genom)
{
    FILE *genomlogFile = fopen("genom.csv","a");
    for(unsigned int a=0; a<genom.size(); a++)
    {
        fprintf(genomlogFile,"%.5f;",genom[a]);
    }
    fprintf(genomlogFile,"\n");
    fclose(genomlogFile);
}
void Snake::on_backpropTraining_pushButton_clicked()
{
    loadBackpropTrainignsData(_backpropTrainingInputs,_backpropTrainingOutputs);
    std::vector<double> output;
    unsigned int trainingSteps = 0;
    double minimalError = (double)ui->maxError_slider->value()/1000;
    double averageError = 1;
    double lastAverageError;
    unsigned int saveInterval = 10;
    FILE *_logfile;

    std::vector<double>   averageErrorList;
    do{
        lastAverageError = averageError;
        averageError = 0;
        for(unsigned int a=0; a<_backpropTrainingInputs.size(); a++)
        {
            _backpropNet->set_input(_backpropTrainingInputs[a]);
            output = _backpropNet->get_output();
            _backpropNet->set_expected(_backpropTrainingOutputs[a]);

            averageError += abs(_backpropNet->get_netError());

            _backpropNet->learn();

        }

        trainingSteps++;
        averageError /= _backpropTrainingInputs.size();
        averageErrorList.push_back(averageError);
        if(averageErrorList.size() > 20)
        {
            averageErrorList.erase(averageErrorList.begin());
        }

        if(trainingSteps % saveInterval == 0)
        {
            _logfile = fopen("backpropScore.csv","a");           //Saves the error in the file: score.csv
            fprintf(_logfile,"%.5f;\n",averageError);            //
            fclose(_logfile);
#ifdef DEBUG_PRINTS
            qDebug() << "averageNetError: "<< averageError;
#endif
            _backpropNet->saveToNetFile();
            logGenom(_backpropNet->get_genom());
        }

      /*  _logfile = fopen("backpropScore.csv","a");           //Saves the error in the file: score.csv
        fprintf(_logfile,"%.5f;\n",averageError);            //
        fclose(_logfile);
        qDebug() << "averageNetError: "<< averageError;
        _backpropNet->saveToNetFile();*/


    }while(averageError > minimalError && trainingSteps < 10000 && abs(lastAverageError-averageError) > 0.001);
    _backpropNet->saveToNetFile();
#ifdef DEBUG_PRINTS
    qDebug() << "end Training, steps: "<<trainingSteps;
#endif
    _backpropTrainingInputs.clear();
    _backpropTrainingOutputs.clear();
}
void Snake::on_selectedSnake_slider_valueChanged(int value)
{
    _selectedSnake = value;
}

void Snake::modeReset()
{
    _botScore = 0;
    _botFood = 0;
    _botSteps = 0;
    _botDeaths = 0;
    _botKills = 0;

    _playerScore = 0;
    _playerFood = 0;
    _playerSteps = 0;
    _playerDeaths = 0;
    _playerKills = 0;

    _versusSaveScoreCount = 0;

    _environment->showInfoText(false);
    _versusEnvironment->showInfoText(false);
    _backpropTrainingEnvironment->showInfoText(false);

    _versusEnvironment->player(0)->resetScore();
    _versusEnvironment->player(0)->resetDeathCount();
    _versusEnvironment->player(1)->resetScore();
    _versusEnvironment->player(1)->resetDeathCount();


    if(_record_enable)
        savescreenshot();
    _step = 0;

}

void Snake::onSnakeKilled(unsigned int killer,unsigned int victim)
{
  //  qDebug() << "snake killed someone: "<<killer<<" : "<<victim;
    if(_modus == Modus::versusAI)
    {
        if(killer == 0){
            _playerKills++;
        }else {
            _botKills++;
        }
    }
  //  qDebug() << "botkills: "<<_botKills << " Playerkills: "<<_playerKills;
}

void Snake::setupFieldOfView()
{

   /*     for(int y=-5; y<=5; y++)
        {
            for(int x=-5; x<=5; x++)
            {
                _fieldOfView.push_back(QPoint(x,y));
            }
        }
*/
   /* for(int a=-5; a<=5; a++)
    {
        _fieldOfView.push_back(QPoint(a,0));
    }
    for(int a=1; a<=6; a++)
    {
        if(a == 0)
            continue;
        _fieldOfView.push_back(QPoint(0,a));
    }*/


   /* for(int a=-5; a<=5; a++)
    {
        if(a == 0)
            continue;
        _fieldOfView.push_back(QPoint(a,-a));
        _fieldOfView.push_back(QPoint(-a,a));
    }*/


}
void Snake::closeEvent(QCloseEvent *event)
{
    _updateTimer->stop();
    if(_modus == Modus::geneticTraining)
        on_saveStats_pushbutton_clicked();

    qDebug() << "exit";
    event->accept();

  //event->ignore();

}
QString Snake::GetLastErrorStr()
{
  DWORD error = GetLastError();
  if (error)
  {
    LPVOID lpMsgBuf;
    DWORD bufLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
    if (bufLen)
    {
      LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
      string result(lpMsgStr, lpMsgStr+bufLen);

      LocalFree(lpMsgBuf);

      return QString::fromStdString(result);
    }
  }
  return QString();
}

void Snake::saveError(QString error)
{
    FILE *file = fopen("error.txt","a");
    if(file)
    {
        fprintf(file,"%s\n",error.toStdString().c_str());
        fclose(file);
    }
    exit(-1);
}


void Snake::on_record_start_pushButton_clicked(bool checked)
{
    _record_enable = !_record_enable;
    if(_record_savePath != ui->record_fileName_lineEdit->text())
    {
        _record_savePath = ui->record_fileName_lineEdit->text();
        //_record_imageIndex = 0;
    }
    _record_generationInterval = (unsigned int)ui->record_generationInterval_spinbox->value();
    _record_stepInterval = (unsigned int)ui->record_stepInterval_spinbox->value();
    if(_record_enable)
    {
        on_record_clearBuffer_pushButton_clicked();
        ui->record_start_pushButton->setText("stop record");
    }
    else
    {
        ui->record_start_pushButton->setText("start record");
    }

}
void Snake::takescreenshot()
{

    if(generation%_record_generationInterval != 0 || _step % _record_stepInterval != 0)
        return;
    _record_imageList.push_back(this->grab());
    ui->record_imagesInBuffer_label->setText(QString::number(_record_imageList.size()));
}
void Snake::savescreenshot()
{
    if(_record_imageList.size() == 0)
        return;
    //if(_record_imageList.size() > 100)
    {
        if(_record_generationIndex != generation)
        {
            _record_imageIndex = 0;
            _record_generationIndex = generation;
        }
        QString folderPath = "generation_"+QString::number(_record_generationIndex);
        if(_record_savePath != "")
        {
            folderPath = _record_savePath +"\\"+ folderPath;
        }
        mkdir(folderPath.toStdString().c_str());
        for(unsigned int a=0; a<_record_imageList.size(); a++)
        {
            QString path = folderPath + "\\"+QString::number(_record_imageIndex*_record_stepInterval)+".png";
            qDebug() << "save image: "<<path;
            QFile file(path);
            file.open(QIODevice::WriteOnly);
            _record_imageList[a].save(&file, "PNG");
            _record_imageIndex++;
        }
        _record_imageList.clear();
        _record_imageList.reserve(1000);
    }
}

void Snake::on_record_clearBuffer_pushButton_clicked()
{
    _record_imageList.clear();
    _record_imageList.reserve(1000);
}


