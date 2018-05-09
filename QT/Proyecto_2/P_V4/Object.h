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
	
	// method functions
    virtual QVector3D getColor () { return QVector3D (0.0, 0.0, 0.0); }
	
    virtual bool intersect(const QVector3D &rayorig, const QVector3D &raydir, float &t)
    {
        cout<<"Itersected Object."<<endl;
        return true;
    }
    virtual void getNormal(const QVector3D &InterPoint, const QVector3D &direction, QVector3D &normal)
    {
        normal = InterPoint - QVector3D(0,0,0);
        normal.normalize();
    }
};

Object::Object () {}

#endif
