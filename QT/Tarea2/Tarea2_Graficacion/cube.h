#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>


class Cube
{

public:
    Cube(int high, int width, QPoint *PosOrigin);

    void drawCube(QPainter &painter);

    QPoint P1[8];
    QPoint P2[8];

private:
    void drawLine(QPainter &painter, QPoint P_1, QPoint P_2);

    int Lhigh;
    int Lwidth;

    int Delta_x;
    int Delta_y;
    int Long;

    float increment_x;
    float increment_y;

    float x;
    float y;

    QPoint PosO[2];

};

#endif // CUBE_H
