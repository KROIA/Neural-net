#include "environment.h"


Environment::Environment(QPainter *p,unsigned int player)
{
    _painter = p;
    _playerAmount = player;

    _environmentColor = QColor(250,250,250);
    _snakeColor      = QColor(0,200,0);
    _obsticleColor   = QColor(250,150,0);

    scale(2);
    mapsize(QSize(30,30));
    tileSize(8);
    tileSpace(2);
    drawPos(QPoint(50,50));



    //_player = new Player(mapsize());
    for(unsigned int a=0; a<_playerAmount; a++)
    {
        _player.push_back(new Player(_player.size(),mapsize()));
        connect(_player[a],SIGNAL(collision(unsigned int,vector<QPoint>)),this,SLOT(snakeCollision(unsigned int,vector<QPoint>)));
        connect(_player[a],SIGNAL(starved(unsigned int)),this,SLOT(snakeStarved(unsigned int)));
    }
    _foodAmount = 10;


    mapInit();

}
Environment::~Environment()
{

}


void Environment::mapsize(QSize size)
{
    _mapsize = size /* scale()*/;
    for(unsigned int a=0; a<_player.size(); a++)
    {
        _player[a]->mapSize(_mapsize);
    }
}
QSize Environment::mapsize()
{
    return _mapsize;
}
void Environment::tileSize(unsigned int size)
{
    _tileSize = size * scale();
}
unsigned int Environment::tileSize()
{
    return _tileSize;
}
void Environment::tileSpace(unsigned int space)
{
    _tileSpace = space * scale();
}
unsigned int Environment::tileSpace()
{
    return _tileSpace;
}
void Environment::drawPos(QPoint pos)
{
    _drawPos = pos;
}
QPoint Environment::drawPos()
{
    return _drawPos;
}
void Environment::scale(unsigned int scale)
{
    if(scale >= 1)
        _scale = scale;
    else
        _scale = 1;
}
unsigned int Environment::scale()
{
    return _scale;
}
void Environment::foodAmount(unsigned int amount)
{
    _foodAmount = amount;
}
unsigned int Environment::foodAmount()
{
    return _foodAmount;
}

void Environment::mapInit()
{
    _map.clear();
    _viewMap = vector<vector<int>   >(mapsize().width(),vector<int>(mapsize().height()));


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
            tmpRect->frameColor(_environmentColor);
            tmpRect->color(_environmentColor);

            tmpVec.push_back(tmpRect);
        }
        _map.push_back(tmpVec);
    }
    for(unsigned int a=0; a<_player.size(); a++)
    {
        _player[a]->kill();
        _player[a]->revive();
    }
}
void Environment::setSnakeOnMap(unsigned int player)
{
    if(_player.size() <= player)
    {
        qDebug() << "out of Range error: void Environment::setSnakeOnMap(unsigned int player) : " << player;
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
            if(_viewMap[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()] == MapData::obsticle)
            {
                controlSnakeDeath(player,true);
                return;
            }
            _viewMap[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()] = MapData::snake;
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->frameSize(5);
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->frameColor(_player[player]->color(a));
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->frame(false);
            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->color(_player[player]->color(a));

            _map[_player[player]->pos()[a].x()][_player[player]->pos()[a].y()]->end(QPoint(tileSize()*1.3,tileSize()*1.3));
        }
    }
}
void Environment::drawMap()
{


    //qDebug() << "------------------------------------";
    for(unsigned int y=0; y<mapsize().height(); y++)
    {
       // QString mapLine;
        for(unsigned int x=0; x<mapsize().width(); x++)
        {
            _map[x][y]->draw();
           // mapLine += QString::number(_viewMap[x][y]);
        }
       // qDebug() << mapLine;
    }
   // qDebug() << "------------------------------------";
}

void Environment::update()
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
                        //reward for player a
                        _player[a] ->addFood(_player[c]->food()/2);
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

    for(unsigned int y=0; y<mapsize().height(); y++)
    {
        for(unsigned int x=0; x<mapsize().width(); x++)
        {
            _viewMap[x][y] = MapData::nothing;
            _map[x][y]->frame(false);
            _map[x][y]->color(_environmentColor);
            _map[x][y]->end(QPoint(tileSize(),tileSize()));

        }
    }
    for(unsigned int a=0; a<mapsize().height(); a++)
    {
        _viewMap[0][a] = MapData::obsticle;
        _viewMap[_viewMap.size()-1][a] = MapData::obsticle;
        _map[0][a]->color(_obsticleColor);
        _map[_viewMap.size()-1][a]->color(_obsticleColor);
    }
    for(unsigned int a=0; a<mapsize().width(); a++)
    {
        _viewMap[a][0] = MapData::obsticle;
        _viewMap[a][_viewMap[a].size()-1] = MapData::obsticle;
        _map[a][0]->color(_obsticleColor);
        _map[a][_viewMap[a].size()-1]->color(_obsticleColor);
    }

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
            _viewMap[_food[a]->pos().x()][_food[a]->pos().y()] = MapData::food;
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->frameSize(2);
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->frameColor(QColor((_food[a]->color().red()*80)/100,(_food[a]->color().green()*80)/100,(_food[a]->color().blue()*80)/100));
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->frame(true);
            _map[_food[a]->pos().x()][_food[a]->pos().y()]->color(_food[a]->color());

            _map[_food[a]->pos().x()][_food[a]->pos().y()]->end(QPoint(tileSize()*0.8,tileSize()*0.8));

        }
    }
    //-----------Set Snake
    for(unsigned int a=0; a<_player.size(); a++)
    {
        setSnakeOnMap(a);
    }
    //----------Set Food
   /* test++;
    for(unsigned int x=0; x<_map.size(); x++)
    {
        for(unsigned int y=0; y<_map[x].size(); y++)
        {
            //_map[x][y]->middlePoint(QPointF(0,0));
            //_map[x][y]->rotate(5);
            QPoint beginPos = QPoint(drawPos().x()+(tileSize()+_tileSpace)*x,drawPos().y()+(tileSize()+_tileSpace)*y);
            QPointF tmpPos = geometry::rotate(QPointF((float)beginPos.x(),(float)beginPos.y()),QPointF(300,300),test);
            _map[x][y]->drawPos(tmpPos.toPoint());
        }
    }*/
}


void Environment::controlSnakeDirection(unsigned int player,Direction dir)
{
    if(_player.size() <= player)
        player = 0;
    _player[player]->direction(dir);
}
void Environment::controlSnakeAddSize(unsigned int player,int increment)
{
    if(_player.size() <= player)
        player = 0;
    _player[player]->addSize(increment);
}
void Environment::controlSnakeAddFood(unsigned int player,int increment)
{
    if(_player.size() <= player)
        player = 0;
    qDebug() << "Added food: "<< increment;
    _player[player]->addFood(increment);
}
void Environment::controlSnakeDeath(unsigned int player,bool death)
{
    if(_player.size() <= player)
        player = 0;
    if(death)
        _player[player]->kill();
    else
        _player[player]->revive();
}

void Environment::snakeCollision(unsigned int player,vector<QPoint> pos)
{
    if(_player.size() <= player)
        player = 0;
    //qDebug() << "<<<collision: "<<pos;
    controlSnakeDeath(player,true);
}
void Environment::snakeStarved(unsigned int player)
{
    if(_player.size() <= player)
        player = 0;
    qDebug() << "<<<starved: ";
    controlSnakeDeath(player,true);
}
Player *Environment::player(unsigned int player)
{
    if(_player.size() <= player)
        player = 0;
    return _player[player];
}
vector<vector<float>    >Environment::AI_mapData(unsigned int player,vector<QPoint>   fieldOfView)
{
    vector<int>   rawData;
    vector<vector<float>    >outputData(3,vector<float>());
    //vector<QPoint> coordView;
    if(fieldOfView.size() == 0 || player >= _player.size())
        return outputData;
    geometry::Angle rotAnge = 0;
    switch(_player[player]->direction()){
        case Direction::_up:
        {
            rotAnge = 0;
            fieldOfView = rotate_90(fieldOfView,QPoint(0,0),4);
            break;
        }
        case Direction::_left:
        {
            rotAnge = 270;
            fieldOfView = rotate_90(fieldOfView,QPoint(0,0),3);
            break;
        }
        case Direction::_down:
        {
            rotAnge = 180;
            fieldOfView = rotate_90(fieldOfView,QPoint(0,0),2);

            break;
        }
        case Direction::_right:
        {
            rotAnge = 90;
            fieldOfView = rotate_90(fieldOfView,QPoint(0,0),1);
            break;
        }
    }

  //  fieldOfView = geometry::rotate(fieldOfView,QPoint(0,0),rotAnge);
    int c = 0;

    vector<QPointF> fieldOfFiew_F;
    for(unsigned int a=0; a<fieldOfView.size(); a++)
    {
        fieldOfFiew_F.push_back(QPointF((float)fieldOfView[a].x(),(float)fieldOfView[a].y()));
    }
    fieldOfFiew_F = geometry::rotate(fieldOfFiew_F,QPoint(0,0),rotAnge);
  /*  if(_player[player]->direction() == Direction::_up)
    {
        fieldOfFiew_F = geometry::rotate(fieldOfFiew_F,QPoint(0,0),geometry::Angle(180));
        fieldOfFiew_F = geometry::rotate(fieldOfFiew_F,QPoint(0,0),geometry::Angle(180));
        fieldOfView = geometry::rotate(fieldOfView,QPoint(0,0),geometry::Angle(180));
        fieldOfView = geometry::rotate(fieldOfView,QPoint(0,0),geometry::Angle(180));
    }
    for(unsigned int a=0; a<fieldOfView.size(); a++)
    {
       fieldOfView[a] = QPoint((int)round(fieldOfFiew_F[a].x()),(int)round(fieldOfFiew_F[a].y()));
    }

    qDebug() << "fieldOfView_";

    for(unsigned int a=0; a<sqrt(fieldOfFiew_F.size()); a++)
    {
        string txt = "";
        for(unsigned int b=0; b<sqrt(fieldOfFiew_F.size()); b++)
        {
            txt+= to_string(fieldOfFiew_F[c].x())+"|"+to_string(fieldOfFiew_F[c].y())+"  ";
            c++;
        }
        qDebug() << QString::fromStdString(txt);
    }
    c = 0;
    for(unsigned int a=0; a<sqrt(fieldOfFiew_F.size()); a++)
    {
        string txt = "";
        for(unsigned int b=0; b<sqrt(fieldOfFiew_F.size()); b++)
        {
            txt+= to_string((int)round(fieldOfFiew_F[c].x()))+"|"+to_string((int)round(fieldOfFiew_F[c].y()))+"  ";
            c++;
        }
        qDebug() << QString::fromStdString(txt);
    }*/

    for(unsigned int a=0; a<fieldOfView.size(); a++)
    {

        outputData[food_layer].push_back(0);
        outputData[snake_layer].push_back(0);
        outputData[obsticle_layer].push_back(0);
        if(_player[player]->pos(0).x()+fieldOfView[a].x() >= _viewMap.size() || _player[player]->pos(0).x()+fieldOfView[a].x() < 0)
        {
            rawData.push_back(MapData::nothing);
        }else if(_player[player]->pos(0).y()+fieldOfView[a].y() >= _viewMap[_player[player]->pos(0).x()+fieldOfView[a].x()].size() || _player[player]->pos(0).y()+fieldOfView[a].y() < 0)
        {
            rawData.push_back(MapData::nothing);
        }else
        {
            QPoint pos = QPoint(_player[player]->pos(0).x()+fieldOfView[a].x(),_player[player]->pos(0).y()+fieldOfView[a].y());
            rawData.push_back(_viewMap[pos.x()][pos.y()]);
            switch(_viewMap[pos.x()][pos.y()])
            {
                case MapData::food:
                {
                    for(unsigned int b=0; b<_food.size(); b++)
                    {
                        if(_food[b]->pos().x() == pos.x() && _food[b]->pos().y() == pos.y())
                        {
                            outputData[food_layer][outputData[food_layer].size()-1] = (float)_food[b]->amount()/100;
                            //outputData[food_layer][outputData[food_layer].size()-1] = 1;
                        }
                    }
                    break;
                }
                case MapData::snake:
                {
                    outputData[snake_layer][outputData[snake_layer].size()-1] = -1.0f; //bad snake
                    for(unsigned int b=0; b<_player[player]->size(); b++)
                    {
                       // qDebug() << "b: " << b << "pos: "<<_player[player]->pos(b) << " | " << pos;
                        if(_player[player]->pos(b).x() == pos.x() && _player[player]->pos(b).y() == pos.y())
                        {
                            outputData[snake_layer][outputData[snake_layer].size()-1] = 1.0f; //good snake
                            break;
                        }
                    }
                    break;
                }
                case MapData::obsticle:
                {
                    outputData[obsticle_layer][outputData[obsticle_layer].size()-1] = 1;
                    break;
                }
                case MapData::nothing:
                {

                    break;
                }
            }
        }
    }
   //qDebug() << "AI_mapData: "<<outputData;
   //qDebug() << "AI_mapData";
    c = 0;
    /*for(unsigned int a=0; a<sqrt(rawData.size()); a++)
    {
        QString raw = "";
        QString food = "";
        QString snake = "";
        QString obsticle = "";
        for(unsigned int b=0; b<sqrt(rawData.size()); b++)
        {
            raw+=QString::number(rawData[c]);
            food+=QString::number(outputData[food_layer][c]);
            snake+=QString::number((int)outputData[snake_layer][c]);
            obsticle+=QString::number((int)outputData[obsticle_layer][c]);
            c++;
        }
        qDebug() << raw << "  |  " << food << "  |  " << snake << "  |  " << obsticle;
    }*/


    return outputData;

    /*for(unsigned int x=0; x<fieldOfView.size(); x++)
    {
        vector<QPoint> tmpLine;
        for(unsigned int y=0; y<fieldOfView[x].size(); y++)
        {
            //tmpLine.push_back(geometry::rotate(_map[pointOfView.x()+fieldOfView[x][y].x()][pointOfView.y()+fieldOfView[x][y].y()],))
            tmpLine.push_back(QPoint(pointOfView.x()fieldOfView[x].,pointOfView.y()));
        }
    }*/
}
vector<QPoint> Environment::rotate_90(vector<QPoint> data,QPoint rotPoint, int amount)
{
    if(amount == 0)
        return data;
    if(data.size() == 0)
        return data;
    QPoint tmpPoint;
    for(unsigned int b=0; b<amount; b++)
    {
        for(unsigned int a=0; a<data.size(); a++)
        {
            tmpPoint.setX(-(data[a].y()+rotPoint.x()));
            tmpPoint.setY(data[a].x()-rotPoint.y());
            data[a] = tmpPoint;
        }
    }
    return data;
}
