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
        void resizeGL(int widht, int height);
        void RenderObj();
        void OpenTexture();

        bool RecalMatri;
        bool Gouraud;
        bool Phong;
        bool Poligon;
        bool Texture;
        bool Mall;
        bool Punt;
        float Angle_X,Angle_Y,Angle_Z;
        bool BoolRot;
        int numTexture;

        GLuint texture[8];
        QImage imag[5];
        ModeloOBJ M_Obj;


    private:
        QVector3D camOrig;
        QVector3D qPoint;
        float theta;

};

#endif // RENDEROBJ_H
