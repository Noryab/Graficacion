#include "cube.h"

#include <QStandardItemModel>

Cube::~Cube(){

}

Cube::Cube(float l, QVector3D PosOriginW, QColor NodeColor, QColor EdgeColor)
{
    L=l;
    PosOW=PosOriginW;
    PosOI=QVector2D(300,300);

    NodeColor=NodeColor;
    EdgeColor=EdgeColor;

    Theta=0;
    Phi=0;
    Alpha=0;

    Nodes[0]=QVector3D(PosOW.x()-L,PosOW.y()-L,PosOW.z()-L);
    Nodes[1]=QVector3D(PosOW.x()-L,PosOW.y()-L,PosOW.z());
    Nodes[2]=QVector3D(PosOW.x()-L,PosOW.y(),PosOW.z()-L);
    Nodes[3]=QVector3D(PosOW.x()-L,PosOW.y(),PosOW.z());
    Nodes[4]=QVector3D(PosOW.x(),PosOW.y()-L,PosOW.z()-L);
    Nodes[5]=QVector3D(PosOW.x(),PosOW.y()-L,PosOW.z());
    Nodes[6]=QVector3D(PosOW.x(),PosOW.y(),PosOW.z()-L);
    Nodes[7]=QVector3D(PosOW.x(),PosOW.y(),PosOW.z());

    Face[2]=QVector4D(0,1,3,2);
    Face[3]=QVector4D(1,5,7,3);
    Face[4]=QVector4D(2,3,7,6);
    Face[5]=QVector4D(4,0,2,6);
    Face[6]=QVector4D(4,5,1,0);
    Face[7]=QVector4D(6,7,5,4);
    //Face[0]=QVector4D(0,1,2,3);
    //Face[1]=QVector4D(1,7,3,5);

    for(int i=2;i<8;i++){
        Faces[i-2][0]=Face[i].x();
        Faces[i-2][1]=Face[i].y();
        Faces[i-2][2]=Face[i].z();
        Faces[i-2][3]=Face[i].w();
        Faces[i-2][4]=Face[i].x();
    }

    QVector3D V,W;

    V=Nodes[2]-Nodes[0];
    W=Nodes[1]-Nodes[0];
    Normal[0]=QVector3D::crossProduct(V,W);
    Normal[0]=Normal[0].normalized();

    /*cout<<"No 0 x="<<Nodes[0].x()<<endl;
    cout<<"No 0 y="<<Nodes[0].y()<<endl;
    cout<<"No 0 z="<<Nodes[0].z()<<endl;

    cout<<"No 2 x="<<Nodes[2].x()<<endl;
    cout<<"No 2 y="<<Nodes[2].y()<<endl;
    cout<<"No 2 z="<<Nodes[2].z()<<endl;

    cout<<"No 1 x="<<Nodes[1].x()<<endl;
    cout<<"No 1 y="<<Nodes[1].y()<<endl;
    cout<<"No 1 z="<<Nodes[1].z()<<endl;*/


    cout<<"Normal 0 x="<<Normal[0].x()<<endl;
    cout<<"Normal 0 y="<<Normal[0].y()<<endl;
    cout<<"Normal 0 z="<<Normal[0].z()<<endl;

    V=Nodes[4]-Nodes[0];
    W=Nodes[2]-Nodes[0];
    Normal[3]=QVector3D::crossProduct(V,W);
    Normal[3]=Normal[3].normalized();

    cout<<"Normal 3 x="<<Normal[3].x()<<endl;
    cout<<"Normal 3 y="<<Normal[3].y()<<endl;
    cout<<"Normal 3 z="<<Normal[3].z()<<endl;


    V=Nodes[1]-Nodes[0];
    W=Nodes[4]-Nodes[0];
    Normal[4]=QVector3D::crossProduct(V,W);
    Normal[4]=Normal[4].normalized();

    cout<<"Normal 4 x="<<Normal[4].x()<<endl;
    cout<<"Normal 4 y="<<Normal[4].y()<<endl;
    cout<<"Normal 4 z="<<Normal[4].z()<<endl;


    Normal[1]=-1*Normal[3];
    Normal[2]=-1*Normal[4];
    Normal[5]=-1*Normal[0];


    edges[0]=QPoint(0,1);
    edges[1]=QPoint(1,3);
    edges[2]=QPoint(3,2);
    edges[3]=QPoint(2,0);
    edges[4]=QPoint(4,5);
    edges[5]=QPoint(5,7);
    edges[6]=QPoint(7,6);
    edges[7]=QPoint(6,4);
    edges[8]=QPoint(0,4);
    edges[9]=QPoint(1,5);
    edges[10]=QPoint(2,6);
    edges[11]=QPoint(3,7);
}

void Cube::DrawCube(QPainter &Painter){

    Line Line1(4,EdgeColor,NodeColor);
    QVector2D P,Q;

    for(int i=0;i<12;i++)
    {        
           P=QVector2D(Nodes[(int)edges[i].x()].x(),Nodes[(int)edges[i].x()].y())+PosOI;
           Q=QVector2D(Nodes[(int)edges[i].y()].x(),Nodes[(int)edges[i].y()].y())+PosOI;

           //Line1.DrawLine(Painter,P,Q,Normal[0]);
    }
    ScanConversion(Painter);
}

void Cube::ScanConversion(QPainter &Painter){

QVector2D P,Q;
int CoordX[500];
int CoordY[500];

    for(int f=0;f<6;f++)
    {
        int Count=0;
        for(int c=0;c<4;c++)
        {
            P=QVector2D(Nodes[Faces[f][c]].x(),Nodes[Faces[f][c]].y())+PosOI;
            Q=QVector2D(Nodes[Faces[f][c+1]].x(),Nodes[Faces[f][c+1]].y())+PosOI;

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

            CoordX[Count]=x;
            CoordY[Count++]=y;

            for(int i=1;i<=Long;i++) {
               x=x+increment_x;
               y=y+increment_y;
               CoordX[Count]=(int)x;
               CoordY[Count++]=(int)y;
               //cout<<Count<<endl;
           }
        }
        Line Line1(1,EdgeColor,NodeColor);
        int j=0;
        Count--;
        for(int i=0;i<Count;i++)
        {
            P=QVector2D(CoordX[i],CoordY[i]);
            Q=QVector2D(CoordX[Count-(j)],CoordY[Count-(j)]);
            j++;
            Line1.DrawLine(Painter,P,Q,Normal[f]);
        }
    }
}

Line::Line(){
    Widht=4;
}

Line::Line(int Widht, QColor Edge, QColor Nodecolor){
    Widht=Widht;
    colorPoint.setColor(Edge);
    colorPoint.setWidth(Widht);
}

int Line::randNumber(int low, int high)
{
     return qrand() % ((high + 1) - low) + low;
}


void Line::DrawLine(QPainter &Painter, QVector2D P_1, QVector2D P_2,QVector3D Normal){

QRgb argb;
        argb=Light.Ambient(Normal,QRgb(Qt::red));
        //cout<<argb<<endl;
        colorPoint.setBrush((QBrush)argb);

        //argb=qRgb(randNumber(0,255),randNumber(10,200),randNumber(0,255));
        //colorPoint.setBrush((QBrush)argb);

        Painter.setPen(colorPoint);

        Delta_x=(float)P_2.x()-(float)P_1.x();
        Delta_y=(float)P_2.y()-(float)P_1.y();

        if(abs(Delta_x)>abs(Delta_y)){
           Long=abs(Delta_x);           
        }else{
           Long=abs(Delta_y);           
        }
        increment_x=(float)Delta_x/Long;
        increment_y=(float)Delta_y/Long;

        x=P_1.x();
        y=P_1.y();
        Painter.drawPoint(x,y);

        for(int i=1;i<=Long;i++) {
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
}

