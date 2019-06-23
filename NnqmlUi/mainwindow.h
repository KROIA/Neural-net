#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtQuick/QQuickPaintedItem>

class Mainwindow: public QQuickPaintedItem
{
public:
    Mainwindow(QQuickItem *parent = 0);
    ~Mainwindow();
    void paint(QPainter *painter);
};

#endif // MAINWINDOW_H
