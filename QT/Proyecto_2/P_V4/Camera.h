#ifndef _Camera_H
#define _Camera_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "math.h"

class Camera
{
	public:	
        Camera ();
        Camera (QVector3D _Cpos, QVector3D _Cdir, QVector3D _Cright, QVector3D _Cdown);

        QVector3D getCameraPosition () { return CamPos; }
        QVector3D getCameraDirection () { return CamDir; }
        QVector3D getCameraRight () { return CamRight; }
        QVector3D getCameraDown () { return CamDown; }

    private:
        QVector3D CamPos;
        QVector3D CamDir;
        QVector3D CamRight;
        QVector3D CamDown;

};

Camera::Camera ()
{
    CamPos = QVector3D(0,0,0);
    CamDir = QVector3D(0,0,1);
    CamRight = QVector3D(0,0,0);
    CamDown = QVector3D(0,0,0);
}

Camera::Camera (QVector3D _Cpos, QVector3D _Cdir, QVector3D _Cright, QVector3D _Cdown)
{
    CamPos = _Cpos;
    CamDir = _Cdir;
    CamRight = _Cright;
    CamDown = _Cdown;
}

#endif
