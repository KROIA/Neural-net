#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include <geneticnet.h>
#include <QDebug>
#include <vector>
#include <iostream>
#include <windows.h>

using namespace std;

#define NOTHING  0
#define ANIMAL   -5
#define FOOD     10
#define POISON   -3
#define OBSTICLE -10


struct Coord{
        int x;
        int y;
};

class Enviroment{
    public:
        Enviroment();
        void setup();
        void cmdXY(int x,int y);
        void draw();
        void tick();
        GeneticNet *_net;
        struct Coord getRandomPos();
        void randomFoodPos();
        void randomPoisonPos();
        void randomAnimalPos();
        int  checkCollision(struct Coord position);
        int  checkAnimalCollision(unsigned int animal);

        void logGenom();
    private:
        void setInputs();
        void walk();
        void restart();
        void kill(unsigned int animal);

        struct Coord _enviromentSize;
        vector<struct Coord>    _animalPositionList;
        vector<unsigned short>   _animalRotationList;
        vector<int>             _animalCurrentFoodList;
        vector<bool>            _animalAliveList;
        vector<int>             _animalBackCountList;
        int                     _animalStartFood;
        vector<double>           _animalScoreList;
        double                   _averageScore;
        unsigned int            _stillAlive;
        unsigned int            _cycles;
        unsigned int            _cyclesCounter;

        unsigned int            _maxTicks;
        unsigned int            _ticks;
        unsigned int            _netSaveInterval;
        unsigned int            _netSaveTicks;

        unsigned int _foodAmount;
        unsigned int _poisonAmount;
        int          _boundrySize;
        vector<struct Coord>    _foodPositionList;
        vector<struct Coord>    _poisonPositionList;
        vector<struct Coord>    _boundaryPositionList;
        vector<vector<int>  >   _map;
        vector<vector<int>  >   _animalMap;


        FILE *_file;
};
#endif // ENVIROMENT_H
