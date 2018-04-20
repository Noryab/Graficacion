#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cube.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette(ui->widget->palette());
    palette.setColor(backgroundRole(), Qt::black);

    setPalette(palette);

    Angle_X=0.0;
    Angle_Y=0.0;
    Angle_Z=0.0;
    ui->spinBox_3->setValue(Angle_X);
    ui->spinBox_4->setValue(Angle_Y);
    ui->spinBox_5->setValue(Angle_Z);

    RDraw=false;    
}



void MainWindow::paintEvent(QPaintEvent *e){


    QPainter painter(this);

    float l=200;
    QVector3D PosOrigen(0,0,-200);
    QRgb EdgeColor;
    EdgeColor=qRgb(0,0,255);
    QRgb NodeColor(Qt::green);
    Cube Cube_1(l,PosOrigen,NodeColor,EdgeColor);

    Cube_1.RotateCubeX(Angle_X);
    Cube_1.RotateCubeY(Angle_Y);
    Cube_1.RotateCubeZ(Angle_Z);
    Cube_1.DrawCube(painter);

        if(RDraw==true  ){
            RDraw=false;
            //Cube_1.RDraw(painter);
            this->update();
        }
}


MainWindow::~MainWindow(){

    delete ui;
}

void MainWindow::RotateX(int AngleX){
    Angle_X=(float)AngleX;
    RDraw=true;
}

void MainWindow::RotateY(int AngleY){
    Angle_Y=(float)AngleY;
    RDraw=true;
}

void MainWindow::RotateZ(int AngleZ){
    Angle_Z=(float)AngleZ;
    RDraw=true;
}


