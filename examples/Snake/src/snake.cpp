#include "snake.h"
#include "ui_snake.h"
#include <QDebug>
Snake::Snake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);

    QString version = "00.00.00";
    QString datum   = "29.05.2019";


    QString infoLabelText  = "Snake AI © by Alex Krieg\n";
            infoLabelText += "               v"+version+"\n";
            infoLabelText += "               "+datum;

    ui->info_label->setText(infoLabelText);


    _statsFilename = "stats.csv";

    setupFieldOfView();
    //----------------NEURAL NET------------------

    unsigned int animals = 100;
    unsigned int inputs = _fieldOfView.size() * 3 ; //3 Sensor Layer (food,snake,obsticle)
    unsigned int hiddenX = 2;
    unsigned int hiddenY = 25;
    unsigned int outputs = 2;                       // left | right

    net = new GeneticNet(animals,inputs,hiddenX,hiddenY,outputs);
    net->bias(true);
    net->loadFromNetFile("snake","net");
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
    _environment = new Environment(_painter,net->animals());
    _environment->mapsize(QSize(50,50));
    _environment->tileSize(5);
    _environment->tileSpace(1);
    _environment->drawPos(QPoint(200,10));

    _environment->mapInit();


    _selfControl = false;
    _pause = false;
    _enableDisplay = true;

    _updateTimer = new QTimer(this);
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    _updateTimer->start(200);

    _updateTimer2 = new QTimer(this);
    connect(_updateTimer2,SIGNAL(timeout()),this,SLOT(timerEvent2()));
    _updateTimer2->start(1000);
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

    _environment->drawMap();

    _painter->end();
}
void Snake::timerEvent()
{
    _environment->update();
    if(_updateTimer->interval() != ui->speed_slider->value())
    {
        _updateTimer->setInterval(500 - ui->speed_slider->value());
    }
    if(_environment->foodAmount() != ui->food_slider->value())
    {
        _environment->foodAmount(ui->food_slider->value());
    }
    if(_averageScoreList.size() != 0)
        ui->score_label->setText(QString::number(_averageScoreList[_averageScoreList.size()-1]));

    ui->food_label->setText(QString::number(_environment->player(0)->food()));
    ui->steps_label->setText(QString::number(_environment->player(0)->steps()));
    this->update();
    handleNet();

}
void Snake::timerEvent2()
{
        _calculationPerSecond =(float) _calculationPerSecond*0.9 + _calculationCounter/(float)_updateTimer2->interval();
        ui->genPerSec_label->setText(QString::fromStdString(to_string(_calculationPerSecond)));
        _calculationCounter = 0;
}

void Snake::handleNet()
{
    //qDebug() << "handle Net";
    unsigned int deathCounter = 0;
    for(unsigned int animal=0; animal<net->animals(); animal++)
    {
        if(!_environment->player(animal)->isAlive())
        {
            deathCounter++;
            continue;
        }
        vector<vector<float>    > netInput = _environment->AI_mapData(animal,_fieldOfView);
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
        if(animal == 0 && _selfControl)
        {
            continue;
        }
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

   // qDebug() << "alive: " << net->animals()-deathCounter << " of: " << net->animals();
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

            float score = food * 0.2f + pow(steps * 0.1f,2);

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


        qDebug() << "learn";
        net->learn(_snakeScore);
        qDebug() << "save";
        net->saveToNetFile();
        generation++;
        _calculationCounter++;

        for(unsigned int animal=0; animal<net->animals(); animal++)
        {
            _environment->player(animal)->revive();
        }
    }
}

void Snake::keyPressEvent(QKeyEvent *e)
{
    switch(char(e->key()))
    {
        //CONTROL 1
        case 'W'://up
        {
            _selfControl = !_selfControl;
            //_environment->controlSnakeDirection(0,Direction::_up);
            break;
        }
        case 'A'://left
        {
            _environment->controlSnakeDirection(0,Direction::_left);
            break;
        }
        case 'S'://down
        {
            _environment->controlSnakeDirection(0,Direction::_down);
            break;
        }
        case 'D'://right
        {
            _environment->controlSnakeDirection(0,Direction::_right);
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
        fprintf(statsFile,"minScore;maxScore;foodScoreAverage;stepScoreAverage;average;\n");
    }
    for(unsigned int a=0; a<_averageScoreList.size(); a++)
    {
        fprintf(statsFile,"%.8f;%.8f;%.8f;%.8f;%.8f;\n",_minScoreList[a],_maxScoreList[a],_foodScore[a],_stepScore[a],_averageScoreList[a]);
    }
    fclose(statsFile);
    _minScoreList.clear();
    _maxScoreList.clear();
    _foodScore.clear();
    _stepScore.clear();
    _averageScoreList.clear();
}
void Snake::on_kill_pushButton_clicked()
{
    for(unsigned int a=0; a<net->animals(); a++)
    {
        _environment->player(a)->kill();
    }
}
void Snake::on_toggleDisplay_pushbutton_clicked()
{
    _enableDisplay = !_enableDisplay;
}

void Snake::setupFieldOfView()
{
    /*
        for(int y=-2; y<=2; y++)
        {
            for(int x=-2; x<=2; x++)
            {
                _fieldOfView.push_back(QPoint(x,y));
            }
        }
    */
    for(int a=-5; a<=5; a++)
    {
        _fieldOfView.push_back(QPoint(a,0));
    }
    for(int a=1; a<=5; a++)
    {
        if(a == 0)
            continue;
        _fieldOfView.push_back(QPoint(0,a));
    }
   /* for(int a=-5; a<=5; a++)
    {
        if(a == 0)
            continue;
        _fieldOfView.push_back(QPoint(a,-a));
        _fieldOfView.push_back(QPoint(-a,a));
    }*/


}




