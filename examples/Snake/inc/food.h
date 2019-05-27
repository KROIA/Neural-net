#ifndef FOOD_H
#define FOOD_H

#include <QColor>
#include <QPoint>
#include <QSize>
#include <vector>
#include <QDebug>


using namespace std;

class Food : public QObject
{
        Q_OBJECT
    public:
        Food(QSize mapSize);
        ~Food();
        void respawn();

        void pos(QPoint pos);
        QPoint pos();

        void amount(int amount);
        int amount();

        QColor color();

        bool liveStatus();
        void update();

    signals:

    public slots:
        void eaten();

    private:

        QSize _mapSize;

        QPoint _pos;
        QColor _color;
        int _amount;
        bool _eaten;

        unsigned int _liveCounter;
};

#endif // FOOD_H
