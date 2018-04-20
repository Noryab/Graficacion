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
        void Poligonal(ModeloOBJ &M_Obj,QPainter &painter);
        camara *camaraObj;

        bool RecalMatri;
        bool Gouraud;
        bool Phong;
        bool Poligon;
        bool Textura;
        bool Mall;
        bool Punt;
        float zLine[5000];

    private:
        QVector2D Proy_Disp(MatrixXd Xp);        
        void ScanConversion(QVector2D P, QVector2D Q, float z1, float z2);
        void Reproject(ModeloOBJ &M_Obj);
        int Count;
        int CoordX[1500];
        int CoordY[1500];
        float Dot;        

        QVector3D camOrig;
        QVector3D qPoint;
        float theta;

        MatrixXd VQ;
        MatrixXd VQq;

};


class Line
{
    public:        
        Line(QPainter &painter,int Widht, QRgb Edge);

        void DrawLine(QPainter &painter, QVector2D P_1, QVector2D P_2, float z1,float z2);
        void DrawLineGouraud(QPainter &painter, QVector2D P_1, QVector2D P_2, float z1,float z2);
        void DrawLinePhong(ModeloOBJ &M_Obj,QPainter &painter, QVector2D P_1, QVector2D P_2, float z1,float z2);
        void DrawLineTextura(QPainter &painter, QVector2D P_1, QVector2D P_2, float z1,float z2,Ilumination LightW,Ilumination LightR,QVector3D V_NormalC);
        int randNumber(int low, int high);
        QVector3D nV[3];   // phong
        QRgb cV[3];   // Gouraud
        QVector2D pV[3];   // Gouraud
        QRgb interpolacionColor(QVector2D P, QVector2D Q, float x_, float y_);
        QVector3D interpolacionNormal(QVector3D N1, QVector3D N2, float x_, float y_);
        bool verifBuffer(int i,int j,float z);
        MatrixXd Buffer_Z;

        QImage textura;
        int texW;
        int texH;


        QVector2D MapTextura(double x_, double y_);

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
