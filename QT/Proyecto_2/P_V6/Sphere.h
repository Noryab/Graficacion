#ifndef _SPHERE_H
#define _SPHERE_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "Object.h"
#include "math.h"

using namespace std;
class Sphere: public Object
{
    QVector3D Center;
    QVector3D Color;
    float Radius;
    float Amb;
    float Spec;
    float Diff;

	public:
        Sphere ();
        Sphere (QVector3D _Center, float _Radius, QVector3D _Color, float _Amb, float _Diff, float _Spec);

        QVector3D getCenter(){return this->Center;}

        virtual float getRadius(){return this->Radius;}
        virtual float getAmb(){return this->Amb;}
        virtual float getSpec(){return this->Spec;}
        virtual float getDiff(){return this->Diff;}

        virtual QVector3D getColor()
        {
            return this->Color;
        }
        virtual float intersect(const QVector3D &rayorig, const QVector3D &raydir)
        {
           double a = 1;
            double b = (2*(rayorig.x() - this->Center.x())*raydir.x()) + (2*(rayorig.y() - this->Center.y())*raydir.y()) + (2*(rayorig.z() - this->Center.z())*raydir.z());
            double c = pow(rayorig.x() - this->Center.x(), 2) + pow(rayorig.y() - this->Center.y(), 2) + pow(rayorig.z() - this->Center.z(), 2) - (this->Radius*this->Radius);

            double discriminant = b*b - 4*a*c;

            if (discriminant > 0) {
                double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;
                if (root_1 > 0) {
                    return root_1;
                }
                else {
                    double root_2 = ((sqrt(discriminant) - b)/2) - 0.000001;
                    return root_2;
                }
            }
            else {
                return -1;
            }
        }

        virtual void getNormal(const QVector3D &InterPoint, QVector3D &normal)
        {
            normal = (InterPoint-this->Center).normalized();
        }
};



Sphere::Sphere ()
{
    this->Center = QVector3D(0,0,0);
    this->Radius = 1.0;
    this->Color  = QVector3D(0.6,0.6,0.5);
    this->Amb   =1;
    this->Spec   =0;
    this->Diff   =0;

}

Sphere::Sphere (QVector3D _Center, float _Radius, QVector3D _Color, float _Amb, float _Diff,float _Spec)
{
    this->Center = _Center;
    this->Radius = _Radius;
    this->Color  = _Color;
    this->Amb   =_Amb;
    this->Spec   =_Spec;
    this->Diff   =_Diff;
}


#endif
