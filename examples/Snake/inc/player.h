#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>
#include <QPoint>
#include <QSize>
#include <vector>
#include <QDebug>

//#define GLOBAL_VIEW //enables global view control


using namespace std;

enum Direction
{
    _up = 0,
    _left = 1,
    _down = 2,
    _right = 3
};

class Player : public QObject
{
        Q_OBJECT
    public:
        Player(unsigned int player,QSize mapSize);
        ~Player();
        void playerIndex(unsigned int index);
        unsigned int playerIndex();

        void sollSize(int size);
        int sollSize();
        unsigned int size();
        void addSize(int increment = 1);
        void food(unsigned int food);
        unsigned int food();
        void addFood(int increment = 1);

        void pos(vector<QPoint> pos);
        QPoint pos(unsigned int index);
        vector<QPoint> pos();

        void standardColor(QColor standard);
        QColor standardColor();
        QColor color(unsigned int index);
        vector<QColor> color();

        void update();

        void direction(int dir);
        int direction();

        void kill();
        void revive();
        bool liveStatus();

        unsigned int steps();

    signals:
        void collision(unsigned int player,vector<QPoint>);
        void starved(unsigned int player);

    private:

        void checkForSelfCollision();

        int _presetSize;
        unsigned int _presetFood;
        unsigned int _foodPerTile;
        QColor _standardBodyColor;

        int _size;
        unsigned int _food;
        unsigned int _steps;


        QSize _mapSize;

        vector<QPoint> _playerPos;
        vector<QColor> _playerColor;
        QPoint _lastPos;
        QColor _lastColor;
        int _direction;
        bool _death;
        unsigned int _playerIndex;

};
#endif // PLAYER_H
