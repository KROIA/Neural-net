#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include "enviroment.h"
#include <QTimer>
#include <QKeyEvent>



namespace Ui {
    class Snake;
}

class Snake : public QMainWindow
{
        Q_OBJECT

    public:
        explicit Snake(QWidget *parent = nullptr);
        ~Snake();

    protected:
        void keyPressEvent(QKeyEvent *e);
        void keyReleaseEvent(QKeyEvent *e);

    public slots:
        void timerEvent();
    private slots:
        void paintEvent(QPaintEvent *e);


    private:
        Ui::Snake *ui;
        Enviroment *_enviroment;
        QPainter   *_painter;
        QTimer     *_updateTimer;

};

#endif // SNAKE_H
