#include "ilumination.h"

Ilumination::Ilumination(){}
Ilumination::Ilumination(QVector3D PosW, QRgb Color)
{
    v_Luz=PosW; ///Posicion de la luz
    v_Luz.normalize();
    ColorLuz=Color;/// Color de la luz
    La=65;
    Le=0.001;
    Ld=0.01;
    Ia=0.6;

    M=255;
    L=10;
    P=2;
}

QRgb Ilumination::LightTexel(QRgb argb, QVector3D Normal, QVector3D positionCam, QVector3D Ocam){

    float N_Dot_L;
    float O_Dot_H;

    QRgb ColorA;
    QRgb ColorD;
    QRgb ColorE;
    QRgb ColorF;
    QVector3D h;
    cLR=qRed(ColorLuz);cLG=qGreen(ColorLuz);cLB=qBlue(ColorLuz);


    // Ambiental
    //cMR=(M_Obj.ModeloRGB_A.x());cMG=(M_Obj.ModeloRGB_A.y());cMB=(M_Obj.ModeloRGB_A.z());
    cMR=qRed(argb)*0.001;cMG=qGreen(argb)*0.001;cMB=qBlue(argb)*0.001;
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
        //cMR=(M_Obj.ModeloRGB_D.x());cMG=(M_Obj.ModeloRGB_D.y());cMB=(M_Obj.ModeloRGB_D.z());
        cMR=qRed(argb)*0.001;cMG=qGreen(argb)*0.001;cMB=qBlue(argb)*0.001;
        cMR=cMR*cLR*N_Dot_L;
        cMG=cMG*cLG*N_Dot_L;
        cMB=cMB*cLB*N_Dot_L;
        if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(cMR,cMG,cMB);
    }else{
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(0,0,0);
    }
    // Especular
    h=2*N_Dot_L*Normal-v_Luz.normalized();
    O_Dot_H=QVector3D::dotProduct(Ocam,h);
    //cMR=(M_Obj.ModeloRGB_E.x());cMG=(M_Obj.ModeloRGB_E.y());cMB=(M_Obj.ModeloRGB_E.z());
    cMR=qRed(argb)*0.001;cMG=qGreen(argb)*0.001;cMB=qBlue(argb)*0.001;
    cMR=cMR*cLR*pow(O_Dot_H,P);
    cMG=cMG*cLG*pow(O_Dot_H,P);
    cMB=cMB*cLB*pow(O_Dot_H,P);
    if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
    //cout<<"cMRE:"<<cMR<<"    cMRE:"<<cMG<<"   cMRE:"<<cMB<<endl;
    ColorE=qRgb(cMR,cMG,cMB);
    ColorF=ColorA+ColorE+ColorD;
    return ColorF;
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
        cMR=cMR*cLR*N_Dot_L;
        cMG=cMG*cLG*N_Dot_L;
        cMB=cMB*cLB*N_Dot_L;
        if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(cMR,cMG,cMB);
    }else{
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(0,0,0);
    }
    // Especular
    h=2*N_Dot_L*Normal-v_Luz.normalized();
    O_Dot_H=QVector3D::dotProduct(Ocam,h);
    cMR=(M_Obj.ModeloRGB_E.x());cMG=(M_Obj.ModeloRGB_E.y());cMB=(M_Obj.ModeloRGB_E.z());
    cMR=cMR*cLR*pow(O_Dot_H,P);
    cMG=cMG*cLG*pow(O_Dot_H,P);
    cMB=cMB*cLB*pow(O_Dot_H,P);
    if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
    //cout<<"cMRE:"<<cMR<<"    cMRE:"<<cMG<<"   cMRE:"<<cMB<<endl;
    ColorE=qRgb(cMR,cMG,cMB);
    ColorF=(ColorA+ColorE+ColorD);
    return ColorF;

}

QRgb Ilumination::LightP(ModeloOBJ &M_Obj, QVector3D Normal, QVector3D positionCam, QVector3D Ocam){

    float N_Dot_L;
    float O_Dot_H;

    QRgb ColorA;
    QRgb ColorD;
    QRgb ColorE;
    QRgb ColorF;
    QVector3D h;
    cLR=qRed(ColorLuz);cLG=qGreen(ColorLuz);cLB=qBlue(ColorLuz);

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
        cMR=cMR*cLR*N_Dot_L;
        cMG=cMG*cLG*N_Dot_L;
        cMB=cMB*cLB*N_Dot_L;
        if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
        if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(cMR,cMG,cMB);
    }else{
        //cout<<"cMRD:"<<cMR<<"    cMRD:"<<cMG<<"   cMRD:"<<cMB<<endl;
        ColorD=qRgb(0,0,0);
    }
    // Especular
    h=2*N_Dot_L*Normal-v_Luz.normalized();
    O_Dot_H=QVector3D::dotProduct(Ocam,h);
    cMR=(M_Obj.ModeloRGB_E.x());cMG=(M_Obj.ModeloRGB_E.y());cMB=(M_Obj.ModeloRGB_E.z());
    cMR=cMR*cLR*pow(O_Dot_H,P);
    cMG=cMG*cLG*pow(O_Dot_H,P);
    cMB=cMB*cLB*pow(O_Dot_H,P);
    if(cMR>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMG>255){ cMR = 255;cMG = 255;cMB = 255; }
    if(cMB>255){ cMR = 255;cMG = 255;cMB = 255; }
    //cout<<"cMRE:"<<cMR<<"    cMRE:"<<cMG<<"   cMRE:"<<cMB<<endl;
    ColorE=qRgb(cMR,cMG,cMB);
    ColorF=(ColorA+ColorE+ColorD)*2;
    return ColorF;


}
