#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>

using namespace std;
#define PI 3.1416

class Cube{
    public:
        Cube(float l, QVector3D PosOrigin);
        Cube(float l, QVector3D PosOrigin, QColor NodeColor, QColor EdgeColor);
        ~Cube();

        void DrawCube(QPainter &Painter);
        //void RDraw(QPainter &Painter);
        void RotateCubeX(float _Theta);
        void RotateCubeY(float _Phi);
        void RotateCubeZ(float _Alpha);
    private:
        void CrossProduct();
        QVector3D PosO;     /// (x,y,z)
        QVector3D Nodes[8]; /// (Xi,Yj,Zk)
        QVector3D NodesV[8]; /// (Xi,Yj,Zk)
        QPoint edges[8];    /// Nodes[i]-Nodes[j]
        QVector4D Face[6];    ///
        QVector3D Normal[6];

        QColor backgroundColor;
        QColor NodeColor;
        QColor EdgeColor;
        QSize NodeSize;

        float Theta;
        float Phi;
        float Alpha;
        float L; /// Size sides of Cube
};



#endif // CUBE_H

class Line
{
    public:
        Line();
        Line(int Widht,QColor Edge,QColor Nodecolor);

        void DrawLine(QPainter &Painter, QVector2D P_1, QVector2D P_2);

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
