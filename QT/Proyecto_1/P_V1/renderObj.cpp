#include "renderObj.h"
#include "ilumination.h"
#include "camara.h"

#include <QStandardItemModel>

camara cam;
QRgb LuzColorR(qRgb(5,250,5));
QVector3D PosLuzR(1,1,0);
QRgb LuzColorW(qRgb(255,255,255));
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

    Buffer_Z=MatrixXd::Zero(1000,1000);

    theta=0.5;
    //camara cam2(camOrig,qPoint,theta);
    //cam=cam2;
    //cout<<"Puntero camara="<<camaraObj->M_TC<<endl;
}


void renderObj::RenderObj(ModeloOBJ &M_Obj,QPainter &painter)
{    

    if(RecalMatri==true)
    {
        Reproject(M_Obj);
        RecalMatri=false;
    }
    /// RELLENO DE SUPERFICIE Poligonos
    if(Poligon==true)
    {
        //Parámetros que definen el tamaño de la ventana (definirán igual el tamaño del Z-Buffer)
        int xdmaxb=1000;
        int ydmaxb=1000;
        int maxDepthb=-80.0;
        //Inicialización del Z-Buffer
        for(int i=0; i<xdmaxb; i++)
        {
            for(int j=0; j<ydmaxb; j++)
            {
                Buffer_Z(i,j) = maxDepthb;
            }
        }
        QVector2D P;
        QVector2D Q;

        QPen colorPoint;
        QRgb EdgeColor;        
        EdgeColor=qRgb(0,0,0);
        Line Line_(painter,1,EdgeColor);

        QVector3D V_Cam;
        V_Cam.setX(cam.V_q.x());
        V_Cam.setY(cam.V_q.y());
        V_Cam.setZ(cam.V_q.z());
        LightW.v_Luz=V_Cam.normalized();
        LightR.v_Luz=V_Cam.normalized();

        for(int f=0;f<M_Obj.getNC();f++)
        {
            QVector3D V_NormalC;
            V_NormalC.setX(M_Obj.modeloNormalesFC[f].x());
            V_NormalC.setY(M_Obj.modeloNormalesFC[f].y());
            V_NormalC.setZ(M_Obj.modeloNormalesFC[f].z());

            Dot=QVector3D::dotProduct(V_NormalC,V_Cam);
            //if(Dot>=0){
                Count=0;                                
                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                    ScanConversion(painter,P,Q);

                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                    ScanConversion(painter,P,Q);

                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                    ScanConversion(painter,P,Q);

                QRgb argb;
                QVector3D camPosition;
                QVector3D Ocam;
                    Ocam.setX(cam.V_q(0));Ocam.setY(1+cam.V_q(1));Ocam.setZ(cam.V_q(2));
                    camPosition.setX(-0.2); camPosition.setY(-0.2); camPosition.setZ(-0.2);
                    argb=LightW.Light(M_Obj,V_NormalC,camPosition,Ocam.normalized());

                    Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1));Ocam.setZ(cam.V_q(2));
                    camPosition.setX(0.5); camPosition.setY(0.5); camPosition.setZ(0);
                    argb=argb+LightR.Light(M_Obj,V_NormalC,camPosition,Ocam.normalized());

                    colorPoint.setBrush((QBrush)argb);
                    painter.setPen(colorPoint);
                int j=0;
                for(int i=0;i<Count;i++)
                {                   
                    P=QVector2D(CoordX[i],CoordY[i]);
                    Q=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    if(verifBuffer(M_Obj,f,P.x(),P.y())){
                     //if(verifBuffer(M_Obj,f,Q.x(),Q.y())){
                        //if(Dot>0)
                            Line_.DrawLine(painter,P,Q);
                      //}
                    }
                    j++;
                }
            //}
        }
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

            Line_.DrawLine(painter,P_1,P_2);
            Line_.DrawLine(painter,P_2,P_3);
            Line_.DrawLine(painter,P_3,P_1);
        }
    }
    /// RELLENO DE SUPERFICIE Gouraud
    if(Gouraud==true)
    {
        //Parámetros que definen el tamaño de la ventana (definirán igual el tamaño del Z-Buffer)
        int xdmaxb=1000;
        int ydmaxb=1000;
        int maxDepthb=-80.0;
        //Inicialización del Z-Buffer
        for(int i=0; i<xdmaxb; i++){
            for(int j=0; j<ydmaxb; j++){
                Buffer_Z(i,j) = maxDepthb;
            }
        }
        QVector2D P;
        QVector2D Q;

        QRgb EdgeColor;
        EdgeColor=qRgb(0,0,0);
        Line Line_(painter,1,EdgeColor);

        QVector3D V_Cam;
        V_Cam.setX(cam.V_q.x());
        V_Cam.setY(cam.V_q.y());
        V_Cam.setZ(cam.V_q.z());
        LightW.v_Luz=V_Cam.normalized();
        LightR.v_Luz=V_Cam.normalized();

        for(int f=0;f<M_Obj.getNC();f++)
        {
                Count=0;
                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                    Line_.pV[0]=P;
                    Line_.pV[1]=Q;
                    ScanConversion(painter,P,Q);
                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                    Line_.pV[2]=Q;
                    ScanConversion(painter,P,Q);

                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                    ScanConversion(painter,P,Q);
                    QVector3D V_NormalC[3];
                    //V1
                    V_NormalC[0].setX(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].x()-1].x());
                    V_NormalC[0].setY(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].x()-1].y());
                    V_NormalC[0].setZ(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].x()-1].z());
                    //V2
                    V_NormalC[1].setX(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].y()-1].x());
                    V_NormalC[1].setY(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].y()-1].y());
                    V_NormalC[1].setZ(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].y()-1].z());
                    //V3
                    V_NormalC[2].setX(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].z()-1].x());
                    V_NormalC[2].setY(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].z()-1].y());
                    V_NormalC[2].setZ(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].z()-1].z());
                    Dot=QVector3D::dotProduct(V_NormalC[0],V_Cam);
                    QRgb argb;
                    QVector3D camPosition;
                    QVector3D Ocam;

                    for(int in=0;in<3;in++){
                        Ocam.setX(cam.V_q(0));Ocam.setY(1+cam.V_q(1));Ocam.setZ(cam.V_q(2));
                        camPosition.setX(-0.2); camPosition.setY(-0.2); camPosition.setZ(-0.2);
                        argb=LightW.Light(M_Obj,V_NormalC[in],camPosition,Ocam.normalized());

                        Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1));Ocam.setZ(cam.V_q(2));
                        camPosition.setX(0.5); camPosition.setY(0.5); camPosition.setZ(0);
                        argb=argb+LightR.Light(M_Obj,V_NormalC[in],camPosition,Ocam.normalized());

                        ///Actualización del color de cada vertice para parametro de interpolación Bilineal
                        Line_.cV[in]=argb;
                    }
                int j=0;
                for(int i=0;i<Count;i++)
                {
                    P=QVector2D(CoordX[i],CoordY[i]);
                    Q=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    //if(verifBuffer(M_Obj,f,P.x(),P.y())){
                     //if(verifBuffer(M_Obj,f,Q.x(),Q.y())){
                        //if(Dot>0)
                            Line_.DrawLineGouraud(painter,P,Q);
                            //Line_.DrawLine(painter,P,Q);
                      //}
                    //}
                    j++;
                }

        }

    }

    /// RELLENO DE SUPERFICIE Phong
    if(Phong==true)
    {
        //Parámetros que definen el tamaño de la ventana (definirán igual el tamaño del Z-Buffer)
        int xdmaxb=1000;
        int ydmaxb=1000;
        int maxDepthb=-80.0;
        //Inicialización del Z-Buffer
        for(int i=0; i<xdmaxb; i++){
            for(int j=0; j<ydmaxb; j++){
                Buffer_Z(i,j) = maxDepthb;
            }
        }
        QVector2D P;
        QVector2D Q;

        QRgb EdgeColor;
        EdgeColor=qRgb(0,0,0);
        Line Line_(painter,1,EdgeColor);

        QVector3D V_Cam;
        V_Cam.setX(cam.V_q.x());
        V_Cam.setY(cam.V_q.y());
        V_Cam.setZ(cam.V_q.z());
        LightW.v_Luz=V_Cam.normalized();
        LightR.v_Luz=V_Cam.normalized();

        for(int f=0;f<M_Obj.getNC();f++)
        {
                Count=0;
                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                    Line_.pV[0]=P;
                    Line_.pV[1]=Q;
                    ScanConversion(painter,P,Q);

                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].y()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                    Line_.pV[2]=Q;
                    ScanConversion(painter,P,Q);

                    P.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].x());
                    P.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].z()-1].y());
                    Q.setX(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].x());
                    Q.setY(M_Obj.modeloVerticesP[M_Obj.modeloCaras[f].x()-1].y());
                    ScanConversion(painter,P,Q);

                    QVector3D V_NormalC[3];
                    //V1
                    V_NormalC[0].setX(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].x()-1].x());
                    V_NormalC[0].setY(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].x()-1].y());
                    V_NormalC[0].setZ(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].x()-1].z());
                    //V2
                    V_NormalC[1].setX(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].y()-1].x());
                    V_NormalC[1].setY(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].y()-1].y());
                    V_NormalC[1].setZ(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].y()-1].z());
                    //V3
                    V_NormalC[2].setX(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].z()-1].x());
                    V_NormalC[2].setY(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].z()-1].y());
                    V_NormalC[2].setZ(M_Obj.modeloNormalesVC[M_Obj.modeloCaras[f].z()-1].z());
                    Dot=QVector3D::dotProduct(V_NormalC[0],V_Cam);
                    QRgb argb;
                    QVector3D camPosition;
                    QVector3D Ocam;
                    Ocam.setX(cam.V_q(0));Ocam.setY(cam.V_q(1));Ocam.setZ(cam.V_q(2));
                    Ocam.normalize();
                    for(int in=0;in<3;in++){

                        camPosition.setX(-1); camPosition.setY(0); camPosition.setZ(0);
                        argb=LightW.Light(M_Obj,V_NormalC[in],camPosition,Ocam);

                        camPosition.setX(1); camPosition.setY(1); camPosition.setZ(0);
                        argb=argb+LightR.Light(M_Obj,V_NormalC[in],camPosition,Ocam);
                        ///Actualización del color de cada vertice para parametro de interpolación Bilineal
                        Line_.cV[in]=argb;
                    }
                int j=0;
                for(int i=0;i<Count;i++)
                {
                    P=QVector2D(CoordX[i],CoordY[i]);
                    Q=QVector2D(CoordX[Count-j],CoordY[Count-j]);
                    if(verifBuffer(M_Obj,f,P.x(),P.y())){
                     //if(verifBuffer(M_Obj,f,Q.x(),Q.y())){
                        //if(Dot>0)
                            Line_.DrawLineGouraud(painter,P,Q);
                      //}
                    }
                    j++;
                }
        }

    }
    fprintf(stderr, "-------- Renderizado -------\n");
}


bool renderObj::verifBuffer(ModeloOBJ &M_Obj,int idx,int i,int j){

    bool V1=false,V2=false,V3=false;
//cout<<"here i="<<i<<" j:"<<j<< " idx:"<<idx<<endl;
    if(i<0 || j<0 || i>1000 || j>1000)
        return false;

    float mayor=Buffer_Z(i,j);

    if(M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].x()-1].z()>Buffer_Z(i,j))
    {
        mayor=M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].x()-1].z();
        //Buffer_Z(i,j)=mayor;
        V1 =true;
    }
    if(M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].y()-1].z()>Buffer_Z(i,j))
    {
        if(mayor<M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].y()-1].z())
        {
            mayor=M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].y()-1].z();
            //Buffer_Z(i,j)=mayor;
        }
        V2= true;
    }
    if(M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].z()-1].z()>Buffer_Z(i,j))
    {
        if(mayor<M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].z()-1].z())
        {
            mayor=M_Obj.modeloVerticesC[M_Obj.modeloCaras[idx].z()-1].z();
            //Buffer_Z(i,j)=mayor;
        }
        V3= true;
    }

    if(V1==true && V2==true && V3==true){
        Buffer_Z(i,j)=mayor;
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

        /// APLICAR TRANSFORMACIONES A VERTICES Y NORMALES
        for(int i=0;i<M_Obj.getNV();i++)
        {
            /// COORDENADAS DEL MUNDO A CAMARA
            VerticesWT<<M_Obj.modeloVerticesW[i].x(),
                             M_Obj.modeloVerticesW[i].y(),
                             M_Obj.modeloVerticesW[i].z();
            VerticesW<<VerticesWT,1;
            VerticesC=cam.M_TC*VerticesW;
            M_Obj.modeloVerticesC.push_back(QVector3D(VerticesC(0),
                                                VerticesC(1),
                                                VerticesC(2)));
            VerticesP=cam.M_ProyOrt*VerticesC;

            /// PROYECCION AL DISPOSITIVO
            int dx= (int)(cam.sx*VerticesP(0)+cam.cx-0.5);
            int dy= (int)(cam.sy*VerticesP(1)+cam.cy-0.5);
            M_Obj.modeloVerticesP.push_back(QVector2D(dx,dy));
        }
         for(int i=0;i<M_Obj.getNV();i++)
        {
            /// TRANSFORMACIÓN NORMALES DE VERTICES A COORDENADAS DE LA CAMARA
            NormalesWT<<M_Obj.modeloNormalesVW[i].x(),
                             M_Obj.modeloNormalesVW[i].y(),
                             M_Obj.modeloNormalesVW[i].z();
            VerticesWT<<M_Obj.modeloVerticesW[i].x(),
                             M_Obj.modeloVerticesW[i].y(),
                             M_Obj.modeloVerticesW[i].z();
            NormalesWT=NormalesWT+VerticesWT;
            VerticesWT<<M_Obj.modeloVerticesC[i].x(),
                             M_Obj.modeloVerticesC[i].y(),
                             M_Obj.modeloVerticesC[i].z();
            VerticesW<<VerticesWT,1;
            NormalesW<<NormalesWT,1;
            //NormalesC=(cam.M_TC.inverse()).transpose()*NormalesW;
            NormalesC=(cam.M_TC*NormalesW)-VerticesW;
            //NormalesC=NormalesW;
            M_Obj.modeloNormalesVC.push_back(QVector3D(NormalesC(0),
                                                    NormalesC(1),
                                                    NormalesC(2)).normalized());
            /*M_Obj.modeloNormalesVC.push_back(QVector3D(NormalesWT(0),
                                                NormalesWT(1),
                                                NormalesWT(2)).normalized());*/
        }

        M_Obj.NormalesCarasC();
}

void renderObj::ScanConversion(QPainter &painter,QVector2D P,QVector2D Q){

    float Delta_x=(float)Q.x()-(float)P.x();
    float Delta_y=(float)Q.y()-(float)P.y();
    float Long;
    if(abs(Delta_x)>abs(Delta_y)){
       Long=abs(Delta_x);
    }else{
       Long=abs(Delta_y);
    }

    float increment_x=Delta_x/Long;
    float increment_y=Delta_y/Long;

    float x=P.x();
    float y=P.y();

    CoordX[Count]=(int)x;
    CoordY[Count]=(int)y;

        for(int i=1;i<=Long;i++)
        {
           Count++;
           x=x+increment_x;
           y=y+increment_y;
           CoordX[Count]=(int)x;
           CoordY[Count]=(int)y;
        }
}

Line::Line(QPainter &painter, int Widht, QRgb Edge)
{
    Widht=Widht;
    colorPoint.setWidth(Widht);
    colorPoint.setBrush((QBrush)Edge);
    painter.setPen(colorPoint);
}
///GOURAUD
QRgb Line::interpolacionBilinealColor(QVector2D P,QVector2D Q,float x,float y)
{

    QRgb fR1,fR2,fP;

    //fR1=((pV[1].x()-P.x())/(pV[1].x()-pV[0].x()))*cV[0]+((P.x()-pV[0].x())/(pV[1].x()-pV[0].x()))*cV[1];
    //fR2=((pV[2].x()-Q.x())/(pV[2].x()-pV[0].x()))*cV[0]+((Q.x()-pV[0].x())/(pV[2].x()-pV[0].x()))*cV[2];
    fR1=((pV[1].x()-x)/(pV[1].x()-pV[0].x()))*cV[0]+((x-pV[0].x())/(pV[1].x()-pV[0].x()))*cV[0];
    fR2=((pV[2].x()-x)/(pV[2].x()-pV[0].x()))*cV[0]+((x-pV[0].x())/(pV[2].x()-pV[0].x()))*cV[1];
    fP=((Q.y()-y)/(Q.y()-P.y()))*fR1+((y-P.y())/(Q.y()-P.y()))*fR2;
    return fP;

}

QVector3D interpolacionBilinealNormal(QVector3D N1, QVector3D N2, float x, float y)
{

    QVector3D N;


    return N;
}



int Line::randNumber(int low, int high)
{
     return qrand() % ((high + 1) - low) + low;
}

void Line::DrawLineGouraud(QPainter &painter, QVector2D P_1, QVector2D P_2)
{
    QRgb argb;
    QPen colorPoint;

        Delta_x=(float)P_2.x()-(float)P_1.x();
        Delta_y=(float)P_2.y()-(float)P_1.y();

        if(abs(Delta_x)>abs(Delta_y))
        {
           Long=abs(Delta_x);
        }else
        {
           Long=abs(Delta_y);
        }

        increment_x=(float)Delta_x/Long;
        increment_y=(float)Delta_y/Long;

        x=P_1.x();
        y=P_1.y();
        argb=interpolacionBilinealColor(P_1,P_2,x,y);
        colorPoint.setBrush((QBrush)argb);
        painter.setPen(colorPoint);
        painter.drawPoint(x,y);

        for(int i=1;i<=Long;i++)
        {
           x=x+increment_x;
           y=y+increment_y;
           argb=interpolacionBilinealColor(P_1,P_2,x,y);
           colorPoint.setBrush((QBrush)argb);
           painter.setPen(colorPoint);
           painter.drawPoint(x,y);
        }
}

void Line::DrawLine(QPainter &painter, QVector2D P_1, QVector2D P_2)
{
        Delta_x=(float)P_2.x()-(float)P_1.x();
        Delta_y=(float)P_2.y()-(float)P_1.y();

        if(abs(Delta_x)>abs(Delta_y))
        {
           Long=abs(Delta_x);
        }else
        {
           Long=abs(Delta_y);
        }

        increment_x=(float)Delta_x/Long;
        increment_y=(float)Delta_y/Long;

        x=P_1.x();
        y=P_1.y();
        painter.drawPoint(x,y);

        for(int i=1;i<=Long;i++)
        {
           x=x+increment_x;
           y=y+increment_y;
           painter.drawPoint(x,y);
        }
}



