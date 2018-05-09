#ifndef _Plane_H
#define _Plane_H

#include "math.h"
#include "Object.h"



class Plane : public Object {
    QVector3D normal;
	double distance;
    QVector3D color;
	
	public:
	
	Plane ();
	
    Plane (QVector3D, double, QVector3D);
	
	// method functions
    QVector3D getPlaneNormal () { return normal; }
	double getPlaneDistance () { return distance; }

    virtual QVector3D getColor ()
    {
        return color;
    }
	
    virtual void getNormal(const QVector3D &InterPoint, const QVector3D &direction, QVector3D &Normal)
    {
        Normal=normal.normalized();
	}
    virtual bool intersect(const QVector3D &rayorig, const QVector3D &raydir, float &t)
    {
        double a = QVector3D::dotProduct(raydir,normal);
		
		if (a == 0) {
			// ray is parallel to the plane
            return false;
		}
		else {
            QVector3D Temp=-(rayorig+(distance*normal));
            double b=QVector3D::dotProduct(normal,Temp);
            t=-1*b/a;
            return true;
		}
	}
	
};

Plane::Plane () {
    normal = QVector3D(1,0,0);
	distance = 0;
    color = QVector3D(0.5,0.5,0.5);
}

Plane::Plane (QVector3D normalValue, double distanceValue, QVector3D colorValue) {
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
