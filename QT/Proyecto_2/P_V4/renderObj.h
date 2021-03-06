#ifndef RENDEROBJ_H
#define RENDEROBJ_H

#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>


using namespace std;
//using namespace Eigen;
#define PI 3.1416

class renderObj
{
    public:        
        renderObj();
        ~renderObj();
        void RenderObj(QPainter &painter);

    private:
        QPen colorPoint;
        QRgb CP_rgb;

        QVector3D O;
        QVector3D X;
        QVector3D Y;
        QVector3D Z;

        QVector3D CamPos;
        QVector3D CamDir;
        QVector3D CamRight;
        QVector3D CamLeft;
        QVector3D CamDown;
        QVector3D CamUp;

        int width ;
        int height ;
        int n ;
        int depth;
        double aspectratio ;
        double ambientlight ;

};

#endif // RENDEROBJ_H
