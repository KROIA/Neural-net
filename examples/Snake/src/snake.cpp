#include "snake.h"
#include "ui_snake.h"
#include <QDebug>



Snake::Snake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);

    QString version = "00.00.01";
    QString datum   = "29.05.2019";


    QString infoLabelText  = "Snake AI © by Alex Krieg\n";
            infoLabelText += "               v"+version+"\n";
            infoLabelText += "               "+datum;

    ui->info_label->setText(infoLabelText);


    _statsFilename = "stats.csv";

    setupFieldOfView();
    //----------------NEURAL NET------------------

    unsigned int animals = 100;
    unsigned int inputs = /*_fieldOfView.size()*/3 * 3 ; //3 Sensor Layer (food,snake,obsticle)
    unsigned int hiddenX = 2;
    unsigned int hiddenY = 16;
    unsigned int outputs = 2;                       // left | right

    net = new GeneticNet(animals,inputs,hiddenX,hiddenY,outputs);
    net->bias(true);
    net->loadFromNetFile("snake","net");
    net->mutationFactor(0.05);
    net->mutationChangeWeight(0.1);
    generation = 0;

    qDebug() << "fieldOfView";
    int c = 0;
    for(unsigned int a=0; a<sqrt(_fieldOfView.size()); a++)
    {
        QString txt = "";
        for(unsigned int b=0; b<sqrt(_fieldOfView.size()); b++)
        {
            txt+=QString::number(_fieldOfView[c].x())+"|"+QString::number(_fieldOfView[c].y())+"  ";
            c++;
        }
        qDebug() << txt;
    }


    _painter = new QPainter(this);
//#ifdef TESTMODE
    _versusEnvironment = new Environment(_painter,2);
    _versusEnvironment->mapsize(QSize(30,30));
    _versusEnvironment->tileSize(10);
    _versusEnvironment->tileSpace(2);
    _versusEnvironment->scale(3);
    _versusEnvironment->drawPos(QPoint(200,10));

    _versusEnvironment->player(0)->globalView(true);
    _versusEnvironment->player(0)->standardColor(QColor(0,100,200));
    _versusEnvironment->player(1)->standardColor(QColor(0,255,100));
    connect(_versusEnvironment,SIGNAL(playerKill(unsigned int,unsigned int)),this,SLOT(onSnakeKilled(unsigned int, unsigned int)));

    _versusBot = false;

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
//#else
    _environment = new Environment(_painter,net->animals());
    _environment->mapsize(QSize(100,100));
    _environment->tileSize(3);
    _environment->tileSpace(1);
    _environment->scale(2);
    _environment->drawPos(QPoint(200,10));
//#endif



    _environment->mapInit();
    _versusEnvironment->mapInit();


    _selfControl = false;
    _pause = false;
    _enableDisplay = true;

    _updateTimer = new QTimer(this);
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    _updateTimer->start(200);

    _updateTimer2 = new QTimer(this);
    connect(_updateTimer2,SIGNAL(timeout()),this,SLOT(timerEvent2()));
    _updateTimer2->start(3000);
    _calculationPerSecond   = 0;
    _calculationCounter     = 0;


}


Snake::~Snake()
{
    delete ui;
}
void Snake::paintEvent(QPaintEvent *e)
{
    if(!_enableDisplay)
        return;
    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);
    _painter->setRenderHints(QPainter::RenderHints(0x04),true);

    if(_versusBot)
    {
        _versusEnvironment->drawMap();
    }
    else
    {
        _environment->drawMap();
    }
    _painter->end();
}
void Snake::timerEvent()
{
    if(_versusBot)
    {
        _versusEnvironment->update();

        ui->playerFood_label->setText(QString::number(_playerFood+_versusEnvironment->player(0)->food()));
        ui->playerSteps_label->setText(QString::number(_playerSteps+_versusEnvironment->player(0)->steps()));
        ui->playerDeaths_label->setText(QString::number(_playerDeaths));
        ui->playerKills_label->setText(QString::number(_playerKills));
        ui->playerScore_label->setText(QString::number(_playerScore));

        ui->botFood_label->setText(QString::number(_botFood+_versusEnvironment->player(1)->food()));
        ui->botSteps_label->setText(QString::number(_botSteps+_versusEnvironment->player(1)->steps()));
        ui->botDeaths_label->setText(QString::number(_botDeaths));
        ui->botKills_label->setText(QString::number(_botKills));
        ui->botScore_label->setText(QString::number(_botScore));

    }else {
        _environment->update();
    }


    if(_updateTimer->interval() != ui->speed_slider->value())
    {
        _updateTimer->setInterval(500 - ui->speed_slider->value());
    }
    if(_environment->foodAmount() != ui->food_slider->value())
    {
        _environment->foodAmount(ui->food_slider->value());
        _versusEnvironment->foodAmount(ui->food_slider->value());
    }
    ui->score_label->setText(QString::number(_averageScore_smoth));

    ui->food_label->setText(QString::number(_environment->player(0)->food()));
    ui->steps_label->setText(QString::number(generation));
    this->update();
    handleNet();


}
void Snake::timerEvent2()
{
        _calculationPerSecond =(float) /*_calculationPerSecond*0.9*/ + 1000*_calculationCounter/(float)_updateTimer2->interval();
        ui->genPerSec_label->setText(QString::fromStdString(to_string(_calculationPerSecond)));
        _calculationCounter = 0;
}

void Snake::handleNet()
{
    //qDebug() << "handle Net";
    unsigned int deathCounter = 0;

    float scoreFoodFactor = 0.2f;
    float scoreStepFactor = 0.1f;

//#ifdef TESTMODE
    if(_versusBot)
    {
        vector<vector<float>    > netInput = _versusEnvironment->AI_mapData_simple(1);
        unsigned int inputIndex = 0;
        for(unsigned int b=0; b<netInput.size(); b++)
        {
            for(unsigned int c=0; c<netInput[b].size(); c++)
            {
                net->input(0,inputIndex,netInput[b][c]);
                inputIndex++;
            }
        }
        net->run(0);
    }
    else
    {
    //#else

        for(unsigned int animal=0; animal<net->animals(); animal++)
        {
            if(!_environment->player(animal)->isAlive())
            {
                deathCounter++;
                continue;
            }
            //vector<vector<float>    > netInput = _environment->AI_mapData(animal,_fieldOfView);
            vector<vector<float>    > netInput = _environment->AI_mapData_simple(animal);
            unsigned int inputIndex = 0;
            for(unsigned int b=0; b<netInput.size(); b++)
            {
                for(unsigned int c=0; c<netInput[b].size(); c++)
                {
                    net->input(animal,inputIndex,netInput[b][c]);
                    inputIndex++;
                }
            }
            net->run(animal);
        }
   // #endif
    }
    if(_versusBot)
    {
  //  #ifdef TESTMODE
        vector<float> output = net->output(0);
        if(output.size() != 2)
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
        }
    }
    else
    {
   // #else
        for(unsigned int animal=0; animal<net->animals(); animal++)
        {
            if(!_environment->player(animal)->isAlive())
            {
                continue;
            }
            vector<float> output = net->output(animal);
            if(output.size() != 2)
            {
                qDebug() << "error: net outputSize wrong: "<< output.size();
                continue;
            }

            //----------------Control the Snakes-------------------
           /* if(animal == 0 && _selfControl)
            {
                qDebug() << "continue";
                continue;
            }*/
            if(output[0] > 0.2f && output[1] > 0.2f)
            {
                if(output[0] > output[1])
                {
                    _environment->controlSnakeDirection(animal,Direction::_left);
                }else
                {
                    _environment->controlSnakeDirection(animal,Direction::_right);
                }
            }
        }
   // #endif
    }
       // qDebug() << "alive: " << net->animals()-deathCounter << " of: " << net->animals();
    if(_versusBot)
    {
   // #ifdef TESTMODE
        if(!_versusEnvironment->player(0)->isAlive())
        {
            _playerFood += _versusEnvironment->player(0)->food();
            _playerSteps += _versusEnvironment->player(0)->steps();
            _playerFood/=2;
            _playerSteps/=2;
            _playerDeaths++;

            _playerScore += _playerFood * scoreFoodFactor + _playerSteps * scoreStepFactor;
            _playerScore/=2;


            _versusEnvironment->player(0)->revive();
        }
        if(!_versusEnvironment->player(1)->isAlive())// bot
        {
            _botFood += _versusEnvironment->player(0)->food();
            _botSteps += _versusEnvironment->player(0)->steps();
            _botFood/=2;
            _botSteps/=2;
            _botDeaths++;

            _botScore += _playerFood * scoreFoodFactor + _playerSteps * scoreStepFactor;
            _botScore/=2;


            _versusEnvironment->player(1)->revive();
        }
    }
    else
    {
   // #else
        if(deathCounter == net->animals())
        {
            qDebug() << "all death";
            _snakeScore.clear();
            float averageScore = 0;
            float maxScore = 0;
            float minScore = 0;
            float averageFoodScore = 0;
            float averageStepScore = 0;
            for(unsigned int animal=0; animal<net->animals(); animal++)
            {
                float food = _environment->player(animal)->food();
                float steps = _environment->player(animal)->steps();

                float score = food * scoreFoodFactor + steps * scoreStepFactor;

                _snakeScore.push_back(score);
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
                averageScore        += _snakeScore[_snakeScore.size()-1]/= net->animals();
                averageFoodScore    += food/= net->animals();
                averageStepScore    += steps/= net->animals();
            }

            qDebug() << "average score: "<<averageScore;

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

            qDebug() << "learn";
            net->learn(_snakeScore);

            generation++;
            _calculationCounter++;

            for(unsigned int animal=0; animal<net->animals(); animal++)
            {
                _environment->player(animal)->revive();
            }
            _saveCounter++;
            if(_saveCounter > 20)
            {
                _saveCounter = 0;
                on_saveStats_pushbutton_clicked();
            }
           // on_saveStats_pushbutton_clicked();
        }
    //#endif
    }
}

void Snake::keyPressEvent(QKeyEvent *e)
{
    switch(char(e->key()))
    {
        //CONTROL 1
        case 'W'://up
        {
            //_selfControl = !_selfControl;
            _versusEnvironment->controlSnakeDirection(0,Direction::_up);
            break;
        }
        case 'A'://left
        {
            _versusEnvironment->controlSnakeDirection(0,Direction::_left);
            break;
        }
        case 'S'://down
        {
            _versusEnvironment->controlSnakeDirection(0,Direction::_down);
            break;
        }
        case 'D'://right
        {
            _versusEnvironment->controlSnakeDirection(0,Direction::_right);
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
        _updateTimer->start();
}
void Snake::on_saveStats_pushbutton_clicked()
{
    qDebug() << "save";
    net->saveToNetFile();
    bool fileExists = false;
    FILE *statsFile = fopen(_statsFilename.c_str(),"r");
    if(statsFile)
    {
        fileExists = true;
        fclose(statsFile);
    }
    statsFile = fopen(_statsFilename.c_str(),"a");
    if(!statsFile)
        return;

    if(!fileExists)
    {
        fprintf(statsFile,"minScore;maxScore;foodScoreAverage;stepScoreAverage;;minScore_smoth;maxScore_smoth;foodScoreAverage_smoth;stepScoreAverage_smoth;;average;average_smoth;\n");
    }
    for(unsigned int a=0; a<_averageScoreList.size(); a++)
    {
        fprintf(statsFile,"%.8f;%.8f;%.8f;%.8f;;%.8f;%.8f;%.8f;%.8f;;%.8f;%.8f;\n",_minScoreList[a],_maxScoreList[a],_foodScore[a],_stepScore[a],_minScoreList_smoth[a],_maxScoreList_smoth[a],_foodScore_smoth[a],_stepScore_smoth[a],_averageScoreList[a],_averageScoreList_smoth[a]);
    }
    fclose(statsFile);
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
}
void Snake::on_kill_pushButton_clicked()
{
    if(_versusBot)
    {
        for(unsigned int a=0; a<_versusEnvironment->playerAmount(); a++)
        {
            _versusEnvironment->player(a)->kill();
        }
    }else {
        for(unsigned int a=0; a<net->animals(); a++)
        {
            _environment->player(a)->kill();
        }
    }
}
void Snake::on_toggleDisplay_pushbutton_clicked()
{
    _enableDisplay = !_enableDisplay;
}
void Snake::on_versusBot_pushButton_clicked()
{
    _versusBot = !_versusBot;

    _versusEnvironment->controlSnakeDeath(0,true);
    _versusEnvironment->controlSnakeDeath(1,true);

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
}
void Snake::onSnakeKilled(unsigned int killer,unsigned int victim)
{
    qDebug() << "snake killed someone: "<<killer<<" : "<<victim;
    if(_versusBot)
    {
        if(killer == 0){
            _playerKills++;
        }else {
            _botKills++;
        }
    }
    qDebug() << "botkills: "<<_botKills << " Playerkills: "<<_playerKills;
}

void Snake::setupFieldOfView()
{

        for(int y=-5; y<=5; y++)
        {
            for(int x=-5; x<=5; x++)
            {
                _fieldOfView.push_back(QPoint(x,y));
            }
        }

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
    if(!_versusBot)
        on_saveStats_pushbutton_clicked();

    qDebug() << "exit";
    event->accept();

  //event->ignore();

}




