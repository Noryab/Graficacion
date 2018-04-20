#ifndef CAMARA_H
#define CAMARA_H


#include <QObject>
#include <QtGui>
#include <QtCore>

#include <iostream>
#include <Eigen/Dense>

#include <Eigen/Geometry>

using namespace std;
using namespace Eigen;

class camara
{
public:
    camara();
    camara(QVector3D camOrig,QVector3D qPoint,double angle);

public:
    Vector3d V_o;
    Vector3d V_q;
    Vector3d V_v;
    Vector3d V_t;
    Vector3d V_u;
    Vector3d V_w;
    Vector3d V_g;

    MatrixXd M_Mc;
    MatrixXd M_TC;
    VectorXd V_c;

    MatrixXd M_ProyPers;
    MatrixXd M_ProyOrt;
    MatrixXd Buffer_Z;

public:
    double theta;
    double xpmin,xpmax,ypmin,ypmax;
    double xdmin,xdmax,ydmin,ydmax;
    double sx;
        double sy;
        double cx;
        double cy;
};

#endif // CAMARA_H

