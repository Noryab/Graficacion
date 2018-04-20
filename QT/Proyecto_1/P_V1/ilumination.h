#ifndef ILUMINATION_H
#define ILUMINATION_H

#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "obj.h"

using namespace std;
#define PI 3.1416


class Ilumination
{
public:
    Ilumination();
    Ilumination(QVector3D PosW,QRgb Color);

    QVector3D v_Luz;
    QRgb ColorLuz;
    QRgb Light(ModeloOBJ &M_Obj,QVector3D Normal, QVector3D positionCam, QVector3D Ocam);

private:    
    int cR,cG,cB;
    int cLR,cLG,cLB;
    float cMR,cMG,cMB;
    double PosX, PosY, PosZ;
    float Le;//Especular
    float La;//Ambiental
    float Ld;//Difusa
    float Ia;//Difusa

    int V;
    int M,L;
    float Theta;
    float P,NTh;
};

#endif // ILUMINATION_H
