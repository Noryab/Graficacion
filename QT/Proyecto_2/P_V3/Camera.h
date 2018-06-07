#ifndef _Camera_H
#define _Camera_H

#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>

class Camera {
    QVector3D campos, camdir, camright, camdown;
	
	public:
    Camera ();
    Camera (QVector3D, QVector3D, QVector3D, QVector3D);
	

    QVector3D getCameraPosition () { return campos; }
    QVector3D getCameraDirection () { return camdir; }
    QVector3D getCameraRight () { return camright; }
    QVector3D getCameraDown () { return camdown; }

};

Camera::Camera () {
    campos = QVector3D(0,0,0);
    camdir = QVector3D(0,0,1);
    camright = QVector3D(0,0,0);
    camdown = QVector3D(0,0,0);
}

Camera::Camera (QVector3D pos, QVector3D dir, QVector3D right, QVector3D down) {
    campos = pos;
    camdir = dir;
    camright = right;
    camdown = down;
}

#endif
