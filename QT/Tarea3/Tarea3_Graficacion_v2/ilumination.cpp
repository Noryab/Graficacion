#include "ilumination.h"

Ilumination::Ilumination()
{
    VectLight=QVector3D(1,1,1);
    VectLight=VectLight.normalized();
}

Ilumination::~Ilumination(){

}

QRgb Ilumination::Ambient(QVector3D Normal,QRgb Color){

    QRgb ColorA;
    float N_Dot_L;

    N_Dot_L=QVector3D::dotProduct(Normal,VectLight);
    cout<<"N_Dot_L="<<N_Dot_L<<endl;

    /*cout<<"Normal x="<<Normal.x()<<endl;
    cout<<"Normal y="<<Normal.y()<<endl;
    cout<<"Normal z="<<Normal.z()<<endl;

    cout<<"VectLight x="<<VectLight.x()<<endl;
    cout<<"VectLight y="<<VectLight.y()<<endl;
    cout<<"VectLight z="<<VectLight.z()<<endl;*/

    if(N_Dot_L>=0){
        ColorA=(Color*N_Dot_L);
        //ColorA=qRgb(100*N_Dot_L,100*N_Dot_L,100*N_Dot_L);
    }else{
        ColorA=qRgb(0,0,255);
    }

    return ColorA;

}

int Ilumination::Diffuse(){

    NTh=pow(cos(Theta/2),P);

    V=(M-L)*NTh+L;

    return V;
}

QRgb Ilumination::Speculate(){

}

float AngleIncident(){

    float Angle;


    return Angle;
}
