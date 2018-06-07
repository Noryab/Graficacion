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

class renderObj:public QWidget
{
    //Q_OBJECT
    public:        
       explicit renderObj(QWidget *parent = 0);
        ~renderObj();
        void RenderObj();
        void Build();

        float Angle_X,Angle_Y,Angle_Z;
        bool BoolRot;

        int Width;
        int Height;

        ModeloOBJ M_Obj;

    private:
        QVector3D camOrig;
        QVector3D qPoint;
        float theta;

};

#endif // RENDEROBJ_H
