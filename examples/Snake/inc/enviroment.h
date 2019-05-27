#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include <QSize>
#include <QDebug>
#include "rect.h"
#include <vector>

#include "player.h"
#include "food.h"

using namespace std;


class Enviroment : public QObject
{
        Q_OBJECT
    public:
        Enviroment(QPainter *p);
        ~Enviroment();

        void mapsize(QSize size);
        QSize mapsize();
        void tileSize(unsigned int size);
        unsigned int tileSize();
        void tileSpace(unsigned int space);
        unsigned int tileSpace();
        void drawPos(QPoint pos);
        QPoint drawPos();
        void scale(unsigned int scale);
        unsigned int scale();

        void foodAmount(unsigned int amount);
        unsigned int foodAmount();

        void mapInit();

        void setSnakeOnMap(unsigned int player);
        void drawMap();

        void update();

        void controlSnakeDirection(int player,Direction dir);
        void controlSnakeAddSize(int player,int increment = 1);
        void controlSnakeAddFood(int player,int increment = 1);
        void controlSnakeDeath(int player,bool death = true);

        Player *player(unsigned int player = 0);

    public slots:
        void snakeCollision(unsigned int player,vector<QPoint> pos);
        void snakeStarved(unsigned int player);

    private:
        QPainter   *_painter;

        QSize _mapsize;
        unsigned int _tileSize;
        unsigned int _tileSpace;
        QPoint _drawPos;
        unsigned int _scale;


        vector<vector<Rect*> > _map;
        vector<Player*> _player;
        //Player *_player2;
        vector<Food*> _food;

        unsigned int _foodAmount;



        QColor _enviromentColor;
        QColor _snakeColor;
};

#endif // ENVIROMENT_H
