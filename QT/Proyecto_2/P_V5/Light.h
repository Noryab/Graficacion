#ifndef _Light_H
#define _Light_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "math.h"

class Light {
    QVector3D position;
    QVector3D color;
	
	public:
	
	Light ();
    Light (QVector3D, QVector3D);
	
	// method functions
    virtual QVector3D getLightPosition () { return position; }
    virtual QVector3D getLightColor () { return color; }
	
};

Light::Light () {
    position = QVector3D(0,0,0);
    color = QVector3D(1,1,1);
}

Light::Light (QVector3D p, QVector3D c) {
	position = p;
	color = c;
}

#endif
