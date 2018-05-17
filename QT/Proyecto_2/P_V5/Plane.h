#ifndef _Plane_H
#define _Plane_H

#include "math.h"
#include "Object.h"



class Plane : public Object {
    QVector3D normal;
	double distance;
    QVector3D color;
    float Amb;
    float Spec;
    float Diff;

	public:
	
	Plane ();    
    Plane (QVector3D, double, QVector3D,float _Amb,float _Spec,float _Diff);
	
	// method functions
	double getPlaneDistance () { return distance; }

    virtual float getAmb(){return this->Amb;}
    virtual float getSpec(){return this->Spec;}
    virtual float getDiff(){return this->Diff;}


    virtual QVector3D getColor ()
    {
        return color;
    }
	
    virtual void getNormal(const QVector3D &InterPoint, QVector3D &Normal)
    {
        Normal=normal.normalized();
	}
    virtual float intersect(const QVector3D &rayorig, const QVector3D &raydir)
    {
        double a = QVector3D::dotProduct(raydir,normal);
		
		if (a == 0) {
            return -1;
		}
		else {
            QVector3D Temp=(rayorig+(distance*normal));
            double b=QVector3D::dotProduct(normal,Temp);
            return -1*b/a;
		}
	}
	
};

Plane::Plane () {
    normal = QVector3D(1,0,0);
	distance = 0;
    color = QVector3D(0.5,0.5,0.5);
}

Plane::Plane (QVector3D normalValue, double distanceValue, QVector3D colorValue, float _Amb, float _Spec, float _Diff)
{
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
    Amb=_Amb;
    Spec=_Spec;
    Diff=_Diff;
}



#endif
