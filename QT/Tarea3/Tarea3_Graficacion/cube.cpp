#include "cube.h"

Cube::~Cube(){


}

Cube::Cube(float l, QVector3D PosOrigin)
{
    L=l;
    PosO=PosOrigin;
    EdgeColor=QColor(Qt::red);
    NodeColor=(Qt::green);

    Theta=45;
    Phi=45;
    Alpha=45;

    Nodes[0]=QVector3D(PosO.x()-L,PosO.y()-L,PosO.z()-L);
    Nodes[1]=QVector3D(PosO.x()-L,PosO.y()-L,PosO.z());
    Nodes[2]=QVector3D(PosO.x()-L,PosO.y(),PosO.z()-L);
    Nodes[3]=QVector3D(PosO.x()-L,PosO.y(),PosO.z());
    Nodes[4]=QVector3D(PosO.x(),PosO.y()-L,PosO.z()-L);
    Nodes[5]=QVector3D(PosO.x(),PosO.y()-L,PosO.z());
    Nodes[6]=QVector3D(PosO.x(),PosO.y(),PosO.z()-L);
    Nodes[7]=QVector3D(PosO.x(),PosO.y(),PosO.z());

    Face[0].setX(0);
    Face[0].setY(1);
    Face[0].setZ(3);
    Face[0].setW(2);
    Face[1]=QVector4D(1,5,7,3);
    Face[2]=QVector4D(2,3,7,6);
    Face[3]=QVector4D(4,0,2,6);
    Face[4]=QVector4D(4,5,1,0);
    Face[5]=QVector4D(5,4,6,7);

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


Cube::Cube(float l, QVector3D PosOrigin, QColor NodeColor, QColor EdgeColor)
{
    L=l;
    PosO=PosOrigin;

    NodeColor=NodeColor;
    EdgeColor=EdgeColor;

    Theta=0;
    Phi=0;
    Alpha=0;

    Nodes[0]=QVector3D(PosO.x()-L,PosO.y()-L,PosO.z()-L);
    Nodes[1]=QVector3D(PosO.x()-L,PosO.y()-L,PosO.z());
    Nodes[2]=QVector3D(PosO.x()-L,PosO.y(),PosO.z()-L);
    Nodes[3]=QVector3D(PosO.x()-L,PosO.y(),PosO.z());
    Nodes[4]=QVector3D(PosO.x(),PosO.y()-L,PosO.z()-L);
    Nodes[5]=QVector3D(PosO.x(),PosO.y()-L,PosO.z());
    Nodes[6]=QVector3D(PosO.x(),PosO.y(),PosO.z()-L);
    Nodes[7]=QVector3D(PosO.x(),PosO.y(),PosO.z());

    NodesV[0]=QVector3D(PosO.x()-L,PosO.y()-L,PosO.z()-L);
    NodesV[1]=QVector3D(PosO.x()-L,PosO.y()-L,PosO.z());
    NodesV[2]=QVector3D(PosO.x()-L,PosO.y(),PosO.z()-L);
    NodesV[3]=QVector3D(PosO.x()-L,PosO.y(),PosO.z());
    NodesV[4]=QVector3D(PosO.x(),PosO.y()-L,PosO.z()-L);
    NodesV[5]=QVector3D(PosO.x(),PosO.y()-L,PosO.z());
    NodesV[6]=QVector3D(PosO.x(),PosO.y(),PosO.z()-L);
    NodesV[7]=QVector3D(PosO.x(),PosO.y(),PosO.z());

    Face[0]=QVector4D(0,1,2,3);
    Face[1]=QVector4D(1,5,7,3);
    Face[2]=QVector4D(2,3,7,6);
    Face[3]=QVector4D(4,0,2,6);
    Face[4]=QVector4D(4,5,1,0);
    Face[5]=QVector4D(5,4,6,7);
    Face[6]=QVector4D(0,1,2,3);
    Face[7]=QVector4D(1,7,3,5);

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

    Line Line1(1,EdgeColor,NodeColor);
    QVector2D P,Q;

    for(int i=0;i<12;i++)
    {        
           P=QVector2D(Nodes[(int)edges[i].x()].x(),Nodes[(int)edges[i].x()].y());
           Q=QVector2D(Nodes[(int)edges[i].y()].x(),Nodes[(int)edges[i].y()].y());

           Line1.DrawLine(Painter,P,Q);
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

void Line::DrawLine(QPainter &Painter, QVector2D P_1, QVector2D P_2){
        colorPoint.setBrush(Qt::green);
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
        Nodes[n].setY(((y-PosO.y())*cosTheta - (z-PosO.z())*sinTheta)+PosO.y());
        Nodes[n].setZ(((z-PosO.z())*cosTheta + (y-PosO.y())*sinTheta)+PosO.z());
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
        Nodes[n].setX(((x-PosO.x())*cosPhi - (z-PosO.z())*sinPhi)+PosO.x());
        Nodes[n].setZ(((z-PosO.z())*cosPhi + (x-PosO.x())*sinPhi)+PosO.z());
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
        Nodes[n].setX(((x-PosO.x())*cosAlpha - (y-PosO.y())*sinAlpha)+PosO.x());
        Nodes[n].setY(((y-PosO.y())*cosAlpha + (x-PosO.x())*sinAlpha)+PosO.y());
    }
}

