#include "enviroment.h"


Enviroment::Enviroment()
{
    _enviromentSize     = Coord({40,40});
    _foodAmount         = 100;
    _poisonAmount       = 50;
    _boundrySize        = 3;

    unsigned int animals = 100;
    unsigned int inputs  = 9;
    unsigned int hiddenX = 1;
    unsigned int hiddenY = 8;
    unsigned int outputs = 4;
    bool         bias    = true;
    bool   enableAverage = false;
    Activation  func     = Activation::Sigmoid;

    _animalStartFood     = 10;
    _maxTicks            = 10000;
    _netSaveInterval     = 1;
    _cycles              = 3;
#define drawMap

    _enviromentSize = Coord({_enviromentSize.x+_boundrySize*2,_enviromentSize.y+_boundrySize*2});
    _map = vector<vector<int>   >(_enviromentSize.x,vector<int>(_enviromentSize.y,NOTHING));
    _animalMap = vector<vector<int> >(_enviromentSize.x,vector<int>(_enviromentSize.y,NOTHING));
    _net = new GeneticNet(animals,inputs,hiddenX,hiddenY,outputs,bias,enableAverage,func);
    try {
        _net->loadFromNetFile("netFile");
    } catch (runtime_error &e) {
        qDebug() << e.what();
        _net->set(animals,inputs,hiddenX,hiddenY,outputs,bias,enableAverage,func);
    }
    _net->set_mutationChangeWeight(0.1);
    _net->set_mutationFactor(0.1);
    _net->updateNetConfiguration();
//    _net->set(animals,inputs,hiddenX,hiddenY,outputs,bias,enableAverage,func);

    _net->saveToNetFile();


    _averageScore = 0;
    _cyclesCounter = 0;
    _netSaveTicks = 0;
    restart();
   // getchar();
}
void Enviroment::restart()
{
    _foodPositionList   = vector<Coord> (_foodAmount    ,Coord({0,0}));
    _poisonPositionList = vector<Coord> (_poisonAmount  ,Coord({0,0}));
    _animalPositionList = vector<Coord> (_net->get_animals(),Coord({0,0}));

    _animalBackCountList = vector<int>(_net->get_animals(),0);
    _animalCurrentFoodList = vector<int>(_net->get_animals(),_animalStartFood);
    _animalRotationList = vector<unsigned short>(_net->get_animals(),0);
    _animalAliveList = vector<bool> (_net->get_animals(),true);
    _animalScoreList = vector<double> (_net->get_animals(),0);
    _ticks = 0;

    _map = vector<vector<int>   >(_enviromentSize.x,vector<int>(_enviromentSize.y,NOTHING));
    _animalMap = vector<vector<int>   >(_enviromentSize.x,vector<int>(_enviromentSize.y,NOTHING));


    _boundaryPositionList = vector<struct Coord>(2*_enviromentSize.x*_boundrySize+(_enviromentSize.y-2*_boundrySize)*_boundrySize*2);
//    qDebug() << _boundaryPositionList.size();
    unsigned int c=0;
    for(int x=0; x<_enviromentSize.x; x++)
    {
        for(int y=0; y<_enviromentSize.y; y++)
        {
            if(x<_boundrySize || x > _enviromentSize.x - _boundrySize-1)
            {
                _boundaryPositionList[c] = Coord({x,y});
                _map[x][y] = OBSTICLE;
                c++;
            }else if(y<_boundrySize || y > _enviromentSize.y - _boundrySize-1)
            {
                _boundaryPositionList[c] = Coord({x,y});
                _map[x][y] = OBSTICLE;
                c++;
            }

        }
    }
    for(unsigned int a=0; a<10; a++)
    {
        Coord pos = getRandomPos();
        _map[pos.x][pos.y] = OBSTICLE;
        _boundaryPositionList.push_back(pos);
    }

    randomFoodPos();
    randomPoisonPos();
    randomAnimalPos();
    return;
}
void Enviroment::setup()
{

}
void Enviroment::cmdXY(int x,int y)
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(hConsole_c,pos);
}
void Enviroment::draw()
{
    cmdXY(0,0);

#ifdef drawMap
    vector<string   > display;
    for(int y=0; y<_enviromentSize.y; y++)
    {
        display.push_back("");
        for(int x=0; x<_enviromentSize.x; x++)
        {

            int obj = _animalMap[x][y];
            if(obj == NOTHING)
            {
                obj = checkCollision(Coord({x,y}));
            }
            switch(obj)
            {
                case NOTHING: //nothing
                {
                 //   printf(" ");
                    display[y]+=" ";
                    break;
                }
                case ANIMAL: //animal
                {
                    display[y]+= "O";
                  //  printf("O");
                    break;
                }
                case FOOD: //food
                {
                    display[y]+= "F";
                    //printf("F");
                    break;
                }
                case POISON: //poison
                {
                    display[y]+= "P";
                   // printf("P");
                    break;
                }
                case OBSTICLE: //boundry
                {
                    display[y]+= "H";
                    //printf("H");
                    break;
                }
            }
        }
        display[y]+="\n";
    }
    for(unsigned int a=0; a<display.size(); a++)
    {
        printf(display[a].c_str());
    }
    printf("\n");
#endif

    printf("Average score: %f  \n",_averageScore);
    printf("Still alive:   %i  \n",_stillAlive);
}
void Enviroment::tick()
{
    setInputs();
    _net->run();
    walk();

    _stillAlive = 0;
    for(unsigned int a=0; a<_net->get_animals(); a++)
    {
        if(_animalAliveList[a] == true)
        {
            _stillAlive++;
            _animalScoreList[a]++;

        }
    }
    _ticks++;
    _netSaveTicks++;

    if(_ticks >= _maxTicks || _stillAlive == 0)
    {

        vector<double>   tmpScore = _animalScoreList;



        //getchar();

        restart();
        if(_cycles <= _cyclesCounter)
        {
            _cycles++;
            _animalScoreList = tmpScore;
        }
        else
        {
            _net->learn(tmpScore);
            //logGenom();
            _cycles = 0;
            _averageScore = 0;
            for(unsigned int a=0; a<tmpScore.size(); a++)
            {
                tmpScore[a] += (double)_animalCurrentFoodList[a];
                _averageScore+=tmpScore[a];
            }
            _averageScore /= (double)_net->get_animals();
            if(_netSaveTicks >= _netSaveInterval)
            {
                _netSaveTicks = 0;
                _net->saveToNetFile();
            }
            _file = fopen("score.csv","a");
            if(_file)
            {
                fprintf(_file,"%f;\n",_averageScore);
                fclose(_file);
            }
        }
    }
}

struct Coord Enviroment::getRandomPos()
{
    struct Coord position = Coord({0,0});
    unsigned int retCounter = 0;
    do {
        position = Coord({rand()%_enviromentSize.x,rand()%_enviromentSize.y});
        //retCounter++;
    }while(retCounter < 100 && checkCollision(position) != NOTHING);
    if(position.x == 0 && position.y == 0)
    {
        return position;
    }
    return position;
}
void Enviroment::randomFoodPos()
{
    for(unsigned int a=0; a<_foodPositionList.size(); a++)
    {
        Coord pos = getRandomPos();
      //  _map[_foodPositionList[a].x][_foodPositionList[a].y] = NOTHING;
        _map[pos.x][pos.y] = FOOD;
        _foodPositionList[a] = pos;
    }
}
void Enviroment::randomPoisonPos()
{
    for(unsigned int a=0; a<_poisonPositionList.size(); a++)
    {
        Coord pos = getRandomPos();
      //  _map[_poisonPositionList[a].x][_poisonPositionList[a].y] = NOTHING;
        _map[pos.x][pos.y] = POISON;
        _poisonPositionList[a] = pos;
    }
}
void Enviroment::randomAnimalPos()
{
    for(unsigned int a=0; a<_animalPositionList.size(); a++)
    {
        Coord pos = getRandomPos();
     //   _animalMap[_animalPositionList[a].x][_animalPositionList[a].y] = NOTHING;
        _animalMap[pos.x][pos.y] = ANIMAL;
        _animalPositionList[a] = pos;
    }
}
int  Enviroment::checkCollision(struct Coord position)
{
    return _map[position.x][position.y];
    /*for(unsigned int a=0; a<_animalPositionList.size(); a++)
    {
        if(_animalPositionList[a].x == position.x && _animalPositionList[a].y == position.y)
        {
            return 1;
        }
    }
    for(unsigned int a=0; a<_foodPositionList.size(); a++)
    {
        if(_foodPositionList[a].x == position.x && _foodPositionList[a].y == position.y)
        {
            return 2;
        }
    }
    for(unsigned int a=0; a<_poisonPositionList.size(); a++)
    {
        if(_poisonPositionList[a].x == position.x && _poisonPositionList[a].y == position.y)
        {
            return 3;
        }
    }
    for(unsigned int a=0; a<_boundaryPositionList.size(); a++)
    {
        if(_boundaryPositionList[a].x == position.x && _boundaryPositionList[a].y == position.y)
        {
            return 4;
        }
    }*/

   // return 0;
}
int  Enviroment::checkAnimalCollision(unsigned int animal)
{
    for(unsigned int a=0; a<_animalPositionList.size(); a++)
    {
        if(a == animal)
        {
            continue;
        }
        if(_animalPositionList[a].x == _animalPositionList[animal].x && _animalPositionList[a].y == _animalPositionList[animal].y)
        {
            return ANIMAL; //Animal
        }
    }
    return _map[_animalPositionList[animal].x][_animalPositionList[animal].y];
   /* for(unsigned int a=0; a<_animalPositionList.size(); a++)
    {
        if(a == animal)
        {
            continue;
        }
        if(_animalPositionList[a].x == _animalPositionList[animal].x && _animalPositionList[a].y == _animalPositionList[animal].y)
        {
            return 1; //Animal
        }
    }
    for(unsigned int a=0; a<_foodPositionList.size(); a++)
    {
        if(_foodPositionList[a].x == _animalPositionList[animal].x && _foodPositionList[a].y == _animalPositionList[animal].y)
        {
            return 2; //Food
        }
    }
    for(unsigned int a=0; a<_poisonPositionList.size(); a++)
    {
        if(_poisonPositionList[a].x == _animalPositionList[animal].x && _poisonPositionList[a].y == _animalPositionList[animal].y)
        {
            return 3; //Poison
        }
    }
    for(unsigned int a=0; a<_boundaryPositionList.size(); a++)
    {
        if(_boundaryPositionList[a].x == _animalPositionList[animal].x && _boundaryPositionList[a].y == _animalPositionList[animal].y)
        {
            return 4; //Boundary
        }
    }
    return 0;*/
}
void Enviroment::logGenom()
{
    vector<double> averageList(_net->get_genomsize(),0);
    for(unsigned int a=0; a<_net->get_animals(); a++)
    {
        for(unsigned int b=0; b<_net->get_genomsize(); b++)
        {
            averageList[b] += _net->get_genom(a)[b];
        }
    }
    _file = fopen("genom.csv","a");
    for(unsigned int b=0; b<_net->get_genomsize(); b++)
    {
        averageList[b] /= (double)_net->get_animals();
        fprintf(_file,"%8f;",averageList[b]);
    }
    fprintf(_file,"\n");
    fclose(_file);
}

void Enviroment::setInputs()
{
    double net_otherAnimal = -0.5;
    double net_food = 1.0;
    double net_nothing = 0.0;
    double net_poison = -1;
    double net_boundry = -0.3;
    vector<vector<int> > posOffset(7,vector<int>(7,0));
    vector<double>   inputList(9,0);

    posOffset[0] = {0,0,0,1,0,0,0};
    posOffset[1] = {0,0,0,1,0,0,0};
    posOffset[2] = {0,0,0,1,0,0,0};
    posOffset[3] = {1,1,1,0,1,1,1};
    posOffset[4] = {0,0,0,0,0,0,0};
    posOffset[5] = {0,0,0,0,0,0,0};
    posOffset[6] = {0,0,0,0,0,0,0};



    int local_a = 0;
    int local_b = 0;
    int local_c = 0;
    //printf("direction: B%i    %i\n",thread_direction[thread_animal],&thread_direction[thread_animal]);
    for(unsigned int a=0; a<_net->get_animals(); a++)
    {
        if(_animalAliveList[a] == false)
        {
            continue;
        }
        local_c = 0;
        for(local_a = 0; local_a<posOffset.size(); local_a++)
        {
            for(local_b = 0; local_b<posOffset[local_a].size(); local_b++)
            {
                if(posOffset[local_a][local_b] == 1)
                {
                    inputList[local_c] = 0;
                    switch(_animalRotationList[a])
                    {
                        case 0:
                        {
                            inputList[local_c] = (double)_map[_animalPositionList[a].x-3 + local_a][_animalPositionList[a].y- 3 + local_b] / 10;
                            local_c++;
                            break;
                        }
                        case 1:
                        {
                            inputList[local_c] = (double)_map[_animalPositionList[a].x-3 + 6-local_b][_animalPositionList[a].y- 3 + local_a] /10;
                            local_c++;
                            break;
                        }
                        case 2:
                        {
                            inputList[local_c] = (double)_map[_animalPositionList[a].x-3 + 6-local_a][_animalPositionList[a].y- 3 + 6-local_b] /10;
                            local_c++;
                            break;
                        }
                        case 3:
                        {
                            inputList[local_c] = (double)_map[_animalPositionList[a].x-3 + local_b][_animalPositionList[a].y- 3 + 6-local_a] /10;
                            local_c++;
                            break;
                        }
                    }
                }
            }
        }
        _net->set_input(a,inputList);
    }

}
void Enviroment::walk()
{
    cmdXY(0,40);
    for(unsigned int a=0; a<_net->get_animals(); a++)
    {
        if(_animalAliveList[a] == false)
        {
            continue;
        }


        double highestOutputValue = _net->get_output(a,0);
        unsigned short   highestOutput = 0;
        for(unsigned short out=1; out<_net->get_outputNeurons(); out++)
        {
            if(_net->get_output(a,(unsigned int)out) > highestOutputValue)
            {
                highestOutputValue  = _net->get_output(a,(unsigned int)out);
                highestOutput       = out;
            }
        }
        // _animalRotationList[a] = highestOutput;

        vector<struct Coord> direction;
        direction.push_back(Coord({0,-1}));
        direction.push_back(Coord({1, 0}));
        direction.push_back(Coord({0, 1}));
        direction.push_back(Coord({-1,0}));

        _animalMap[_animalPositionList[a].x][_animalPositionList[a].y] = NOTHING;


        switch(highestOutput)
        {
            case 0: //up
            {
                _animalBackCountList[a] = 0;
                _animalPositionList[a].x += direction[_animalRotationList[a]].x;
                _animalPositionList[a].y += direction[_animalRotationList[a]].y;
                _animalCurrentFoodList[a]--;
                break;
            }
            case 1: //right
            {
                _animalBackCountList[a] = 0;
                _animalPositionList[a].x += direction[_animalRotationList[a]].x;
                _animalPositionList[a].y += direction[_animalRotationList[a]].y;
                _animalCurrentFoodList[a]--;
                break;
            }
            case 2: //down
            {
                _animalPositionList[a].x += direction[_animalRotationList[a]].x;
                _animalPositionList[a].y += direction[_animalRotationList[a]].y;
                _animalCurrentFoodList[a]--;
                _animalBackCountList[a]++;
                if(_animalBackCountList[a]>2)
                {
                    _animalScoreList[a]-=0.5;
                }
                break;
            }
            case 3: //left
            {
                _animalBackCountList[a] = 0;
                _animalPositionList[a].x += direction[_animalRotationList[a]].x;
                _animalPositionList[a].y += direction[_animalRotationList[a]].y;
                _animalCurrentFoodList[a]--;

                break;
            }
        }
        _animalMap[_animalPositionList[a].x][_animalPositionList[a].y] = ANIMAL;
        _animalRotationList[a] = highestOutput;
        int collision = checkAnimalCollision(a);
        //qDebug() << "rotation: "<< a << "\tdirection: "<<highestOutput << " alive: "<< _animalAliveList[a] << "collision: "<<collision << " food: "<<_animalCurrentFoodList[a] << "   ";
        if(collision != NOTHING)
        {
            switch(collision)
            {
                case ANIMAL:
                {
                    kill(a);
                    break;
                }
                case FOOD:
                {
                    _animalCurrentFoodList[a]+=4;
                    Coord pos = getRandomPos();
                    _map[_animalPositionList[a].x][_animalPositionList[a].y] = NOTHING;
                    _map[pos.x][pos.y] = FOOD;
                    for(unsigned int b=0; b<_foodPositionList.size(); b++)
                    {
                        if(_foodPositionList[b].x == _animalPositionList[a].x && _foodPositionList[b].y == _animalPositionList[a].y)
                        {
                            _foodPositionList[b] = getRandomPos();
                        }
                    }
                    break;
                }
                case POISON:
                {
                    _animalCurrentFoodList[a]-=4;
                    for(unsigned int b=0; b<_poisonPositionList.size(); b++)
                    {
                        if(_poisonPositionList[b].x == _animalPositionList[a].x && _poisonPositionList[b].y == _animalPositionList[a].y)
                        {
                            _poisonPositionList[b] = getRandomPos();
                        }
                    }
                    break;
                }
                case OBSTICLE:
                {
                    kill(a);
                    break;
                }
            }
        }
        if(_animalCurrentFoodList[a] <= 0)
        {
            kill(a);
        }

    }
}
void Enviroment::kill(unsigned int animal)
{
  //  qDebug() << "kill: "<<animal;
    if(_animalAliveList[animal] == true)
    {
        _animalMap[_animalPositionList[animal].x][_animalPositionList[animal].y] = NOTHING;
        _animalPositionList[animal] = Coord({0,0});
        _animalAliveList[animal] = false;
    }
}
