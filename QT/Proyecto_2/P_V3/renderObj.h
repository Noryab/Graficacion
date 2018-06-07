#ifndef RENDEROBJ_H
#define RENDEROBJ_H
#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>

#include "obj.h"



using namespace std;
using namespace Eigen;
#define PI 3.1416

class renderObj
{
    //Q_OBJECT
    public:        
    renderObj();

        ~renderObj();
        void RenderObj(ModeloOBJ &M_Obj, QPainter &painter);
        void Build();
        QVector3D Tracing( const QVector3D &rayorig,const QVector3D &raydir,const int depth);

    private:
    // Variables para pintar
    QPen colorPoint;
    QRgb CP_rgb;

    //Variables de camara
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

        int dpi ;
        int width ;
        int height ;
        int n ;
        int aadepth ;
        double aathreshold ;
        double aspectratio ;
        double ambientlight ;
        double accuracy ;

};

#endif // RENDEROBJ_H
