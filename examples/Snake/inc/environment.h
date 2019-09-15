#ifndef Environment_H
#define Environment_H

#include <QSize>
#include <QDebug>
#include "rect.h"
#include <vector>
#include "geometry.h"
#include <QTimer>
#include <QLabel>

#include "player.h"
#include "food.h"

using namespace std;

enum MapData
{
    nothing = 0,
    snake = 1,
    food = 2,
    obsticle = 3
};
enum AI_dataLayer
{
    food_layer = 0,
    snake_layer = 1,
    obsticle_layer = 2
};

class Environment : public QObject
{
        Q_OBJECT
    public:
        Environment(QWidget *parent,QPainter *p,unsigned int player);
        ~Environment();

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
        void placeObsticle();

        void setSnakeOnMap(unsigned int player);
        void drawMap();


        void update();

        void controlSnakeDirection(unsigned int player,Direction dir);
        void controlSnakeAddSize(unsigned int player,int increment = 1);
        void controlSnakeAddFood(unsigned int player,int increment = 1);
        void controlSnakeDeath(unsigned int player,bool death = true);

        Player *player(unsigned int player = 0);
        unsigned int playerAmount();
        vector<vector<float>    >AI_mapData(unsigned int player, vector<QPoint>   fieldOfView);
        vector<vector<float>    >AI_mapData_simple(unsigned int player);
        vector<QPoint> rotate_90(vector<QPoint> data,QPoint rotPoint, int amount = 1);

        void showInfoText(bool enable);
        bool showInfoText();
        void drawEnable(bool enable);
        bool drawEnable();
    signals:
        void playerKill(unsigned int,unsigned int);

    public slots:
        void snakeCollision(unsigned int player,vector<QPoint> pos);
        void snakeStarved(unsigned int player);
        void obsticleReplace();

    private:
        QPainter   *_painter;
        QWidget    *_parent;


        QSize _mapsize;
        unsigned int _tileSize;
        unsigned int _tileSpace;
        QPoint _drawPos;
        unsigned int _scale;
        unsigned int _playerAmount;


        vector<vector<Rect*> > _map;
        vector<vector<QLabel*> > _labelMap;
        vector<vector<int>  >  _viewMap;
        vector<Player*> _player;
        vector<Food*> _food;

        unsigned int _foodAmount;



        QColor _environmentColor;
        QColor _snakeColor;
        QColor _obsticleColor;
        bool   _showInfoText;
        bool   _drawEnable;

       // int test = 0;
};

#endif // Environment_H
