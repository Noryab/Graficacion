#include "cube.h"

Cube::Cube(int high, int width, QPoint *PosOrigin)
{

    Lhigh=high;
    Lwidth=width;
    PosO[0]=PosOrigin[0];
    PosO[1]=PosOrigin[1];

}

void Cube::drawCube(QPainter &painter){

    for(int j=0;j<2;j++){
        P1[0+4*j]=PosO[j];
        P2[0+4*j].setX(PosO[j].x());
        P2[0+4*j].setY(PosO[j].y()+Lhigh);

        P1[1+4*j].setX(PosO[j].x());
        P1[1+4*j].setY(PosO[j].y()+Lhigh);
        P2[1+4*j].setX(PosO[j].x()+Lwidth);
        P2[1+4*j].setY(PosO[j].y()+Lhigh);

        P1[2+4*j].setX(PosO[j].x()+Lwidth);
        P1[2+4*j].setY(PosO[j].y()+Lhigh);
        P2[2+4*j].setX(PosO[j].x()+Lwidth);
        P2[2+4*j].setY(PosO[j].y());

        P1[3+4*j].setX(PosO[j].x()+Lwidth);
        P1[3+4*j].setY(PosO[j].y());
        P2[3+4*j]=PosO[j];

        for(int i=0+4*j;i<4+4*j;i++){
            drawLine(painter,P1[i],P2[i]);
        }

    }

    drawLine(painter,P1[0],P2[7]);
    drawLine(painter,P1[1],P2[4]);
    drawLine(painter,P1[2],P2[5]);
    drawLine(painter,P1[3],P2[6]);
}

void Cube::drawLine(QPainter &painter,QPoint P_1,QPoint P_2){

        Delta_x=P_2.x()-P_1.x();
        Delta_y=P_2.y()-P_1.y();

        if(abs(Delta_x)>abs(Delta_y)){
           Long=abs(Delta_x);
        }else{
           Long=abs(Delta_y);
        }
        increment_x=(float)Delta_x/Long;
        increment_y=(float)Delta_y/Long;

        x=P_1.x();
        y=P_1.y();
        painter.drawPoint(x,y);

        for(int i=1;i<=Long;i++) {
           x=x+increment_x;
           y=y+increment_y;
           painter.drawPoint(x,y);
        }

}

