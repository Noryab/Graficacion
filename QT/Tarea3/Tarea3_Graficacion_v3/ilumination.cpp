#include "ilumination.h"

Ilumination::Ilumination()
{
    M=255;
    L=10;
    P=2;
    VectLight=QVector3D(10,20,-10);
    VectLight=VectLight.normalized();
}

Ilumination::~Ilumination(){

}

QRgb Ilumination::Ambient(QVector3D Normal,QRgb Color){

    QRgb ColorA;
    float N_Dot_L;

    N_Dot_L=QVector3D::dotProduct(Normal,VectLight);

    if(N_Dot_L>=0){
        //ColorA=(Color*N_Dot_L);
        ColorA=qRgb(15*N_Dot_L,15*N_Dot_L,255*N_Dot_L);
    }else{
        ColorA=qRgb(0,0,0);
    }
    return ColorA;

}


QRgb Ilumination::Speculate(QVector3D Normal,QVector3D Ob,QRgb Color){

    QRgb ColorA;
    float N_Dot_L;
    QVector3D h;
    Ke=0.4;
    float delta_e=1;
    float Ie;

    h=2*QVector3D::dotProduct(Normal,VectLight)*Normal-VectLight;
    Ie=Ke*delta_e*QVector3D::dotProduct(-Ob,h);
    ColorA=qRgb(0*Ie,0*Ie,200*Ie);
    //ColorA=Color*Ie;

    return ColorA;

}
