#include "snake.h"
#include "ui_snake.h"
#include <QDebug>



Snake::Snake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);

    QString version = "00.00.03";
    QString datum   = "04.06.2019";


    QString infoLabelText  = "Snake AI © by Alex Krieg\n";
            infoLabelText += "               v"+version+"\n";
            infoLabelText += "               "+datum;

    ui->info_label->setText(infoLabelText);


    _statsFilename = "stats.csv";


    // setupFieldOfView();

    bool enableKillreward = false;
    _respawnAmount        = 3;
    //----------------NEURAL NET------------------

    unsigned int animals = 100;
    unsigned int inputs = /*_fieldOfView.size()*/3 * (3 + 2); //3 Sensor Layer (food,snake,obsticle)
    unsigned int hiddenX = 1;
    unsigned int hiddenY = 20;
    unsigned int outputs = 2;                       // left | right

    net = new GeneticNet(animals,inputs,hiddenX,hiddenY,outputs);
    net->bias(true);
    net->loadFromNetFile("snake","net");
    net->mutationFactor(0.05);
    net->mutationChangeWeight(0.1);

    _backpropNet = new BackpropNet(net->inputNeurons(),
                                   net->hiddenNeuronsX(),
                                   net->hiddenNeuronsY(),
                                   net->outputNeurons(),
                                   net->bias(),
                                   net->enableAverage(),
                                   net->activationFunction());
    _backpropNet->netFileName("backPropNet");
    _backpropNet->netFileEnding("net");
    ui->selectedSnake_slider->setRange(0,net->animals()-1);
    _selectedSnake = ui->selectedSnake_slider->value();

    generation = 0;

  /*  qDebug() << "fieldOfView";
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
    }*/


    _painter = new QPainter(this);
//#ifdef TESTMODE
    _versusEnvironment = new Environment(this,_painter,2);
    _versusEnvironment->mapsize(QSize(30,30));
    _versusEnvironment->tileSize(10);
    _versusEnvironment->tileSpace(2);
    _versusEnvironment->scale(3);
    _versusEnvironment->drawPos(QPoint(300,10));

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
//#else
    _environment = new Environment(this,_painter,net->animals());
    _environment->mapsize(QSize(100,100));
    _environment->tileSize(3);
    _environment->tileSpace(1);
    _environment->scale(2);
    _environment->drawPos(QPoint(300,10));

    _backpropTrainingEnvironment = new Environment(this,_painter,1);
    _backpropTrainingEnvironment->mapsize(QSize(30,30));
    _backpropTrainingEnvironment->tileSize(10);
    _backpropTrainingEnvironment->tileSpace(2);
    _backpropTrainingEnvironment->scale(3);
    _backpropTrainingEnvironment->drawPos(QPoint(300,10));

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
    _playerKeyInputDisabler = false;
    _modus = Modus::geneticTraining;


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
    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            _environment->update();
            break;
        }
        case Modus::versusAI:
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
            break;
        }
        case Modus::backpropTraining:
        {
            _backpropTrainingEnvironment->update();
            break;
        }
    }


    if(_updateTimer->interval() != ui->speed_slider->value())
    {
        _updateTimer->setInterval(500 - ui->speed_slider->value());
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
    this->update();
    handleNet();
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

    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            for(unsigned int animal=0; animal<net->animals(); animal++)
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
                _environment->controlSnakeDirection(animal,getDirectionFromData(output));
            }

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
                    if(animal == 0)
                    {
                        qDebug() << "score: food:"<<_environment->player(animal)->averageScore().food<< " steps: "<< _environment->player(animal)->averageScore().steps;
                        qDebug() << "-----------------";
                        for(int b=0; b<_environment->player(animal)->score().size(); b++)
                        {
                            qDebug() << "--: food:"<<_environment->player(animal)->score()[b].food<< " steps: "<< _environment->player(animal)->score()[b].steps;
                        }
                    }
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
                    averageScore        += _snakeScore[_snakeScore.size()-1] / net->animals();
                    averageFoodScore    += _environment->player(animal)->averageScore().food / net->animals();
                    averageStepScore    += (float)_environment->player(animal)->averageScore().steps / net->animals();
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
                    _environment->player(animal)->resetDeathCount();
                    _environment->player(animal)->resetScore();
                    _environment->player(animal)->revive();
                }
                _environment->obsticleReplace();
                _saveCounter++;
                if(_saveCounter > 20)
                {
                    _saveCounter = 0;
                    on_saveStats_pushbutton_clicked();
                }
               // on_saveStats_pushbutton_clicked();
            }
            break;
        }
        case Modus::versusAI:
        {
            _versusEnvironment->AI_mapData_simple(0);
            vector<vector<float>    > netInput = _versusEnvironment->AI_mapData_simple(1);
            unsigned int inputIndex = 0;
            for(unsigned int b=0; b<netInput.size(); b++)
            {
                for(unsigned int c=0; c<netInput[b].size(); c++)
                {
                    net->input(_selectedSnake,inputIndex,netInput[b][c]);
                    inputIndex++;
                }
            }
            net->run(_selectedSnake);

            vector<float> output = net->output(_selectedSnake);
            if(output.size() != 2)
            {
                qDebug() << "error: net outputSize wrong: "<< output.size();
            }
            else
            {
                /*if(output[0] > 0.2f && output[1] > 0.2f)
                {
                    if(output[0] > output[1])
                    {
                        _versusEnvironment->controlSnakeDirection(1,Direction::_left);
                    }else
                    {
                        _versusEnvironment->controlSnakeDirection(1,Direction::_right);
                    }
                }*/
                _versusEnvironment->controlSnakeDirection(1,getDirectionFromData(output));
            }

            if(!_versusEnvironment->player(0)->isAlive() && !_versusEnvironment->player(1)->isAlive())
            {
                _versusEnvironment->obsticleReplace();
            }
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
            break;
        }
        case Modus::backpropTraining:
        {
            //_versusEnvironment->AI_mapData_simple(0);
            vector<vector<float>    > netInput = _backpropTrainingEnvironment->AI_mapData_simple(0);
            vector<float> inputVec;
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
            _backpropTrainingOutputs.push_back(vector<float>(2,0));

            net->input(_selectedSnake,inputVec);
            net->run(_selectedSnake);
            _backpropTrainingOutputs[_backpropTrainingOutputs.size()-1] = net->output(_selectedSnake);
            _backpropTrainingEnvironment->controlSnakeDirection(0,getDirectionFromData(_backpropTrainingOutputs[_backpropTrainingOutputs.size()-1]));
            //net->run(0);

            //vector<float> output = net->output(0);
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

                _playerFood += _backpropTrainingEnvironment->player(0)->food();
                _playerSteps += _backpropTrainingEnvironment->player(0)->steps();
                _playerFood/=2;
                _playerSteps/=2;
                _playerDeaths++;

                _playerScore += _playerFood * scoreFoodFactor + _playerSteps * scoreStepFactor;
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
        qDebug() << "saveBackpropTrainingsData no data to save";
    }
    if(_backpropTrainingInputs.size() != _backpropTrainingOutputs.size())
    {
        qDebug() << "ERROR: saveBackpropTrainingsData size != size" << _backpropTrainingInputs.size() << " != " << _backpropTrainingOutputs.size();
    }
    //_backpropTrainingInputs.erase(_backpropTrainingInputs.begin());
    FILE *_file = fopen(_backprobTrainingsDataFileName.c_str(),"a");
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
void Snake::loadBackpropTrainignsData(vector<vector<float> > &inputs,vector<vector<float>  > &outputs)
{
    FILE *_file = fopen(_backprobTrainingsDataFileName.c_str(),"r");
    if(!_file)
    {
        qDebug() << "File: "<<_backprobTrainingsDataFileName.c_str() << " not found";
        return;
    }
    vector<string> fileBuffer;
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
        vector<float> tmpInput;
        vector<float> tmpOutput;

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
        qDebug() << tmpInput << tmpOutput;
        inputs.push_back(tmpInput);
        outputs.push_back(tmpOutput);
    }

    fclose(_file);
}
Direction Snake::getDirectionFromData(vector<float> inputs)
{
    if(inputs[0] > 0.2f || inputs[1] > 0.2f)
    {
        if(inputs[0] > inputs[1])
        {
            return Direction::_left;
        }else
        {
            return Direction::_right;
        }
    }
    return Direction::_up;
}
float Snake::getScore(Player *player)
{
    if(player == nullptr)
        return 0.f;
    float scoreFoodFactor = 0.2f;
    float scoreStepFactor = 0.1f;
    struct Score playerScore = player->averageScore();
    return playerScore.food * scoreFoodFactor + playerScore.food * scoreStepFactor;
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
                    vector<float> tmpOutput;
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
                    vector<float> tmpOutput;
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
                    vector<float> tmpOutput;
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
                    vector<float> tmpOutput;
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
    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            for(unsigned int a=0; a<net->animals(); a++)
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
        return;
    }

    switch(_modus)
    {
        case Modus::geneticTraining:
        {
            _environment->showInfoText(ui->mapinfo_checkbox->isChecked());
            _versusEnvironment->showInfoText(false);
            _backpropTrainingEnvironment->showInfoText(false);
            break;
        }
        case Modus::versusAI:
        {
            _environment->showInfoText(false);
            _backpropTrainingEnvironment->showInfoText(false);
            _versusEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
            break;
        }
        case Modus::backpropTraining:
        {
            _environment->showInfoText(false);
            _backpropTrainingEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
            _versusEnvironment->showInfoText(false);
            break;
        }
    }
}
void Snake::on_versusBot_pushButton_clicked()
{
    /*

    _versusEnvironment->controlSnakeDeath(0,true);
    _versusEnvironment->controlSnakeDeath(1,true);

    if(_versusBot && _enableDisplay)
    {
        _environment->showInfoText(false);
        _versusEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
    }
    else
    {
        _environment->showInfoText(ui->mapinfo_checkbox->isChecked());
        _versusEnvironment->showInfoText(false);
    }

    _botScore = 0;
    _botFood = 0;
    _botSteps = 0;
    _botDeaths = 0;
    _botKills = 0;

    _playerScore = 0;
    _playerFood = 0;
    _playerSteps = 0;
    _playerDeaths = 0;
    _playerKills = 0;*/
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
    _versusEnvironment->controlSnakeDeath(0,true);
    _versusEnvironment->controlSnakeDeath(1,true);

    modeReset();
    _versusEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
}
void Snake::on_backpropTraining_radioButton_clicked(bool checked)
{
    _modus = Modus::backpropTraining;
    modeReset();
    _backpropTrainingEnvironment->showInfoText(ui->mapinfo_checkbox->isChecked());
}
void Snake::logGenom(vector<float> genom)
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
    vector<float> output;
    unsigned int trainingSteps = 0;
    float minimalError = (float)ui->maxError_slider->value()/1000;
    float averageError = 1;
    float lastAverageError;
    unsigned int saveInterval = 10;
    FILE *_logfile;

    vector<float>   averageErrorList;
    do{
        lastAverageError = averageError;
        averageError = 0;
        for(unsigned int a=0; a<_backpropTrainingInputs.size(); a++)
        {
            _backpropNet->input(_backpropTrainingInputs[a]);
            output = _backpropNet->output();
            _backpropNet->expected(_backpropTrainingOutputs[a]);

            averageError += abs(_backpropNet->netError());

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
            qDebug() << "averageNetError: "<< averageError;
            _backpropNet->saveToNetFile();
            logGenom(_backpropNet->genom());
        }

      /*  _logfile = fopen("backpropScore.csv","a");           //Saves the error in the file: score.csv
        fprintf(_logfile,"%.5f;\n",averageError);            //
        fclose(_logfile);
        qDebug() << "averageNetError: "<< averageError;
        _backpropNet->saveToNetFile();*/


    }while(averageError > minimalError && trainingSteps < 10000 && abs(lastAverageError-averageError) > 0.001);
    _backpropNet->saveToNetFile();
    qDebug() << "end Training, steps: "<<trainingSteps;
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

    _environment->showInfoText(false);
    _versusEnvironment->showInfoText(false);
    _backpropTrainingEnvironment->showInfoText(false);
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












