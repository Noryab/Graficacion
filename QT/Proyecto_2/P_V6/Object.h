#ifndef _OBJECT_H
#define _OBJECT_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "math.h"

class Object {	
	public:
	Object ();
    virtual QVector3D getColor () { return QVector3D (0.0, 0.0, 0.0); }
    virtual float getAmb(){return 0;}
    virtual float getSpec(){return 0;}
    virtual float getDiff(){return 0;}

    virtual float intersect(const QVector3D &rayorig, const QVector3D &raydir)
    {
        return -1;
    }
    virtual void getNormal(const QVector3D &InterPoint, QVector3D &normal)
    {
        normal = QVector3D(0,0,0);
    }
};

Object::Object () {}

#endif
