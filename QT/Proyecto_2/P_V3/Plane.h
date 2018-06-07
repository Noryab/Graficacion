#ifndef _Plane_H
#define _Plane_H


#include <QWidget>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <memory>

#include "math.h"
#include "Object.h"
#include "Color.h"

class Plane : public Object {
    QVector3D normal;
	double distance;
    QColor color;
	
	public:
	
	Plane ();
	
    Plane (QVector3D, double, QColor);
	
	// method functions
    QVector3D getPlaneNormal () { return normal; }
	double getPlaneDistance () { return distance; }
    virtual QColor getColor () { return color; }
	
    virtual QVector3D getNormalAt(QVector3D point) {
		return normal;
	}
	
	virtual double findIntersection(Ray ray) {
        QVector3D ray_direction = ray.getRayDirection();
		
        double a = QVector3D::dotProduct(ray_direction, normal);

		if (a == 0) {
			// ray is parallel to the plane
			return -1;
		}
        else {
            //double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
            double b = QVector3D::dotProduct(normal,(ray_direction+(-1*normal*distance)));
			return -1*b/a;
		}
	}
	
};

Plane::Plane () {
    normal = QVector3D(1,0,0);
	distance = 0;
    color = QColor(0.5,0.5,0.5, 0);
}

Plane::Plane (QVector3D normalValue, double distanceValue, QColor colorValue) {
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
