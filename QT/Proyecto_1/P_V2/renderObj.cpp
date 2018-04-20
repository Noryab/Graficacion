#include "renderObj.h"
#include "ilumination.h"
#include "camara.h"

#include <QStandardItemModel>

camara cam;
QRgb LuzColorR(qRgb(40,40,215));
QVector3D PosLuzR(1,1,0);
QRgb LuzColorW(qRgb(200,200,200));
QVector3D PosLuzW(-1,-1,0);

Ilumination LightW(PosLuzW,LuzColorW);
Ilumination LightR(PosLuzR,LuzColorR);

renderObj::renderObj()
{
    camOrig.setX(0);
    camOrig.setY(0);
    camOrig.setZ(-1);

    qPoint.setX(0);
    qPoint.setY(0);
    qPoint.setZ(1);    
    theta=0.5;

}

void renderObj::RenderObj(ModeloOBJ &M_Obj,QPainter &painter)
{    
    float z[3];
    float zt;
    if(RecalMatri==true)
    {
        Reproject(M_Obj);
        RecalMatri=false;
    }
    /// TEXTURA
    if(Textura==true)
    {
        float z[3];

        QVector2D P[3];
        QVector2D Q[3];

        QPen colorPoint;
        QRgb EdgeColor;
        EdgeColor=qRgb(0,0,0);
        Line Line_(painter,1,EdgeColor);

        int xdmaxb=1000;
        int ydmaxb=1000;
        int maxDepthb=-80.0;

        for(int i=0; i<xdmaxb; i++)
        {
            for(int j=0; j<ydmaxb; j++)
            {
                Line_.Buffer_Z(i,j) = maxDepthb;
            }
        }

        QVector3D V_Cam;
        V_Cam.setX(cam.V_g.x());
        V_Cam.setY(cam.V_g.y());
        V_Cam.setZ(cam.V_g.z());
        LightW.v_Luz=V_Cam.normalized();
        LightR.v_Luz=V_Cam.normalized();

        for(int f=0;f<M_Obj.getNC();f++)
        {
            QVector3D V_NormalC;
            V_NormalC.setX(M_Obj.modeloNormalesFC[f].x());
            V_NormalC.setY(M_Obj.modeloNormalesFC[f].y());
            V_NormalC.setZ(M_Obj.modeloNormalesFC[f].z());

            Count=0;
                P[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                P[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                Q[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                Q[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Line_.pV[0]=P[0];
                Line_.pV[1]=Q[0];

                P[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                P[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Q[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                Q[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Line_.pV[2]=Q[1];

                P[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                P[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Q[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                Q[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());

                z[0]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].x()-1].z();
                z[1]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].y()-1].z();
                z[2]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].z()-1].z();

                ScanConversion(P[0],Q[0],z[0],z[1]);
                ScanConversion(P[1],Q[1],z[1],z[2]);
                ScanConversion(P[2],Q[2],z[2],z[0]);

                int j=0;
                for(int i=0;i<Count;i++)
                {
                    P[0]=QVector2D(CoordX[i],CoordY[i]);
                    Q[0]=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    Line_.DrawLineTextura(painter,P[0],Q[0],zLine[i],zLine[Count-j],LightW,LightR,V_NormalC);
                    j++;
                }
        }
    }
    /// RELLENO DE SUPERFICIE Poligonos
    if(Poligon==true)
    {
        Poligonal(M_Obj,painter);
    }    
    /// NUBE DE PUNTOS
    if(Punt==true)
    {
        QPen colorPoint;
        QRgb argb;
        argb=qRgb(0,225,45);
        colorPoint.setWidth(1);
        colorPoint.setBrush((QBrush)argb);
        painter.setPen(colorPoint);

        for(int i=0;i<M_Obj.getNV();i++)
        {
            int dx= M_Obj.modeloVerticesP[i].x();
            int dy= M_Obj.modeloVerticesP[i].y();
            painter.drawPoint(dx,dy);
        }
    }
    /// MALLA
    if(Mall==true)
    {
        QVector2D P_1;
        QVector2D P_2;
        QVector2D P_3;

        QRgb EdgeColor;
        EdgeColor=qRgb(0,0,225);
        Line Line_(painter,1,EdgeColor);

        for(int i=0;i<M_Obj.getNC();i++)
        {
            P_1.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[i].x()-1].x());
            P_1.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[i].x()-1].y());
            P_2.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[i].y()-1].x());
            P_2.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[i].y()-1].y());
            P_3.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[i].z()-1].x());
            P_3.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[i].z()-1].y());

            Line_.DrawLine(painter,P_1,P_2,zt,zt);
            Line_.DrawLine(painter,P_2,P_3,zt,zt);
            Line_.DrawLine(painter,P_3,P_1,zt,zt);
        }
    }
    /// RELLENO DE SUPERFICIE Gouraud
    if(Gouraud==true)
    {

        QVector2D P[3];
        QVector2D Q[3];

        QRgb EdgeColor;
        EdgeColor=qRgb(0,0,0);
        Line Line_(painter,1,EdgeColor);

        int xdmaxb=1000;
        int ydmaxb=1000;
        int maxDepthb=-80.0;

        for(int i=0; i<xdmaxb; i++)
        {
            for(int j=0; j<ydmaxb; j++)
            {
                Line_.Buffer_Z(i,j) = maxDepthb;
            }
        }

        QVector3D V_Cam;
        V_Cam.setX(cam.V_g.x());
        V_Cam.setY(cam.V_g.y());
        V_Cam.setZ(cam.V_g.z());
        LightW.v_Luz=V_Cam.normalized();
        LightR.v_Luz=V_Cam.normalized();

        for(int f=0;f<M_Obj.getNC();f++)
        //for(int f=0;f<4000;f++)
        {
                Count=0;
                P[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                P[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                Q[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                Q[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Line_.pV[0]=P[0];
                Line_.pV[1]=Q[0];

                P[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                P[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Q[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                Q[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Line_.pV[2]=Q[1];

//                cout<<"pVx[0]:"<<Line_.pV[0].x()<<"    pVx[1]:"<<Line_.pV[1].x()<<"   pVx[2]:"<<Line_.pV[2].x()<<endl;
//                cout<<"pVy[0]:"<<Line_.pV[0].y()<<"    pVy[1]:"<<Line_.pV[1].y()<<"   pVy[2]:"<<Line_.pV[2].y()<<endl<<endl;

                P[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                P[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Q[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                Q[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());

                z[0]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].x()-1].z();
                z[1]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].y()-1].z();
                z[2]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].z()-1].z();

                ScanConversion(P[0],Q[0],z[0],z[1]);
                ScanConversion(P[1],Q[1],z[1],z[2]);
                ScanConversion(P[2],Q[2],z[2],z[0]);

                    QVector3D V_NormalC[3];
                    //V1
                    V_NormalC[0].setX(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1].x());
                    V_NormalC[0].setY(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1].y());
                    V_NormalC[0].setZ(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1].z());
                    //V2
                    V_NormalC[1].setX(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1].x());
                    V_NormalC[1].setY(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1].y());
                    V_NormalC[1].setZ(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1].z());
                    //V3
                    V_NormalC[2].setX(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1].x());
                    V_NormalC[2].setY(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1].y());
                    V_NormalC[2].setZ(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1].z());
                    QRgb argb;
                    QVector3D camPosition;
                    QVector3D Ocam;

                    for(int in=0;in<3;in++){
                        Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
                        camPosition.setX(-0.1); camPosition.setY(-0.1); camPosition.setZ(-0.1);
                        argb=LightW.Light(M_Obj,V_NormalC[in],camPosition,Ocam.normalized());

                        Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)+1);Ocam.setZ(cam.V_q(2));
                        camPosition.setX(0.2); camPosition.setY(-0.1); camPosition.setZ(0.1);
                        argb=argb+LightR.Light(M_Obj,V_NormalC[in],camPosition,Ocam.normalized());

                        ///Actualización del color de cada vertice para parametro de interpolación
                        Line_.cV[in]=argb;
                    }

                int j=0;
                for(int i=0;i<Count;i++)
                {
                    P[0]=QVector2D(CoordX[i],CoordY[i]);
                    Q[0]=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    Line_.DrawLineGouraud(painter,P[0],Q[0],zLine[i],zLine[Count-j]);
                    j++;
                }
        }

    }

    /// RELLENO DE SUPERFICIE Phong
    if(Phong==true)
    {
        QVector2D P[3];
        QVector2D Q[3];

        QRgb EdgeColor;
        EdgeColor=qRgb(0,0,0);
        Line Line_(painter,1,EdgeColor);

        int xdmaxb=1000;
        int ydmaxb=1000;
        int maxDepthb=-80.0;

        for(int i=0; i<xdmaxb; i++)
        {
            for(int j=0; j<ydmaxb; j++)
            {
                Line_.Buffer_Z(i,j) = maxDepthb;
            }
        }

        QVector3D V_Cam;
        V_Cam.setX(cam.V_g.x());
        V_Cam.setY(cam.V_g.y());
        V_Cam.setZ(cam.V_g.z());
        LightW.v_Luz=V_Cam.normalized();
        LightR.v_Luz=V_Cam.normalized();

        for(int f=0;f<M_Obj.getNC();f++)        
        {
                Count=0;
                P[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                P[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                Q[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                Q[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Line_.pV[0]=P[0];
                Line_.pV[1]=Q[0];

                P[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                P[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Q[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                Q[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Line_.pV[2]=Q[1];

                P[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                P[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Q[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                Q[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());

                z[0]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].x()-1].z();
                z[1]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].y()-1].z();
                z[2]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].z()-1].z();

                ScanConversion(P[0],Q[0],z[0],z[1]);
                ScanConversion(P[1],Q[1],z[1],z[2]);
                ScanConversion(P[2],Q[2],z[2],z[0]);

                    QVector3D V_NormalC[3];
                    //V1
                    V_NormalC[0].setX(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1].x());
                    V_NormalC[0].setY(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1].y());
                    V_NormalC[0].setZ(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1].z());
                    //V2
                    V_NormalC[1].setX(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1].x());
                    V_NormalC[1].setY(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1].y());
                    V_NormalC[1].setZ(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1].z());
                    //V3
                    V_NormalC[2].setX(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1].x());
                    V_NormalC[2].setY(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1].y());
                    V_NormalC[2].setZ(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1].z());
                    QRgb argb;
                    QVector3D camPosition;
                    QVector3D Ocam;

                        Line_.nV[0]=V_NormalC[0];
                        Line_.nV[1]=V_NormalC[1];
                        Line_.nV[2]=V_NormalC[2];

                int j=0;
                for(int i=0;i<Count;i++)
                {
                    P[0]=QVector2D(CoordX[i],CoordY[i]);
                    Q[0]=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    Line_.DrawLinePhong(M_Obj,painter,P[0],Q[0],zLine[i],zLine[Count-j]);
                    j++;
                }
        }

    }
    fprintf(stderr, "-------- Renderizado -------\n");
}


bool Line::verifBuffer(int i,int j,float z){
    if(i<0 || j<0 || i>=1000 || j>=1000)
        return false;

    if(z>=Buffer_Z(i,j))
    {     
            Buffer_Z(i,j)=z;
        return true;
    }
    return false;
}

void renderObj::Reproject(ModeloOBJ &M_Obj)
{
        Vector4d LightT=VectorXd::Ones(4,1);
        Vector4d LightC=VectorXd::Ones(4,1);

        LightT<<LightW.v_Luz.x(),
                LightW.v_Luz.y(),
                LightW.v_Luz.z(),
                1;
        LightC=cam.M_TC*LightT;
        LightW.v_Luz.setX(LightC(0));
        LightW.v_Luz.setY(LightC(1));
        LightW.v_Luz.setZ(LightC(2));
        LightT<<LightR.v_Luz.x(),
                LightR.v_Luz.y(),
                LightR.v_Luz.z(),
                1;
        LightC=cam.M_TC*LightT;
        LightR.v_Luz.setX(LightC(0));
        LightR.v_Luz.setY(LightC(1));
        LightR.v_Luz.setZ(LightC(2));

        Vector4d VerticesW=VectorXd::Ones(4,1);
        Vector4d VerticesC=VectorXd::Ones(4,1);
        Vector3d VerticesWT=VectorXd::Ones(3,1);

        Vector4d NormalesW=VectorXd::Ones(4,1);
        Vector4d NormalesC=VectorXd::Ones(4,1);
        Vector3d NormalesWT=VectorXd::Ones(3,1);

        Vector4d VerticesP=VectorXd::Ones(4,1);

        M_Obj.modeloVerticesC.clear();
        M_Obj.modeloVerticesP.clear();
        M_Obj.modeloNormalesVC.clear();

        /// APLICAR TRANSFORMACIONES A VERTICES Y NORMALES
        for(int i=0;i<M_Obj.getNV();i++)
        {
            /// COORDENADAS DEL MUNDO A CAMARA
            VerticesWT<<    M_Obj.modeloVerticesW[i].x(),
                            M_Obj.modeloVerticesW[i].y(),
                            M_Obj.modeloVerticesW[i].z();
            VerticesW<<VerticesWT,1;
            VerticesC=cam.M_TC*VerticesW;            
            M_Obj.modeloVerticesC.push_back(QVector3D(VerticesC(0),
                                                VerticesC(1),
                                                VerticesC(2)));
            /// PROYECCIÓN A 2D VERTICES
            VerticesP=cam.M_ProyOrt*VerticesC;            
            /// PROYECCION AL DISPOSITIVO VERTICES
            int dx= (int)(cam.sx*VerticesP(0)+cam.cx-0.5);
            int dy= (int)(cam.sy*VerticesP(1)+cam.cy-0.5);
            M_Obj.modeloVerticesP.push_back(QVector2D(dx,dy));

            /// TRANSFORMACIÓN DE NORMALES DE CADA VERTICE EN EL MUNDO A COORDENADAS DE LA CAMARA
            NormalesWT<<    M_Obj.modeloNormalesVW[i].x(),
                            M_Obj.modeloNormalesVW[i].y(),
                            M_Obj.modeloNormalesVW[i].z();

            VerticesWT<<    M_Obj.modeloVerticesW[i].x(),
                            M_Obj.modeloVerticesW[i].y(),
                            M_Obj.modeloVerticesW[i].z();

            NormalesWT=NormalesWT+VerticesWT;
            NormalesW<<NormalesWT,1;

            VerticesWT<<    M_Obj.modeloVerticesC[i].x(),
                            M_Obj.modeloVerticesC[i].y(),
                            M_Obj.modeloVerticesC[i].z();
            VerticesW<<VerticesWT,1;

            NormalesC=(cam.M_TC*NormalesW)-VerticesW;            
            M_Obj.modeloNormalesVC.push_back(QVector3D( NormalesC(0),
                                                        NormalesC(1),
                                                        NormalesC(2)).normalized());
        }
    M_Obj.NormalesCarasC();
}

void renderObj::ScanConversion( QVector2D P, QVector2D Q, float z1, float z2){

    float Delta_x=(float)Q.x()-(float)P.x();
    float Delta_y=(float)Q.y()-(float)P.y();
    float Delta_z=z2-z1;
    float Long;
    if(abs(Delta_x)>abs(Delta_y)){
       Long=abs(Delta_x);
    }else{
       Long=abs(Delta_y);
    }

    float increment_x=Delta_x/Long;
    float increment_y=Delta_y/Long;
    float increment_z=Delta_z/Long;

    float x=P.x();
    float y=P.y();
    float z=z1;

    CoordX[Count]=(int)x;
    CoordY[Count]=(int)y;
    zLine[Count]=z;

        for(int i=1;i<=Long;i++)
        {
           Count++;
           x=x+increment_x;
           y=y+increment_y;
           z=z+increment_z;

            CoordX[Count]=(int)x;
            CoordY[Count]=(int)y;
            zLine[Count]=z;
        }
}

Line::Line(QPainter &painter, int Widht, QRgb Edge)
{
    Widht=Widht;
    colorPoint.setWidth(Widht);
    colorPoint.setBrush((QBrush)Edge);
    painter.setPen(colorPoint);
    Buffer_Z=MatrixXd::Zero(1000,1000);

    textura.load("/home/noryab/QT/Proyecto_1/P_V2/texturaConejo.jpg");
    //textura.load("/home/noryab/QT/Proyecto_1/P_V2/textura2.jpg");
    texW = textura.width();
    texH = textura.height();

}
///GOURAUD
QRgb Line::interpolacionColor(QVector2D P,QVector2D Q,float x_,float y_)
{
    QRgb fP;
    float Ax, Bx, Cx, Px;
    float Ay, By, Cy, Py;
    float barA, barB , barC;
    Ax=pV[0].x();
    Ay=pV[0].y();
    Bx=pV[1].x();
    By=pV[1].y();
    Cx=pV[2].x();
    Cy=pV[2].y();

    Px=(float)x_;
    Py=(float)y_;
    barA=(((By-Cy)*(Px-Cx)+(Cx-Bx)*(Py-Cy))/((By-Cy)*(Ax-Cx)+(Cx-Bx)*(Ay-Cy)+0.001));
    barB=(((Cy-Ay)*(Px-Cx)+(Ax-Cx)*(Py-Cy))/((By-Cy)*(Ax-Cx)+(Cx-Bx)*(Ay-Cy)+0.001));
    barC=1-barA-barB;
    int cLR1,cLG1,cLB1;int cLR2,cLG2,cLB2; int cLR3,cLG3,cLB3;
    float cLRF,cLGF,cLBF;
    cLR1=qRed(cV[0]);cLG1=qGreen(cV[0]);cLB1=qBlue(cV[0]);
    cLR2=qRed(cV[1]);cLG2=qGreen(cV[1]);cLB2=qBlue(cV[1]);
    cLR3=qRed(cV[2]);cLG3=qGreen(cV[2]);cLB3=qBlue(cV[2]);

    cLRF=barA*cLR1+barB*cLR2+barC*cLR3;
    cLGF=barA*cLG1+barB*cLG2+barC*cLG3;
    cLBF=barA*cLB1+barB*cLB2+barC*cLB3;

    fP=qRgb(cLRF,cLGF,cLBF);

    return fP;
}

void Line::DrawLinePhong(ModeloOBJ &M_Obj, QPainter &painter, QVector2D P_1, QVector2D P_2, float z1, float z2)
{
    QPen colorPoint;
    QVector3D NormalInter;

    Delta_x=(float)P_2.x()-(float)P_1.x();
    Delta_y=(float)P_2.y()-(float)P_1.y();
    float Delta_z=z2-z1;

    if(abs(Delta_x)>abs(Delta_y))
    {
       Long=abs(Delta_x);
    }else
    {
       Long=abs(Delta_y);
    }

    increment_x=(float)Delta_x/Long;
    increment_y=(float)Delta_y/Long;
    float increment_z=(float)Delta_z/Long;

    QRgb argb,argbT;
    QVector3D camPosition;
    QVector3D Ocam;
    Ocam.setX(cam.V_g(0));Ocam.setY(cam.V_g(1));Ocam.setZ(cam.V_g(2));
    Ocam.normalize();


    x=P_1.x();
    y=P_1.y();
    float z=z1;

    if(verifBuffer((int)x,(int)y,z)){
            NormalInter=interpolacionNormal(P_1,P_2,x,y);

            Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
            camPosition.setX(-0.1); camPosition.setY(-0.1); camPosition.setZ(-0.1);
            argb=LightW.LightP(M_Obj,NormalInter,camPosition,Ocam.normalized());

            Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)+1);Ocam.setZ(cam.V_q(2));
            camPosition.setX(0.2); camPosition.setY(-0.1); camPosition.setZ(0.1);
            argb=argb+LightR.LightP(M_Obj,NormalInter,camPosition,Ocam.normalized());



        colorPoint.setBrush((QBrush)argb);
        painter.setPen(colorPoint);
        painter.drawPoint(x,y);
    }

    for(int i=1;i<=Long;i++)
    {
           x=x+increment_x;
           y=y+increment_y;
           z=z+increment_z;
           if(verifBuffer((int)x,(int)y,z)){
               NormalInter=interpolacionNormal(P_2,P_1,x,y);

               Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
               camPosition.setX(-0.1); camPosition.setY(-0.1); camPosition.setZ(-0.1);
               argb=LightW.LightP(M_Obj,NormalInter,camPosition,Ocam.normalized());

               Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)+1);Ocam.setZ(cam.V_q(2));
               camPosition.setX(0.2); camPosition.setY(-0.1); camPosition.setZ(0.1);
               argb=argb+LightR.LightP(M_Obj,NormalInter,camPosition,Ocam.normalized());

               colorPoint.setBrush((QBrush)argb);
               painter.setPen(colorPoint);
               painter.drawPoint(x,y);
           }
    }
}



QVector3D Line::interpolacionNormal(QVector3D N1, QVector3D N2, float x_, float y_)
{

    QVector3D N;

    float Ax, Bx, Cx, Px;
    float Ay, By, Cy, Py;
    float barA, barB , barC;
    Ax=pV[0].x();
    Ay=pV[0].y();
    Bx=pV[1].x();
    By=pV[1].y();
    Cx=pV[2].x();
    Cy=pV[2].y();

    Px=(float)x_;
    Py=(float)y_;
    barA=(((By-Cy)*(Px-Cx)+(Cx-Bx)*(Py-Cy))/((By-Cy)*(Ax-Cx)+(Cx-Bx)*(Ay-Cy)+0.001));
    barB=(((Cy-Ay)*(Px-Cx)+(Ax-Cx)*(Py-Cy))/((By-Cy)*(Ax-Cx)+(Cx-Bx)*(Ay-Cy)+0.001));
    barC=1-barA-barB;

    float nX1,nY1,nZ1;float nX2,nY2,nZ2;float nX3,nY3,nZ3;
    float nXF,nYF,nZF;

    nX1=nV[0].x();nY1=nV[0].y();nZ1=nV[0].z();
    nX2=nV[1].x();nY2=nV[1].y();nZ2=nV[1].z();
    nX3=nV[2].x();nY3=nV[2].y();nZ3=nV[2].z();

    nXF=barA*nX1+barB*nX2+barC*nX3;
    nYF=barA*nY1+barB*nY2+barC*nY3;
    nZF=barA*nZ1+barB*nZ2+barC*nZ3;

    N=QVector3D(nXF,nYF,nZF);
    N.normalize();

    return N;
}



int Line::randNumber(int low, int high)
{
     return qrand() % ((high + 1) - low) + low;
}

void Line::DrawLineTextura(QPainter &painter, QVector2D P_1, QVector2D P_2, float z1, float z2, Ilumination LightW, Ilumination LightR, QVector3D V_NormalC)
{
    QPen colorPoint;

    Delta_x=(float)P_2.x()-(float)P_1.x();
    Delta_y=(float)P_2.y()-(float)P_1.y();
    float Delta_z=z2-z1;

    if(abs(Delta_x)>abs(Delta_y))
    {
       Long=abs(Delta_x);
    }else
    {
       Long=abs(Delta_y);
    }

    increment_x=(float)Delta_x/Long;
    increment_y=(float)Delta_y/Long;
    float increment_z=(float)Delta_z/Long;

    QRgb argb,argbT;
    QVector3D camPosition;
    QVector3D Ocam;
    Ocam.setX(cam.V_g(0));Ocam.setY(cam.V_g(1));Ocam.setZ(cam.V_g(2));
    Ocam.normalize();

    x=P_1.x();
    y=P_1.y();
    float z=z1;
    if(verifBuffer((int)x,(int)y,z)){
        QVector2D cUV =MapTextura(x,y);
        argb=QRgb(textura.pixel(cUV.x(),cUV.y()));
        Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
        camPosition.setX(-0.1); camPosition.setY(-0.1); camPosition.setZ(-0.1);
        argbT=LightW.LightTexel(argb,V_NormalC,camPosition,Ocam);

        Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)+1);Ocam.setZ(cam.V_q(2));
        camPosition.setX(0.2); camPosition.setY(-0.1); camPosition.setZ(0.1);
        argb=argbT+LightR.LightTexel(argbT,V_NormalC,camPosition,Ocam);

        colorPoint.setBrush((QBrush)argbT);
        painter.setPen(colorPoint);
        painter.drawPoint(x,y);
    }

    for(int i=1;i<=Long;i++)
    {
           x=x+increment_x;
           y=y+increment_y;
           z=z+increment_z;
           if(verifBuffer((int)x,(int)y,z)){
               QVector2D cUV =MapTextura(x,y);
                argb=QRgb(textura.pixel(cUV.x(),cUV.y()));
                Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
                camPosition.setX(-0.1); camPosition.setY(-0.1); camPosition.setZ(-0.1);
                argbT=LightW.LightTexel(argb,V_NormalC,camPosition,Ocam);

                Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)+1);Ocam.setZ(cam.V_q(2));
                camPosition.setX(0.2); camPosition.setY(-0.1); camPosition.setZ(0.1);
                argb=argbT+LightR.LightTexel(argbT,V_NormalC,camPosition,Ocam);

                colorPoint.setBrush((QBrush)argbT);
                painter.setPen(colorPoint);
                painter.drawPoint(x,y);
           }
    }
}


QVector2D Line::MapTextura(double x_, double y_){

    QVector2D coord_UV;
    int Ax, Bx, Cx, Px;
    int Ay, By, Cy, Py;
    int barA, barB , barC;
    int c_U,c_V;
    int U1, U2, U3;
    int V1, V2, V3;

    U1=x_-130;
    V1=y_-90;
    U2=x_-160;
    V2=y_-240;
    U3=x_-140;
    V3=y_-210;

    Ax=pV[0].x();
    Ay=pV[0].y();
    Bx=pV[1].x();
    By=pV[1].y();
    Cx=pV[2].x();
    Cy=pV[2].y();

    Px=(int)x_;
    Py=(int)y_;

    barA=(int)(((By-Cy)*(Px-Cx)+(Cx-Bx)*(Py-Cy))/((By-Cy)*(Ax-Cx)+(Cx-Bx)*(Ay-Cy)+0.01));
    barB=(int)(((Cy-Ay)*(Px-Cx)+(Ax-Cx)*(Py-Cy))/((By-Cy)*(Ax-Cx)+(Cx-Bx)*(Ay-Cy)+0.01));
    barC=1-barA-barB;
    c_U=barA*U1+barB*U2+barC*U3;
    c_V=barA*V1+barB*V2+barC*V3;
    if(c_U<0)
    {
        c_U=0;
    }
    if(c_U>texW)
    {
        c_U=texW-1;
    }
    if(c_V<0)
    {
        c_V=0;
    }
    if(c_V>texH)
    {
        c_V=texH-1;
    }
    coord_UV.setX(c_U);
    coord_UV.setY(c_V);

    return coord_UV;
}

void Line::DrawLineGouraud(QPainter &painter, QVector2D P_1, QVector2D P_2, float z1,float z2)
{
    QRgb argb;
    QPen colorPoint;

    Delta_x=(float)P_2.x()-(float)P_1.x();
    Delta_y=(float)P_2.y()-(float)P_1.y();
    float Delta_z=z2-z1;

    if(abs(Delta_x)>abs(Delta_y))
    {
       Long=abs(Delta_x);
    }else
    {
       Long=abs(Delta_y);
    }

    increment_x=(float)Delta_x/Long;
    increment_y=(float)Delta_y/Long;
    float increment_z=(float)Delta_z/Long;

    x=P_1.x();
    y=P_1.y();
    float z=z1;

    if(verifBuffer((int)x,(int)y,z)){
        argb=interpolacionColor(P_1,P_2,x,y);
        colorPoint.setBrush((QBrush)argb);
        painter.setPen(colorPoint);
        painter.drawPoint(x,y);
    }

    for(int i=1;i<=Long;i++)
    {
           x=x+increment_x;
           y=y+increment_y;
           z=z+increment_z;
           if(verifBuffer((int)x,(int)y,z)){
               argb=interpolacionColor(P_2,P_1,x,y);
               colorPoint.setBrush((QBrush)argb);
               painter.setPen(colorPoint);
               painter.drawPoint(x,y);
           }
    }
}


void Line::DrawLine(QPainter &painter, QVector2D P_1, QVector2D P_2, float z1,float z2)
{
        Delta_x=(float)P_2.x()-(float)P_1.x();
        Delta_y=(float)P_2.y()-(float)P_1.y();
        float Delta_z=z2-z1;

        if(abs(Delta_x)>abs(Delta_y))
        {
           Long=abs(Delta_x);
        }else
        {
           Long=abs(Delta_y);
        }

        increment_x=(float)Delta_x/Long;
        increment_y=(float)Delta_y/Long;
        float increment_z=(float)Delta_z/Long;

        x=P_1.x();
        y=P_1.y();
        float z=z1;
        if(verifBuffer((int)x,(int)y,z))
            painter.drawPoint(x,y);

        for(int i=1;i<=Long;i++)
        {
               x=x+increment_x;
               y=y+increment_y;
               z=z+increment_z;
               if(verifBuffer((int)x,(int)y,z))
                   painter.drawPoint(x,y);
        }
}

void renderObj::Poligonal(ModeloOBJ &M_Obj, QPainter &painter)
{
    float z[3];

    QVector2D P[3];
    QVector2D Q[3];

    QPen colorPoint;
    QRgb EdgeColor;
    EdgeColor=qRgb(0,0,0);
    Line Line_(painter,1,EdgeColor);

    int xdmaxb=1000;
    int ydmaxb=1000;
    int maxDepthb=-80.0;

    for(int i=0; i<xdmaxb; i++)
    {
        for(int j=0; j<ydmaxb; j++)
        {
            Line_.Buffer_Z(i,j) = maxDepthb;
        }
    }

    QVector3D V_Cam;
    V_Cam.setX(cam.V_g.x());
    V_Cam.setY(cam.V_g.y());
    V_Cam.setZ(cam.V_g.z());
    LightW.v_Luz=V_Cam.normalized();
    LightR.v_Luz=V_Cam.normalized();


    for(int f=0;f<M_Obj.getNC();f++)
    {
        QVector3D V_NormalC;
        V_NormalC.setX(M_Obj.modeloNormalesFC[f].x());
        V_NormalC.setY(M_Obj.modeloNormalesFC[f].y());
        V_NormalC.setZ(M_Obj.modeloNormalesFC[f].z());

            Count=0;
                P[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                P[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                Q[0].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                Q[0].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());

                P[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                P[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                Q[1].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                Q[1].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());

                P[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                P[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                Q[2].setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                Q[2].setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());

                z[0]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].x()-1].z();
                z[1]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].y()-1].z();
                z[2]=M_Obj.modeloVerticesC[M_Obj.modeloCaras[f].z()-1].z();

                ScanConversion(P[0],Q[0],z[0],z[1]);
                ScanConversion(P[1],Q[1],z[1],z[2]);
                ScanConversion(P[2],Q[2],z[2],z[0]);

                QRgb argb;
                QVector3D camPosition;
                QVector3D Ocam;
                Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
                camPosition.setX(-0.1); camPosition.setY(-0.1); camPosition.setZ(-0.1);
                argb=LightW.Light(M_Obj,V_NormalC,camPosition,Ocam.normalized());

                Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1)-1);Ocam.setZ(cam.V_q(2));
                camPosition.setX(0.2); camPosition.setY(-0.1); camPosition.setZ(0.1);
                argb=argb+LightR.Light(M_Obj,V_NormalC,camPosition,Ocam.normalized());


                    colorPoint.setBrush((QBrush)argb);
                    painter.setPen(colorPoint);
                int j=0;
                for(int i=0;i<Count;i++)
                {
                    P[0]=QVector2D(CoordX[i],CoordY[i]);
                    Q[0]=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    Line_.DrawLine(painter,P[0],Q[0],zLine[i],zLine[Count-j]);
                    j++;
                }
    }
}
