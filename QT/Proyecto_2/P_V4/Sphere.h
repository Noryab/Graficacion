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
    float Tran;
    float Spec;
    float Diff;

	public:
        Sphere ();
        Sphere (QVector3D _Center, float _Radius, QVector3D _Color,float _Tran,float _Spec,float _Diff);

        QVector3D getCenter(){return this->Center;}

        float getRadius(){return this->Radius;}
        float getTran(){return this->Tran;}
        float getSPec(){return this->Spec;}
        float getDiff(){return this->Diff;}

        virtual QVector3D getColor()
        {
            return this->Color;
        }
        virtual bool intersect(const QVector3D &rayorig, const QVector3D &raydir, float &t)
        {
            float t1 = INFINITY;
            //cout<<"Itersected Sphere."<<endl;
            //float t1 = 0;

            QVector3D l = this->Center - rayorig;
            //cout<<this->Center.x()<<endl;
            float tca = QVector3D::dotProduct(l,raydir);
            if (tca < 0){
                return false;
            }

            float d2 = QVector3D::dotProduct(l,l) - tca * tca;
            if (d2 > Radius*Radius){
                return false;
            }

            float thc = sqrt(Radius*Radius - d2);
            t = tca - thc;
            t1 = tca + thc;

            if (t < 0){
                t = t1;
            }

            return true;
        }

        virtual void getNormal(const QVector3D &InterPoint, const QVector3D &direction, QVector3D &normal)
        {
            normal = InterPoint - this->Center;
            normal.normalize();
        }


};



Sphere::Sphere ()
{
    this->Center = QVector3D(0,0,0);
    this->Radius = 1.0;
    this->Color  = QVector3D(0.6,0.6,0.5);
    this->Tran   =1;
    this->Spec   =0;
    this->Diff   =0;

}

Sphere::Sphere (QVector3D _Center, float _Radius, QVector3D _Color, float _Tran, float _Spec, float _Diff)
{
    this->Center = _Center;
    this->Radius = _Radius;
    this->Color  = _Color;
    this->Tran   =_Tran;
    this->Spec   =_Spec;
    this->Diff   =_Diff;
}






#endif
