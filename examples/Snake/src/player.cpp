#include "player.h"



Player::Player(unsigned int player,QSize mapSize)
{
    _mapSize = mapSize;
    playerIndex(player);
    _presetSize = 5;
    _foodPerTile = 20;
    _maxSteps = 5000;
    _presetFood = _presetSize * _foodPerTile;
    standardColor(QColor(0,200,0));
    globalView(false);
    killreward(false);
    resetDeathCount();

    _death = true;
    revive();
}
Player::~Player()
{

}
void Player::playerIndex(unsigned int index)
{
    _playerIndex = index;
}
unsigned int Player::playerIndex()
{
    return _playerIndex;
}
void Player::mapSize(QSize mapSize)
{
    _mapSize = mapSize;
}
QSize Player::mapsize()
{
    return _mapSize;
}

void Player::sollSize(int size)
{
    if(size >= 2)
        _size = size;
    else
        _size = 2;
}
int Player::sollSize()
{
    return _size;
}
unsigned int Player::size()
{
    return _playerPos.size();
}
void Player::addSize(int increment)
{
    sollSize(sollSize() + increment);
}
void Player::food(long int food)
{
    _food = food;
}
long int Player::food()
{
    return _food;
}
void Player::addFood(int increment)
{
    food(food() + increment);
}

void Player::pos(std::vector<QPoint> pos)
{
    if(pos.size() >= 2)
    {
        _playerPos = pos;
        sollSize(_playerPos.size());
    }
}
QPoint *Player::pos(unsigned int index)
{
    if(_playerPos.size() <= index)
    {
        if(_playerPos.size() == 0)
        {
            throw std::runtime_error("Player::pos(unsigned int ["+std::to_string(index)+"]) _playerPos.size() == 0");
        }
        throw std::runtime_error("Player::pos(unsigned int ["+std::to_string(index)+"]) out of range : 0 - "+std::to_string(_playerPos.size()-1));
    }
    return &_playerPos[index];
}
std::vector<QPoint> *Player::pos()
{
    return &_playerPos;
}
void Player::standardColor(QColor standard)
{
    _standardBodyColor = standard;
}
QColor Player::standardColor()
{
    return _standardBodyColor;
}
QColor Player::color(unsigned int index)
{
    if(_playerColor.size() <= index)
    {
        return _standardBodyColor;
    }
    return _playerColor[index];
}
std::vector<QColor> Player::color()
{
    return _playerColor;
}


void Player::update()
{
    if(_death)
        return;
    if(_playerPos.size() > _size)
    {
        _playerPos.pop_back();
        _playerColor.pop_back();
    }else if(_playerPos.size() < _size)
    {
        _playerPos.push_back(_lastPos);
        _playerColor.push_back(_lastColor);
    }

    //----------Shift

    _lastPos   = _playerPos[_playerPos.size()-1];
    _lastColor = _playerColor[_playerColor.size()-1];
    for(int a=_playerPos.size()-2; a>=0; a--)
    {
        _playerPos[a+1] = _playerPos[a];
        _playerColor[a+1] = _playerColor[a];
    }
    _playerColor[0] = _standardBodyColor;
    switch (_direction)
    {
        case Direction::_up:
        {
            _playerPos[0] = QPoint(_playerPos[1].x(),_playerPos[1].y()-1);
            break;
        }
        case Direction::_left:
        {
            _playerPos[0] = QPoint(_playerPos[1].x()-1,_playerPos[1].y());
            break;
        }
        case Direction::_down:
        {
            _playerPos[0] = QPoint(_playerPos[1].x(),_playerPos[1].y()+1);
            break;
        }
        case Direction::_right:
        {
            _playerPos[0] = QPoint(_playerPos[1].x()+1,_playerPos[1].y());
            break;
        }
    }
   // qDebug() << "ges food: " << food();
    int tmpFood = food();
    if(tmpFood < 0)
        tmpFood = 0;
    for(int a=0; a<size(); a++)
    {
        double colorFactor;
        if(tmpFood / _foodPerTile != 0)
        {
            colorFactor = 1;
        }
        else
        {
            colorFactor =  (double)(tmpFood % _foodPerTile)/_foodPerTile;
        }
        //qDebug() << "food index: " << a << " : " << colorFactor << "tmpFood: " << tmpFood;
       // colorFactor = 1.f - colorFactor;
        tmpFood-=_foodPerTile;
        if(tmpFood < 0)
        {
            tmpFood = 0;
        }
        if(colorFactor <= 0.01)
        {
            sollSize(size()-1);
        }
        _playerColor[a] = QColor(255-(double)255*colorFactor,(double)_standardBodyColor.green()*colorFactor,(double)_standardBodyColor.blue()*colorFactor);
        //_playerColor[a] = QColor(0,0,0);
    }

    addFood(-1);
    if(food() <= 0)
    {
     //   qDebug() << "starved: "<< _playerIndex;
     //   emit starved(_playerIndex);
        this->kill();
    }
    if(food() / _foodPerTile > size()-1)
    {
        addSize(1);
    }
    checkForSelfCollision();
    _steps++;
    if(_steps >= _maxSteps)
    {
        kill();
    }
    //---------------
}
void Player::globalView(bool globalView)
{
    _glovalView = globalView;
}
bool Player::globalView()
{
    return _glovalView;
}
void Player::direction(Direction dir)
{
    if(_glovalView){
        if(_direction == dir)
            return;
        if((_direction+dir)%2 == 0)// check if direction is opposite
            return;

        addFood(-2);
        _direction = dir;
    }
    else {
        if(dir == Direction::_up)
            return;
        addFood(-2);
        _direction = (_direction + dir)%4;
    }
}
Direction Player::direction()
{
    return (Direction)_direction;
}
void Player::checkForSelfCollision()
{
    if(_death)
        return;

    std::vector<QPoint> collisionPos;
    for(unsigned int a=0; a<size(); a++)
    {
        for(unsigned int b=a+1; b<size(); b++)
        {
            //qDebug() << "check: "<<a<<" to: "<<b;
            if(_playerPos[a].x() == _playerPos[b].x() &&
               _playerPos[a].y() == _playerPos[b].y())
            {
              //  qDebug() << "collision: "<< _playerPos[a];
                collisionPos.push_back(_playerPos[a]);
            }
        }
    }
    if(collisionPos.size() != 0)
    {
       // qDebug() << "collision: "<< _playerIndex;
       // emit collision(_playerIndex,collisionPos);
        this->kill();
    }
}
void Player::kill()
{
    /*
        qDebug() << "Killed Player: " << _playerIndex;
        qDebug() << "  Steps: " << steps();
        qDebug() << "  Food: " << food();
        qDebug() << "  Size: " << size();
    */
    if(_death)
        return;

    _death = true;
    _playerPos.clear();
    _playerColor.clear();
    if(steps() == 0)
    {
        return;
    }
    _deathCount++;

    _scoreList.push_back(currentScore());
}
void Player::revive()
{
    if(!_death)
        return;

    _death = false;

    sollSize(_presetSize);
    food(_presetFood);
    //qDebug() << "food: " << food() << " " << _presetFood;
    _steps = 0;

    _direction = rand()%4;
    //_playerPos = std::vector<QPoint>(sollSize());
    _lastPos = QPoint(5+rand() %(_mapSize.width()-_mapSize.width()/10),5+rand()%(_mapSize.height()-_mapSize.height()/10));
    while(_playerPos.size() < sollSize())
    {
        _playerPos.push_back(_lastPos);
        _playerColor.push_back(_standardBodyColor);
    }


    _playerColor = std::vector<QColor>();
    _playerColor.reserve(sollSize());
    for(int a=0; a<sollSize(); a++)
    {
        _playerColor.push_back(QColor());
    }
    _playerColor[0] = _standardBodyColor;


    for(unsigned int a=1; a<size(); a++)
    {
        _playerPos[a] = QPoint(_playerPos[a-1].x()-1,_playerPos[a-1].y());
        _playerColor[a] = _standardBodyColor;
    }
}
bool Player::isAlive()
{
    return !_death;
}
unsigned int Player::steps()
{
    return _steps;
}

void Player::killreward(bool enable)
{
    _killreward = enable;
}
bool Player::killreward()
{
    return _killreward;
}
unsigned int Player::deathCount()
{
    return _deathCount;
}
void Player::resetDeathCount()
{
    _deathCount = 0;
}
std::vector<struct Score> Player::score()
{
    return _scoreList;
}
struct Score Player::score(unsigned int index)
{
    struct Score ret;
    ret.food = 0;
    ret.steps = 0;
    if(_scoreList.size() <= index)
        return ret;

    ret = _scoreList[index];
    return ret;
}
struct Score Player::lastScore()
{
    struct Score ret;
    ret.food = 0;
    ret.steps = 0;
    if(_scoreList.size() == 0)
        return ret;

    return score(_scoreList.size()-1);
}
struct Score Player::currentScore()
{
    Score currentScore;
    currentScore.food = food();
    currentScore.steps = steps();
    return currentScore;
}
struct Score Player::averageScoreOverTheLast(unsigned int lastAmount)
{
    Score retScore;
    retScore.food = 0;
    retScore.steps = 0;
    if(_scoreList.size() < lastAmount || lastAmount == 0)
    {
        lastAmount = _scoreList.size();
        if(lastAmount == 0)
            return retScore;
    }
    for(unsigned int a=0; a<lastAmount; a++)
    {
        retScore.food += _scoreList[_scoreList.size() -1 -a].food / lastAmount;
        retScore.steps += _scoreList[_scoreList.size() -1 -a].steps / lastAmount;
    }
    return retScore;
}

void Player::resetScore()
{
    _scoreList.clear();
}
struct Score Player::averageScore()
{
    Score tmpScore;
    tmpScore.food = 0;
    tmpScore.steps = 0;
    if(_scoreList.size() == 0)
        return tmpScore;
    for(unsigned int a=0; a<_scoreList.size(); a++)
    {
        tmpScore.food += _scoreList[a].food / _scoreList.size();
        tmpScore.steps += _scoreList[a].steps / _scoreList.size();
    }
    return tmpScore;
}
struct Score Player::averageScore(std::vector<Score> scoreList)
{
    Score averageScore;
    averageScore.food = 0;
    averageScore.steps = 0;
    if(scoreList.size() == 0)
        return averageScore;
    for(unsigned int a=0; a<scoreList.size(); a++)
    {
        averageScore.food += scoreList[a].food / scoreList.size();
        averageScore.steps += scoreList[a].steps / scoreList.size();
    }
    return averageScore;
}

struct Score Player::addedUpScore()
{
    Score tmpScore;
    tmpScore.food = 0;
    tmpScore.steps = 0;
    if(_scoreList.size() == 0)
        return tmpScore;
    for(unsigned int a=0; a<_scoreList.size(); a++)
    {
        tmpScore.food += _scoreList[a].food;
        tmpScore.steps += _scoreList[a].steps;
    }
    return tmpScore;
}
