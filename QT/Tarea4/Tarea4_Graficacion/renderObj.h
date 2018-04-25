#ifndef RENDEROBJ_H
#define RENDEROBJ_H
#include <QGLWidget>
#include <GL/glut.h>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "ilumination.h"
#include "obj.h"

using namespace std;
using namespace Eigen;
#define PI 3.1416

class renderObj:public QGLWidget
{
    //Q_OBJECT
    public:        
       explicit renderObj(QWidget *parent = 0);
        ~renderObj();
        void initializeGL();
        void paintGL();
        void resizeGL(int w,int h);
        void RenderObj();
        void OpenTextura();

        bool RecalMatri;
        bool Gouraud;
        bool Phong;
        bool Poligon;
        bool Textura;
        bool Mall;
        bool Punt;
        float Angle_X,Angle_Y,Angle_Z;
        bool BoolRot;

        GLuint texture[8]; //Para almacenar las posibles texturas
        QImage img[3];
        ModeloOBJ M_Obj;


    private:
        QVector3D camOrig;
        QVector3D qPoint;
        float theta;

        MatrixXd VQ;
        MatrixXd VQq;



};

#endif // RENDEROBJ_H
