#ifndef RENDEROBJ_H
#define RENDEROBJ_H

#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "ilumination.h"
#include "obj.h"
#include "camara.h"

using namespace std;
using namespace Eigen;
#define PI 3.1416

class renderObj{
    public:        
        renderObj();

        void RenderObj(ModeloOBJ &M_Obj, QPainter &painter);
        void DrawCube(QPainter &Painter);        
        camara *camaraObj;

        bool RecalMatri;
        bool Gouraud;
        bool Phong;
        bool Poligon;
        bool Mall;
        bool Punt;

    private:
        QVector2D Proy_Disp(MatrixXd Xp);        
        void ScanConversion(QPainter &painter, QVector2D P, QVector2D Q);
        bool verifBuffer(ModeloOBJ &M_Obj,int idx,int i,int j);
        void Reproject(ModeloOBJ &M_Obj);
        int Count;
        int CoordX[4500];
        int CoordY[4500];
        float Dot;        

        QVector3D camOrig;
        QVector3D qPoint;
        float theta;

        MatrixXd VQ;
        MatrixXd VQq;
        MatrixXd Buffer_Z;
};


class Line
{
    public:        
        Line(QPainter &painter,int Widht, QRgb Edge);

        void DrawLine(QPainter &painter, QVector2D P_1, QVector2D P_2);
        void DrawLineGouraud(QPainter &painter, QVector2D P_1, QVector2D P_2);
        int randNumber(int low, int high);
        QRgb cV[3];   // Gouraud
        QVector2D pV[3];   // Gouraud
        QRgb interpolacionBilinealColor(QVector2D P, QVector2D Q, float x, float y);
        QRgb interpolacionBilinealNormal(QVector3D N1, QVector3D N2, float x, float y);


    private:
        int Widht;
        float Delta_x;
        float Delta_y;
        float Long;
        float increment_x;
        float increment_y;
        float x;
        float y;
        QPen colorPoint;
};


#endif // RENDEROBJ_H
