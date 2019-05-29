#include "food.h"

Food::Food(QSize mapSize)
{
    _mapSize = mapSize;
    respawn();
}
Food::~Food()
{

}
void Food::respawn()
{
    _eaten = false;
    _liveCounter = 80 + rand() % 100;
    pos(QPoint(1+rand()%(_mapSize.width()-2),1+rand()%(_mapSize.height()-2)));

    int foodamount = 50 + rand() % 50;
    if(rand()%10 == 1)
    {
        foodamount = -foodamount;
    }

    amount((float)foodamount * 0.5f);
}

void Food::pos(QPoint pos)
{
    if(pos.x() < 0)
        pos.setX(0);

    if(pos.y() < 0)
        pos.setY(0);

    if(pos.x() > _mapSize.width())
        pos.setX(_mapSize.width());

    if(pos.y() > _mapSize.height())
        pos.setY(_mapSize.height());

    _pos = pos;
}
QPoint Food::pos()
{
    return _pos;
}

void Food::amount(int amount)
{
    if(amount >= -100 && amount <= 100)
    _amount = amount;
    if(_amount < 0)
    {
        _color = QColor((255*(-_amount))/100,0,0);
    }
    else
    {
        _color = QColor(0,(255*(_amount))/100,0);
    }

}
int Food::amount()
{
    return _amount;
}

QColor Food::color()
{
    return _color;
}
bool Food::isAlive()
{
    return _eaten;
}
void Food::update()
{
    if(_liveCounter == 0)
    {
        respawn();
    }
    else
    {
        _liveCounter--;
    }
}
void Food::eaten()
{
    _eaten = true;
}
