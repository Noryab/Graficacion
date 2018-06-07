#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderObj.h"

#include <vector>
#include <fstream>

#include "stdio.h"

renderObj Render;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette(ui->widget->palette());
    palette.setColor(backgroundRole(), Qt::white);
    setPalette(palette);

    RDraw=true;

    fprintf(stderr, "-------- Cargando Modelo -------\n");
    M_Obj.cargarModelo("/usr/bunny.obj");

}

// *** Eventos de la interfaz ***//
void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);
    if(RDraw==true){
        Render.RenderObj(M_Obj,painter);
        RDraw=false;
    }
}

void MainWindow::RotateX(int AngleX){
    Angle_X=(float)AngleX;

}

void MainWindow::RotateY(int AngleY){
    Angle_Y=(float)AngleY;

}

void MainWindow::RotateZ(int AngleZ){
    Angle_Z=(float)AngleZ;

}

//********************************//

MainWindow::~MainWindow(){

    delete ui;
}

