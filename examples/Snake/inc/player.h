#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>
#include <QPoint>
#include <QSize>
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
struct Score
{
        double food;
        unsigned int steps;
};

class Player : public QObject
{
        Q_OBJECT
    public:
        Player(unsigned int player,QSize mapSize);
        ~Player();
        void playerIndex(unsigned int index);
        unsigned int playerIndex();

        void mapSize(QSize mapSize);
        QSize mapsize();

        void sollSize(int size);
        int sollSize();
        unsigned int size();
        void addSize(int increment = 1);
        void food(long int food);
        long int food();
        void addFood(int increment = 1);

        void pos(std::vector<QPoint> pos);
        QPoint *pos(unsigned int index);
        std::vector<QPoint> *pos();

        void standardColor(QColor standard);
        QColor standardColor();
        QColor color(unsigned int index);
        std::vector<QColor> color();

        void update();

        void globalView(bool globalView);
        bool globalView();
        void direction(Direction dir);
        Direction direction();

        void kill();
        void revive();
        bool isAlive();

        unsigned int steps();

        void killreward(bool enable);
        bool killreward();
        unsigned int deathCount();
        void resetDeathCount();
        std::vector<struct Score> score();
        struct Score score(unsigned int index);
        struct Score lastScore();
        struct Score currentScore();
        struct Score averageScore();
        struct Score averageScore(std::vector<Score> scoreList);
        struct Score averageScoreOverTheLast(unsigned int lastAmount);
        struct Score addedUpScore();
        void resetScore();


    signals:
        //void collision(unsigned int player,std::vector<QPoint>);
        //void starved(unsigned int player);

    private:

        void checkForSelfCollision();

        int _presetSize;
        unsigned int _presetFood;
        unsigned int _foodPerTile;
        QColor _standardBodyColor;

        int _size;
        long _food;
        unsigned int _steps;
        unsigned int _maxSteps;


        QSize _mapSize;

        std::vector<QPoint> _playerPos;
        std::vector<QColor> _playerColor;
        QPoint _lastPos;
        QColor _lastColor;
        int _direction;
        bool _death;
        unsigned int _playerIndex;
        bool _glovalView;
        bool _killreward;
        unsigned int _deathCount;

        std::vector<struct Score>_scoreList;

};
#endif // PLAYER_H
