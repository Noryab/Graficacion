#ifndef ILUMINATION_H
#define ILUMINATION_H

#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>

using namespace std;
#define PI 3.1416


class Ilumination
{
public:
    Ilumination();
    ~Ilumination();

    QVector3D VectLight;

    QRgb Speculate();
    QRgb Ambient(QVector3D Normal, QRgb Color);
    int Diffuse();

private:
    int V;
    int M,L;
    float Theta;
    float P,NTh;
};

#endif // ILUMINATION_H
