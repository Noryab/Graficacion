#include "camara.h"

camara::camara()
{
    int NumCoorHom=4;
    int NumCoorNoHom=3;

    theta=0.5;

    V_o=Vector3d::Zero(NumCoorNoHom,1);
        V_o(0)=0;
        V_o(1)=1;
        V_o(2)=1;
    V_q=Vector3d::Zero(NumCoorNoHom,1);
        V_q(0)=0;
        V_q(1)=1;
        V_q(2)=-1;
    V_t=Vector3d::Zero(NumCoorNoHom,1);
            V_t(0)=0;
            V_t(1)=-1;
            V_t(2)=0;

    V_t=V_t.normalized();
    V_w=V_o-V_q;
    V_w=V_w.normalized();
    V_g=-V_w;
    V_u=V_t.cross(V_w);
    V_u=V_u.normalized();
    V_v=V_w.cross(V_u);

    M_Mc=MatrixXd::Zero(NumCoorNoHom,NumCoorNoHom);
    M_Mc<< V_u, V_v, V_w;

    M_TC=MatrixXd::Zero(NumCoorHom,NumCoorHom);
    M_TC<<M_Mc.transpose(), -M_Mc.transpose()*V_o, MatrixXd::Zero(1,3), 1;

    M_ProyOrt=MatrixXd::Zero(NumCoorHom,NumCoorHom);
    M_ProyOrt<<  1,0,0,0,
                0,1,0,0,
                0,0,0,0,
                0,0,0,1;

    /*float l=3,c=1,f=4;
    M_ProyOrt=MatrixXd::Zero(NumCoorHom,NumCoorHom);
    M_ProyOrt<<  1,0,0,0,
                0,1,0,0,
                0,0,(l/(l-c)),(-f*l/(l-c)),
                0,0,0,1;*/


    /*M_ProyPers=MatrixXd::Identity(NumCoorHom,NumCoorHom);
    MatProy_Pers(0,0)=f/w;
    MatProy_Pers(1,1)=f/h;
    MatProy_Pers(2,2)=le/(le-c);
    MatProy_Pers(3,2)=1.0;
    MatProy_Pers(2,3)=-f*le/(le-c);*/

    xpmin=-2.8,xpmax=2.8,ypmin=-2.8,ypmax=2.8;
    xdmin=0.0,xdmax=800.0, ydmin=0.0,ydmax=800.0;
    sx=(xdmax-xdmin)/(xpmax-xpmin);
    sy=(ydmax-ydmin)/(ypmax-ypmin);
    cx=sx*(-xpmin)+xdmin;
    cy=sy*(-ypmin)+ydmin;
}

camara::camara(QVector3D camOrig,QVector3D qPoint,double angle){

    int NumCoorHom=4;
    int NumCoorNoHom=3;

    V_o=Vector3d::Zero(NumCoorNoHom,1);
        V_o(0)=camOrig.x();
        V_o(1)=camOrig.y();
        V_o(2)=camOrig.z();
    V_q=Vector3d::Zero(NumCoorNoHom,1);
        V_q(0)=qPoint.x();
        V_q(1)=qPoint.y();
        V_q(2)=qPoint.z();
    V_t=Vector3d::Zero(NumCoorNoHom,1);
            V_t(0)=0;
            V_t(1)=1;
            V_t(2)=0;

    V_w=V_o-V_q;
    V_w.normalize();
    V_g=-V_w;
    V_u=V_t.cross(V_w);
    V_u.normalize();
    V_v=V_w.cross(V_u);

    M_Mc=MatrixXd::Zero(NumCoorNoHom,NumCoorNoHom);
    M_Mc<< V_u, V_v, V_w;

    M_TC=MatrixXd::Zero(NumCoorHom,NumCoorHom);
    M_TC<<M_Mc.transpose(), -M_Mc.transpose()*V_o, MatrixXd::Zero(1,3), 1;

    M_ProyOrt=MatrixXd::Zero(NumCoorHom,NumCoorHom);
    M_ProyOrt<<  1,0,0,0,
                0,1,0,0,
                0,0,0,0,
                0,0,0,1;

    //M_ProPers=MatrixXd::Identity(4,4);
    /*MatProy_Pers(0,0)=f/w;
    MatProy_Pers(1,1)=f/h;
    MatProy_Pers(2,2)=le/(le-c);
    MatProy_Pers(3,2)=1.0;
    MatProy_Pers(2,3)=-f*le/(le-c);*/

    xpmin=-2.0,xpmax=2.0,ypmin=-2.0,ypmax=2.0;
    xdmin=0.0,xdmax=500.0, ydmin=0.0,ydmax=500.0;
    sx=(xdmax-xdmin)/(xpmax-xpmin);
    sy=(ydmax-ydmin)/(ypmax-xpmin);
    cx=sx*-xpmin+xdmin;
    cy=sy*-ypmin+ydmin;
}

