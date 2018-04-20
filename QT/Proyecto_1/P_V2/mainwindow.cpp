#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderObj.h"

#include "stdio.h"

    renderObj Render;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
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

    RDraw=true;
    Render.Gouraud=true;
    Render.Phong=false;
    Render.Poligon=false;
    Render.Mall=false;
    Render.Punt=false;
    Render.Textura=false;
    Render.RecalMatri=true;

    fprintf(stderr, "-------- Cargando Modelo -------\n");
    M_Obj.cargarModelo("/usr/bunny.obj");

}

void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);

    if(RDraw==true){
        //this->update();
        Render.RenderObj(M_Obj,painter);
        //this->update();
        RDraw=false;

    }
}


// *** Eventos de la interfaz ***//
void MainWindow::RotateX(int AngleX){
    Angle_X=(float)AngleX;
    M_Obj.RotateX(Angle_X);
    Render.RecalMatri=true;
    RDraw=true;   
    this->update();
}

void MainWindow::RotateY(int AngleY){
    Angle_Y=(float)AngleY;
    M_Obj.RotateY(Angle_Y);
    Render.RecalMatri=true;
    RDraw=true;
    this->update();
}

void MainWindow::RotateZ(int AngleZ){
    Angle_Z=(float)AngleZ;
    M_Obj.RotateZ(Angle_Z);
    Render.RecalMatri=true;
    RDraw=true;
    this->update();
}

void MainWindow::Gouraud()
{
    Render.Gouraud=true;
    Render.Phong=false;
    Render.Poligon=false;
    Render.Mall=false;
    Render.Punt=false;
    Render.Textura=false;
    Render.RecalMatri=false;
    RDraw=true;
    this->update();
}
void MainWindow::Phong()
{
    Render.Gouraud=false;
    Render.Phong=true;
    Render.Poligon=false;
    Render.Mall=false;
    Render.Punt=false;
    Render.Textura=false;
    Render.RecalMatri=false;
    RDraw=true;
    this->update();
}
void MainWindow::Poligonos()
{
    Render.Gouraud=false;
    Render.Phong=false;
    Render.Poligon=true;
    Render.Mall=false;
    Render.Punt=false;
    Render.Textura=false;
    Render.RecalMatri=false;
    RDraw=true;
    this->update();
}

void MainWindow::Mallas()
{
    Render.Gouraud=false;
    Render.Phong=false;
    Render.Poligon=false;
    Render.Mall=true;
    Render.Punt=false;
    Render.Textura=false;
    Render.RecalMatri=false;
    RDraw=true;
    this->update();
}


void MainWindow::Puntos()
{
    Render.Gouraud=false;
    Render.Phong=false;
    Render.Poligon=false;
    Render.Mall=false;
    Render.Punt=true;
    Render.Textura=false;
    Render.RecalMatri=false;
    RDraw=true;
    this->update();
}

void MainWindow::Textura()
{
    Render.Gouraud=false;
    Render.Phong=false;
    Render.Poligon=false;
    Render.Mall=false;
    Render.Punt=false;
    Render.Textura=true;
    Render.RecalMatri=false;
    RDraw=true;
    this->update();
}

//********************************//

MainWindow::~MainWindow(){

    delete ui;
}

