#include "snake.h"
#include "ui_snake.h"
#include <QDebug>
Snake::Snake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);



    _painter = new QPainter(this);
    _enviroment = new Enviroment(_painter);

    _updateTimer = new QTimer(this);
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    _updateTimer->start(200);
}


Snake::~Snake()
{
    delete ui;
}
void Snake::paintEvent(QPaintEvent *e)
{

    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);
    _painter->setRenderHints(QPainter::RenderHints(0x04),true);

    _enviroment->drawMap();

    _painter->end();
}
void Snake::timerEvent()
{
    _enviroment->update();
    if(_updateTimer->interval() != ui->speed_slider->value())
    {
        _updateTimer->setInterval(1000 - ui->speed_slider->value());
    }
    if(_enviroment->foodAmount() != ui->food_slider->value())
    {
        _enviroment->foodAmount(ui->food_slider->value());
    }
    ui->score_label->setText(QString::number(_enviroment->player()->steps()+_enviroment->player()->food()));
    ui->food_label->setText(QString::number(_enviroment->player()->food()));
    ui->steps_label->setText(QString::number(_enviroment->player()->steps()));
    this->update();
}

void Snake::keyPressEvent(QKeyEvent *e)
{
    switch(char(e->key()))
    {
        //CONTROL 1
        case 'W'://up
        {
            _enviroment->controlSnakeDirection(0,Direction::_up);
            break;
        }
        case 'A'://left
        {
            _enviroment->controlSnakeDirection(0,Direction::_left);
            break;
        }
        case 'S'://down
        {
            _enviroment->controlSnakeDirection(0,Direction::_down);
            break;
        }
        case 'D'://right
        {
            _enviroment->controlSnakeDirection(0,Direction::_right);
            break;
        }
        //CONTOL 2
        case 'I'://up
        {
            _enviroment->controlSnakeDirection(1,Direction::_up);
            break;
        }
        case 'J'://left
        {
            _enviroment->controlSnakeDirection(1,Direction::_left);
            break;
        }
        case 'K'://down
        {
            _enviroment->controlSnakeDirection(1,Direction::_down);
            break;
        }
        case 'L'://right
        {
            _enviroment->controlSnakeDirection(1,Direction::_right);
            break;
        }
        //------------------
        case 'F':
        {
            _enviroment->controlSnakeAddSize(0,1);
            break;
        }
        case 'G':
        {
            _enviroment->controlSnakeAddSize(0,-1);
            break;
        }
        case 'M':
        {
            _enviroment->controlSnakeDeath(0, false);
            _enviroment->controlSnakeDeath(1, false);
            break;
        }
        case 'V':
        {
            _enviroment->controlSnakeAddFood(0,500);
            break;
        }
    }
}
void Snake::keyReleaseEvent(QKeyEvent *e)
{

}
