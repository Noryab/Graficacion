#ifndef _OBJECT_H
#define _OBJECT_H


#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>

#include "Ray.h"
#include "Color.h"

class Object {	
	public:
	Object ();
	
    virtual QColor getColor () { return QColor (0.0, 0.0, 0.0, 0); }
	
    virtual QVector3D getNormalAt(QVector3D intersection_position)
    {
        return QVector3D (0, 0, 0);
	}
	
	virtual double findIntersection(Ray ray) {
		return 0;
	}
	
};

Object::Object () {}

#endif
