#include "ilumination.h"

Ilumination::Ilumination(){}
Ilumination::Ilumination(QVector3D PosW, QRgb Color)
{
    v_Luz=PosW; ///Posicion de la luz
    v_Luz.normalize();
    ColorLuz=Color;/// Color de la luz
    La=25;
    Le=0.001;
    Ld=0.01;
    Ia=0.6;

    M=255;
    L=10;
    P=2;
}

QRgb Ilumination::Light(ModeloOBJ &M_Obj, QVector3D Normal, QVector3D positionCam, QVector3D Ocam){

    float N_Dot_L;    
    float O_Dot_H;

    QRgb ColorA;
    QRgb ColorD;
    QRgb ColorE;
    QRgb ColorF;
    QVector3D h;
    cLR=qRed(ColorLuz);cLG=qGreen(ColorLuz);cLB=qBlue(ColorLuz);

    float d_FacetoLight;
    d_FacetoLight=sqrt(pow(Normal.x()-positionCam.x(),2)+pow(Normal.y()-positionCam.y(),2)+pow(Normal.z()-positionCam.z(),2));
    float aten;
    aten=1.0/pow(d_FacetoLight,1);

    // Ambiental
    cMR=(M_Obj.ModeloRGB_A.x());cMG=(M_Obj.ModeloRGB_A.y());cMB=(M_Obj.ModeloRGB_A.z());
    cMR=cMR*La;
    cMG=cMG*La;
    cMB=cMB*La;
    //cout<<"cMRA:"<<cMR<<"    cMRA:"<<cMG<<"   cMRA:"<<cMB<<endl;
    if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
    ColorA=qRgb(cMR,cMG,cMB);
    // Difusa
    N_Dot_L=QVector3D::dotProduct(Normal,Ocam);
    if(N_Dot_L>0)
    {
        cMR=(M_Obj.ModeloRGB_D.x());cMG=(M_Obj.ModeloRGB_D.y());cMB=(M_Obj.ModeloRGB_D.z());
        cMR=aten*cMR*cLR*N_Dot_L;
        cMG=aten*cMG*cLG*N_Dot_L;
        cMB=aten*cMB*cLB*N_Dot_L;
        if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(cMR,cMG,cMB);
    }else{
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(cMR,cMG,cMB);
        //ColorD=qRgb(0,0,0);
    }
    // Especular
    h=2*N_Dot_L*Normal-v_Luz.normalized();
    O_Dot_H=QVector3D::dotProduct(Ocam,h);
    cMR=(M_Obj.ModeloRGB_E.x());cMG=(M_Obj.ModeloRGB_E.y());cMB=(M_Obj.ModeloRGB_E.z());
    cMR=aten*cMR*cLR*pow(O_Dot_H,P);
    cMG=aten*cMG*cLG*pow(O_Dot_H,P);
    cMB=aten*cMB*cLB*pow(O_Dot_H,P);
    if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
    //cout<<"cMRE:"<<cMR<<"    cMRE:"<<cMG<<"   cMRE:"<<cMB<<endl;
    ColorE=qRgb(cMR,cMG,cMB);
    ColorF=ColorA+ColorE+ColorD;
    return ColorF;

/*
    N_Dot_L=QVector3D::dotProduct(v_Luz,Normal);
    if(N_Dot_L>0)
    {
        float d_FacetoLight;
        d_FacetoLight=sqrt(pow(Normal.x()-positionCam.x(),2)+pow(Normal.y()-positionCam.y(),2)+pow(Normal.z()-positionCam.z(),2));
        float aten;
        aten=1.0/pow(d_FacetoLight,2);

        cR=qRed(ColorLuz);cG=qGreen(ColorLuz);cB=qBlue(ColorLuz);
        cMR=qRed(ColorLuz);cMG=qGreen(ColorLuz);cMB=qBlue(ColorLuz);

        cR=Ia*La*cMR+cR*cMR*Ld*aten*N_Dot_L;
        cG=Ia*La*cMG+cG*cMG*Ld*aten*N_Dot_L;
        cB=Ia*La*cMB+cB*cMB*Ld*aten*N_Dot_L;
        if(cR>255){ cR = 255; }
        if(cG>255){ cG = 255; }
        if(cB>255){ cB = 255; }

        ColorA=qRgb(cR,cG,cB);
    }else
    {
        ColorA=qRgb(0,0,0);
        return ColorA;
    }

    float Ie;

    //h=2*QVector3D::dotProduct(v_Luz,Normal)*Normal-v_Luz;
    h=2*QVector3D::crossProduct(Normal,v_Luz)*Normal-v_Luz;
    Ie=Le*pow(QVector3D::dotProduct(-Ocam,h),P);
    ColorE=qRgb(h.x()*Ie,h.y()*Ie,h.z()*Ie);

    ColorF=ColorA+ColorE+ColorD;
    return ColorF;
*/

}
