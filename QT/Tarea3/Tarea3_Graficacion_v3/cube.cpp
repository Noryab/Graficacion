#include "cube.h"

#include <QStandardItemModel>

Cube::~Cube(){

}

Cube::Cube(float l, QVector3D PosOriginW, QRgb NodColor, QRgb EdgColor)
{
    L=l;
    PosOW=PosOriginW;
    PosOI=QVector2D(200,200);

    NodeColor=NodColor;
    EdgeColor=EdgColor;

    Theta=0;
    Phi=0;
    Alpha=0;

    Nodes[0]=QVector3D(-L, L, L)+PosOW;
    Nodes[1]=QVector3D( L, L, L)+PosOW;
    Nodes[2]=QVector3D( L,-L, L)+PosOW;
    Nodes[3]=QVector3D(-L,-L, L)+PosOW;
    Nodes[4]=QVector3D( L, L,-L)+PosOW;
    Nodes[5]=QVector3D(-L, L,-L)+PosOW;
    Nodes[6]=QVector3D( L,-L,-L)+PosOW;
    Nodes[7]=QVector3D(-L,-L,-L)+PosOW;

    Face[2]=QVector4D(0,1,2,3);
    Face[3]=QVector4D(4,5,7,6);
    Face[4]=QVector4D(1,4,6,2);
    Face[5]=QVector4D(5,0,3,7);
    Face[6]=QVector4D(5,4,1,0);
    Face[7]=QVector4D(3,2,6,7);

    for(int i=2;i<8;i++){
        Faces[i-2][0]=Face[i].x();
        Faces[i-2][1]=Face[i].y();
        Faces[i-2][2]=Face[i].z();
        Faces[i-2][3]=Face[i].w();
        Faces[i-2][4]=Face[i].x();
    }

    V=Nodes[1]-Nodes[0];
    W=Nodes[3]-Nodes[0];
    Normal[0]=QVector3D::crossProduct(V,W);
    Normal[0]=Normal[0].normalized();
    Normal[1]=-1*Normal[0];
    V=Nodes[4]-Nodes[1];
    W=Nodes[2]-Nodes[1];
    Normal[2]=QVector3D::crossProduct(V,W);
    Normal[2]=Normal[2].normalized();
    Normal[3]=-1*Normal[2];

    V=Nodes[4]-Nodes[5];
    W=Nodes[0]-Nodes[5];
    Normal[4]=QVector3D::crossProduct(V,W);
    Normal[4]=Normal[4].normalized();
    Normal[5]=-1*Normal[4];

    edges[0]=QPoint(0,1);
    edges[1]=QPoint(1,2);   // 0
    edges[2]=QPoint(2,3);
    edges[3]=QPoint(3,0);

    edges[4]=QPoint(4,5);
    edges[5]=QPoint(5,7);   // 1
    edges[6]=QPoint(7,6);
    edges[7]=QPoint(6,4);

    edges[8]=QPoint(1,4);
    edges[9]=QPoint(2,6);   // 2
    edges[10]=QPoint(0,5);
    edges[11]=QPoint(3,7);

    MatMod=MatrixXd(4,4);
    MatView=MatrixXd(4,4);
    MatPro_Pers=MatrixXd(4,4);
    VP=MatrixXd(4,1);
    VPp=MatrixXd(4,1);
    VQ=MatrixXd(4,1);
    VQq=MatrixXd(4,1);
    Buffer_Z=MatrixXd::Ones(600,600)*(-1000);

     W1=1;
     Xc=0;
     Yc=0;
     f=10;
     w=40;
     h=40;
     le=50;
     c=10;
}

void Cube::DrawCube(QPainter &Painter){

    MatMod=MatrixXd::Identity(4,4);
    MatView=MatrixXd::Identity(4,4);
    MatPro_Pers=MatrixXd::Identity(4,4);
    MatPro_Pers(0,0)=f/w;
    MatPro_Pers(1,1)=f/h;
    MatPro_Pers(2,2)=le/(le-c);
    MatPro_Pers(3,2)=1.0;
    MatPro_Pers(2,3)=-f*le/(le-c);

    VP=MatrixXd::Zero(4,1);
    VQ=MatrixXd::Zero(4,1);

   ScanConversion(Painter);
}

QVector2D Cube::Proy_Disp(MatrixXd Xp){

    float XPmax=150;
    float YPmax=150;
    float XPmin=0;
    float YPmin=0;

    float Xdmax=200;
    float Ydmax=200;
    float Xdmin=0;
    float Ydmin=0;

    float Sx,Sy;
    float Cx,Cy;

    float Xd,Yd;

    Sx=(Xdmax-Xdmin)/(XPmax-XPmin);
    Sy=(Ydmax-Ydmin)/(YPmax-YPmin);

    Cx=Sx*(-XPmin)+Xdmin;
    Cy=Sy*(-YPmin)+Ydmin;

    Xd=Sx*Xp(0,0)+Cx-0.5;
    Yd=Sy*Xp(1,0)+Cy-0.5;

    return QVector2D(Xd,Yd);
}

void Cube::ScanConversion(QPainter &Painter){

QVector2D P,Q;
int CoordX[1500];
int CoordY[1500];

int Count=0;
float W2;
float Dot;
QVector3D Camara;
Camara=QVector3D(0,0,-1);

Buffer_Z=MatrixXd::Ones(600,600)*(-1000);
    for(int f=0;f<6;f=f+1)
    {       
        Dot=QVector3D::dotProduct(Normal[f],Camara);
        if(Dot>0){
            Count=0;
            for(int c=0;c<4;c=c+2)
            {
                VP(0,0)=(double)Nodes[(int)Faces[f][c]].x();
                VP(1,0)=(double)Nodes[(int)Faces[f][c]].y();
                VP(2,0)=(double)Nodes[(int)Faces[f][c]].z();
                VP(3,0)=1.0;
                VP=VP*W1;

                VPp=MatPro_Pers*VP;
                W2=VPp(3,0);
                //VPp=VPp*(1.0/W2);
                P=Proy_Disp(VPp);

                VQ(0,0)=(double)Nodes[(int)Faces[f][c+1]].x();
                VQ(1,0)=(double)Nodes[(int)Faces[f][c+1]].y();
                VQ(2,0)=(double)Nodes[(int)Faces[f][c+1]].z();
                VQ(3,0)=1.0;
                VQ=VQ*W1;

                VQq=MatPro_Pers*VQ;
                W2=VQq(3,0);
                //VQq=VQq*(1.0/W2);
                Q=Proy_Disp(VQq);
                //cout<<VQq<<endl;

                P=P+PosOI;
                Q=Q+PosOI;

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
                Line Line1(2,EdgeColor,NodeColor);
                int j=0;
                    for(int i=0;i<Count;i++)
                    {
                            P=QVector2D(CoordX[i],CoordY[i]);
                            Q=QVector2D(CoordX[Count-(j)],CoordY[Count-(j)]);
                            Line1.DrawLine(Painter,P,Q,Normal[f],EdgeColor);
                        j++;
                    }
        }
     }
    //exit(1);
}



Line::Line(int Widht, QRgb Edge, QRgb Nodecolor)
{
    Widht=Widht;
    colorPoint.setWidth(Widht);
    colorPoint.setBrush((QBrush)Edge);
}



int Line::randNumber(int low, int high)
{
     return qrand() % ((high + 1) - low) + low;
}



void Line::DrawLine(QPainter &Painter, QVector2D P_1, QVector2D P_2,QVector3D Normal, QRgb Edgecolor)
{
    QVector3D Camara;
    Camara=QVector3D(0,0,-1);
    QRgb argb;
        argb=Light.Ambient(Normal,Edgecolor);
        argb=argb+Light.Speculate(Normal,Camara,Edgecolor);
        colorPoint.setBrush((QBrush)argb);
        Painter.setPen(colorPoint);

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
        Painter.drawPoint(x,y);

        for(int i=1;i<=Long;i++)
        {
           x=x+increment_x;
           y=y+increment_y;
           Painter.drawPoint(x,y);
        }
}

// Rotate shape around the z-axis
void Cube::RotateCubeX(float _Theta)
{
    /// Rot X
    float sinTheta = sin((Theta-_Theta)*PI/180);
    float cosTheta = cos((Theta-_Theta)*PI/180);
    Theta=_Theta;
    for (int n=0; n<8; n++) {
        QVector3D node = Nodes[n];
        float y = node.y();
        float z = node.z();
        Nodes[n].setY(((y-PosOW.y())*cosTheta - (z-PosOW.z())*sinTheta)+PosOW.y());
        Nodes[n].setZ(((z-PosOW.z())*cosTheta + (y-PosOW.y())*sinTheta)+PosOW.z());
    }
    V=Nodes[1]-Nodes[0];
    W=Nodes[3]-Nodes[0];
    Normal[0]=QVector3D::crossProduct(V,W);
    Normal[0]=Normal[0].normalized();
    Normal[1]=-1*Normal[0];

    V=Nodes[4]-Nodes[1];
    W=Nodes[2]-Nodes[1];
    Normal[2]=QVector3D::crossProduct(V,W);
    Normal[2]=Normal[2].normalized();
    Normal[3]=-1*Normal[2];

    V=Nodes[4]-Nodes[5];
    W=Nodes[0]-Nodes[5];
    Normal[4]=QVector3D::crossProduct(V,W);
    Normal[4]=Normal[4].normalized();
    Normal[5]=-1*Normal[4];
}

void Cube::RotateCubeY(float _Phi)
{
    /// Rot Y
    float sinPhi = sin((Phi-_Phi)*PI/180);
    float cosPhi = cos((Phi-_Phi)*PI/180);
    Phi=_Phi;
    for (int n=0; n<8; n++) {
        QVector3D node = Nodes[n];
        float x = node.x();
        float z = node.z();
        Nodes[n].setX(((x-PosOW.x())*cosPhi - (z-PosOW.z())*sinPhi)+PosOW.x());
        Nodes[n].setZ(((z-PosOW.z())*cosPhi + (x-PosOW.x())*sinPhi)+PosOW.z());
    }
    V=Nodes[1]-Nodes[0];
    W=Nodes[3]-Nodes[0];
    Normal[0]=QVector3D::crossProduct(V,W);
    Normal[0]=Normal[0].normalized();
    Normal[1]=-1*Normal[0];

    V=Nodes[4]-Nodes[1];
    W=Nodes[2]-Nodes[1];
    Normal[2]=QVector3D::crossProduct(V,W);
    Normal[2]=Normal[2].normalized();
    Normal[3]=-1*Normal[2];

    V=Nodes[4]-Nodes[5];
    W=Nodes[0]-Nodes[5];
    Normal[4]=QVector3D::crossProduct(V,W);
    Normal[4]=Normal[4].normalized();
    Normal[5]=-1*Normal[4];
}

void Cube::RotateCubeZ(float _Alpha)
{
    /// Rot Z
    double sinAlpha = sin((Alpha-_Alpha)*PI/180);
    double cosAlpha = cos((Alpha-_Alpha)*PI/180);
    Alpha=_Alpha;
    for (int n=0; n<8; n++) {
        QVector3D node = Nodes[n];
        double x = node.x();
        double y = node.y();
        Nodes[n].setX(((x-PosOW.x())*cosAlpha - (y-PosOW.y())*sinAlpha)+PosOW.x());
        Nodes[n].setY(((y-PosOW.y())*cosAlpha + (x-PosOW.x())*sinAlpha)+PosOW.y());
    }

    V=Nodes[1]-Nodes[0];
    W=Nodes[3]-Nodes[0];
    Normal[0]=QVector3D::crossProduct(V,W);
    Normal[0]=Normal[0].normalized();
    Normal[1]=-1*Normal[0];

    V=Nodes[4]-Nodes[1];
    W=Nodes[2]-Nodes[1];
    Normal[2]=QVector3D::crossProduct(V,W);
    Normal[2]=Normal[2].normalized();
    Normal[3]=-1*Normal[2];

    V=Nodes[4]-Nodes[5];
    W=Nodes[0]-Nodes[5];
    Normal[4]=QVector3D::crossProduct(V,W);
    Normal[4]=Normal[4].normalized();
    Normal[5]=-1*Normal[4];
}

