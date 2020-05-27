// Autor    Alex Krieg
// Version  00.01.00
// Datum    28.05.2019

#include "rect.h"

Rect::Rect(QPainter *p)
{
    painter = p;
    color(QColor(0,255,0));
    frameColor(color());
    frameSize(3);
    begin(QPoint(0,0));
    end(QPoint(100,100));
    angle(0);
    rotatePos(RotationPos::middle);
    rotationPoint(QPoint(0,0));
    middlePoint(QPointF(0,0));
    drawPos(QPoint(0,0));
    frame(true);
    infill(true);
}
Rect::~Rect()
{

}

void Rect::color(QColor col)
{
    _color = col;
}
QColor Rect::color()
{
    return _color;
}
void Rect::frameColor(QColor col)
{
    _frameColor = col;
}
QColor Rect::frameColor()
{
    return _frameColor;
}
void Rect::frameSize(int size)
{
    if(size < 0)
    {
        size = 0;
    }
    _frameSize = size;
}
int Rect::frameSize()
{
    return _frameSize;
}
void Rect::begin(QPoint pos)
{
    _posA = pos;
}
QPoint Rect::begin()
{
    return _posA;
}
void Rect::end(QPoint pos)
{
    _posB = pos;
}
QPoint Rect::end()
{
    return _posB;
}
void   Rect::angle(geometry::Angle angle)
{
    _angle = angle;
}
void   Rect::angle(float degree)
{
    _angle.degree(degree);
}
float  Rect::angle()
{
    return _angle.degree();
}
void   Rect::rotate(float deltaDegree)
{
    _angle+deltaDegree;
}
void   Rect::rotatePos(int rotPos)
{
    if(rotPos < 0 || rotPos > 4)
    {
        qDebug() << "Rect::rotatePos("<<rotPos<<") out of range. Min: 0 Max: 4";
        return;
    }
    _rotationPos = rotPos;
}
void   Rect::rotationPoint(QPoint rotationPoint)
{
    _rotationPoint = rotationPoint;
}
QPoint Rect::rotationPoint()
{
    return _rotationPoint;
}
void   Rect::drawPos(QPoint pos)
{
    _drawPos = pos;
}
QPoint Rect::drawPos()
{
    return _drawPos;
}
void Rect::frame(bool enable)
{
    _frameEnable = enable;
}
bool Rect::frame()
{
    return _frameEnable;
}
void Rect::infill(bool enable)
{
    _infillEnable = enable;
}
bool Rect::infill()
{
    return _infillEnable;
}

void Rect::draw()
{
      QRect rect(_posA,_posB);

      if(_infillEnable)
      {
          QBrush brush(_color);
          painter->setBrush(brush);
      }
      QPen linepen(_frameColor);
      if(!_frameEnable)
      {
          linepen = QColor(0,0,0,0);
      }
      linepen.setWidth(_frameSize);
      painter->setPen(linepen);

    //  painter->translate(_drawPos);
      QPoint rotOffset;
      switch(_rotationPos)
      {
          case middle:
          {
              rotOffset = QPoint(rect.width()/2,rect.height()/2);
              break;
          }
          case cornerUL:
          {
              rotOffset = QPoint(0,0);
              break;
          }
          case cornerUR:
          {
              rotOffset = QPoint(rect.width(),0);
              break;
          }
          case cornerDL:
          {
              rotOffset = QPoint(0,rect.height());
              break;
          }
          case cornerDR:
          {
              rotOffset = QPoint(rect.width(),rect.height());
              break;
          }
      }
      //painter->translate(QPoint(/*_drawPos.x() + */rotOffset.x(),/*_drawPos.y()+*/rotOffset.y()));
      //painter->rotate(qreal(_angle.degree()));
      painter->translate(QPoint(/*rotOffset.x()+*/_drawPos.x(),/*rotOffset.y()+*/_drawPos.y()));

      //painter->translate(QPoint(-rotOffset.x(),-rotOffset.y()));
      //painter->translate(QPoint(_drawPos.x() - rotOffset.x(),_drawPos.y()-rotOffset.y()));
      if(_frameEnable || _infillEnable)
      {
        painter->rotate(-qreal(_angle.degree()));
        painter->translate(QPoint(-rotOffset.x()+_rotationPoint.x()-_middlePointOffset.x(),-rotOffset.y()+_rotationPoint.y()-_middlePointOffset.y()));
        painter->drawRect(rect);
        painter->translate(QPoint(rotOffset.x()-_rotationPoint.x()+_middlePointOffset.x(),rotOffset.y()-_rotationPoint.y()+_middlePointOffset.x()));
        painter->rotate(qreal(_angle.degree()));
      }
      painter->translate(QPoint(/*_drawPos.x() + *//*-rotOffset.x()*/-_drawPos.x(),/*_drawPos.y()+*//*-rotOffset.y()*/-_drawPos.y()));

      //painter->rotate(-qreal(_angle.degree()));
      //painter->translate(QPoint(-_drawPos.x(),-_drawPos.y()));
}
void        Rect::middlePoint(QPointF middlePoint)
{
    _middlePoint = middlePoint;

    _middlePointOffset = QPoint(_middlePoint.x()*(float)(end().x()-begin().x()),_middlePoint.y()*(float)(end().y()-begin().y()));
}
QPointF     Rect::middlePoint()
{
    return _middlePoint;
}
