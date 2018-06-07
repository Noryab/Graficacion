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

    private:
        QVector3D camOrig;
        QVector3D qPoint;
        float theta;

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
