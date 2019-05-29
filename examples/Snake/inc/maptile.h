#ifndef MAPTILE_H
#define MAPTILE_H

#include <QSize>
#include <QDebug>
#include "rect.h"
#include <vector>

using namespace std;

enum MapData
{
    nothing = 0,
    snake = 1,
    food = 2,
    obsticle = 3
};

class Maptile : public QObject
{
        Q_OBJECT
    public:
        Maptile();
        ~Maptile();

       // void pos(QPoint pos);
       // QPoint pos();


    private:
        QPoint _pos;
        QColor _color;
        QSize  _size;
        int    _data;




};

#endif // MAPTILE_H
