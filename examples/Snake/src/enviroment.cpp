#include "enviroment.h"


Enviroment::Enviroment(QPainter *p)
{
    _painter = p;

    _enviromentColor = QColor(250,250,250);
    _snakeColor      = QColor(0,200,0);

    scale(2);
    mapsize(QSize(10,10));
    tileSize(10);
    tileSpace(3);
    drawPos(QPoint(50,50));

    mapInit();

    //_player = new Player(mapsize());
    _player.push_back(new Player(_player.size(),mapsize()));
    _player.push_back(new Player(_player.size(),mapsize()));
    //_player2 = new Player(mapsize());
    //drawMap();
    for(unsigned int a = 0; a<_player.size(); a++)
    {
        connect(_player[a],SIGNAL(collision(unsigned int,vector<QPoint>)),this,SLOT(snakeCollision(unsigned int,vector<QPoint>)));
        connect(_player[a],SIGNAL(starved(unsigned int)),this,SLOT(snakeStarved(unsigned int)));
    }

    _foodAmount = 10;

}
Enviroment::~Enviroment()
{

}


void Enviroment::mapsize(QSize size)
{
    _mapsize = size * scale();
}
QSize Enviroment::mapsize()
{
    return _mapsize;
}
void Enviroment::tileSize(unsigned int size)
{
    _tileSize = size * scale();
}
unsigned int Enviroment::tileSize()
{
    return _tileSize;
}
void Enviroment::tileSpace(unsigned int space)
{
    _tileSpace = space * scale();
}
unsigned int Enviroment::tileSpace()
{
    return _tileSpace;
}
void Enviroment::drawPos(QPoint pos)
{
    _drawPos = pos;
}
QPoint Enviroment::drawPos()
{
    return _drawPos;
}
void Enviroment::scale(unsigned int scale)
{
    if(scale >= 1)
        _scale = scale;
    else
        _scale = 1;
}
unsigned int Enviroment::scale()
{
    return _scale;
}
void Enviroment::foodAmount(unsigned int amount)
{
    _foodAmount = amount;
}
unsigned int Enviroment::foodAmount()
{
    return _foodAmount;
}

void Enviroment::mapInit()
{
    _map.clear();


    for(unsigned int x=0; x<mapsize().width(); x++)
    {
        vector<Rect*> tmpVec;
        for(unsigned int y=0; y<mapsize().height(); y++)
        {
            Rect *tmpRect = new Rect(_painter);

            tmpRect->begin(QPoint(0,0));
            tmpRect->end(QPoint(tileSize(),tileSize()));

            tmpRect->drawPos(QPoint(drawPos().x()+(tileSize()+_tileSpace)*x,drawPos().y()+(tileSize()+_tileSpace)*y));

            tmpRect->frame(false);
            tmpRect->frameSize(5);
            tmpRect->frameColor(_enviromentColor);
            tmpRect->color(_enviromentColor);

            tmpVec.push_back(tmpRect);
        }
        _map.push_back(tmpVec);
    }
}
void Enviroment::setSnakeOnMap(unsigned int player)
{
    if(_player.size() <= player)
    {
        qDebug() << "out of Range error: void Enviroment::setSnakeOnMap(unsigned int player) : " << player;
        return;
    }
    for(unsigned int a=0; a<_player[player]->size(); a++)
    {
        if(_player[player]->pos()[a].x() >= _mapsize.width() ||
           _player[player]->pos()[a].x() < 0 ||
           _player[player]->pos()[a].y() >= _mapsize.height() ||
           _player[player]->pos()[a].y() < 0)
        {
            qDebug() << "ERROR: snake: "<<player<<" out of Boudry";
            qDebug() << _player[player]->pos()[a];
            controlSnakeDeath(player,true);
        }else
        {
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->frameSize(5);
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->frameColor(_player[player]->color(a));
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->frame(true);
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->color(_player[player]->color(a));
        }
    }
}
void Enviroment::drawMap()
{
    for(unsigned int y=0; y<mapsize().height(); y++)
    {
        for(unsigned int x=0; x<mapsize().width(); x++)
        {
            _map[x][y]->frame(false);
            _map[x][y]->color(_enviromentColor);
        }
    }
    //-----------Set Snake
    for(unsigned int a=0; a<_player.size(); a++)
    {
        setSnakeOnMap(a);
    }
    //----------Set Food
    for(unsigned int a=0; a<_food.size(); a++)
    {
        if(_food[a]->pos().x() >= _mapsize.width() ||
           _food[a]->pos().x() < 0 ||
           _food[a]->pos().y() >= _mapsize.height() ||
           _food[a]->pos().y() < 0)
        {
            qDebug() << "ERROR: food out of Boudry";
            qDebug() << _food[a]->pos();

        }else
        {
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->frameSize(2);
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->frameColor(QColor((_food[a]->color().red()*80)/100,(_food[a]->color().green()*80)/100,(_food[a]->color().blue()*80)/100));
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->frame(true);
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->color(_food[a]->color());
        }
    }


    for(unsigned int y=0; y<mapsize().height(); y++)
    {
        for(unsigned int x=0; x<mapsize().width(); x++)
        {
            _map[x][y]->draw();
        }
    }
}

void Enviroment::update()
{
    if(_food.size() != _foodAmount)
    {
        if(_food.size() > _foodAmount)
        {
            _food.pop_back();
        }
        if(_food.size() < _foodAmount)
        {
            _food.push_back(new Food(_mapsize));
        }
    }
    for(unsigned int c=0; c<_player.size(); c++)
    {
    //---------Check Snake Food collision
        vector<QPoint> snakePos = _player[c]->pos();
        for(unsigned int a=0; a<snakePos.size(); a++)
        {
            for(unsigned int b=0; b<_food.size(); b++)
            {
                if(snakePos[a].x() == _food[b]->pos().x() &&
                   snakePos[a].y() == _food[b]->pos().y())
                {
                    qDebug() << "Food eaten. Amount: " << _food[b]->amount();
                    _player[c]->addFood(_food[b]->amount());
                    _food[b]->eaten();
                    _food[b]->respawn();
                }
            }
        }
        for(unsigned int a=0; a<_player.size(); a++)
        {
            if(c == a)
                continue;
            for(unsigned int b=0; b<_player[a]->size(); b++)
            {
                if(_player[c]->pos(0).x() == _player[a]->pos(b).x() &&
                   _player[c]->pos(0).y() == _player[a]->pos(b).y())
                {
                    qDebug() << "Collision between snakes: " << c << " : " << a;
                    if(b == 0)
                    {
                        if(_player[c]->size() > _player[a]->size())
                        {
                            controlSnakeDeath(a,true);
                        }
                        else {
                            controlSnakeDeath(c,true);
                        }
                    }else
                    {
                        controlSnakeDeath(c,true);
                    }
                }
            }
        }


        _player[c]->update();
    }
    //-----------------------------------

    for(unsigned int a=0; a<_food.size(); a++)
    {
        _food[a]->update();
    }
}


void Enviroment::controlSnakeDirection(int player,Direction dir)
{
    if(_player.size() <= player)
        player = 0;
    _player[player]->direction(dir);
}
void Enviroment::controlSnakeAddSize(int player,int increment)
{
    if(_player.size() <= player)
        player = 0;
    _player[player]->addSize(increment);
}
void Enviroment::controlSnakeAddFood(int player,int increment)
{
    if(_player.size() <= player)
        player = 0;
    qDebug() << "Added food: "<< increment;
    _player[player]->addFood(increment);
}
void Enviroment::controlSnakeDeath(int player,bool death)
{
    if(_player.size() <= player)
        player = 0;
    if(death)
        _player[player]->kill();
    else
        _player[player]->revive();
}

void Enviroment::snakeCollision(unsigned int player,vector<QPoint> pos)
{
    if(_player.size() <= player)
        player = 0;
    //qDebug() << "<<<collision: "<<pos;
    controlSnakeDeath(player,true);
}
void Enviroment::snakeStarved(unsigned int player)
{
    if(_player.size() <= player)
        player = 0;
    qDebug() << "<<<starved: ";
    controlSnakeDeath(player,true);
}
Player *Enviroment::player(unsigned int player)
{
    if(_player.size() <= player)
        player = 0;
    return _player[player];
}
