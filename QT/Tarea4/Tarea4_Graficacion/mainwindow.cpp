#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderObj.h"
#include <GL/glut.h>
#include <QDebug>
#include <QImage>
#include <vector>
#include <fstream>

#include "stdio.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette(ui->widget->palette());
    palette.setColor(backgroundRole(), Qt::black);
    setPalette(palette);

    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);

}


// *** Eventos de la interfaz ***//
void MainWindow::ChooseTextura(int numTexture)
{
    ui->widget->numTexture=numTexture;
    ui->widget->updateGL();

}

void MainWindow::RotateX(int AngleX){
    ui->widget->Angle_X=(float)AngleX;
    ui->widget->BoolRot=true;
    ui->widget->updateGL();
}

void MainWindow::RotateY(int AngleY){
    ui->widget->Angle_Y=(float)AngleY;
    ui->widget->BoolRot=true;
    ui->widget->updateGL();
}

void MainWindow::RotateZ(int AngleZ){
    ui->widget->Angle_Z=(float)AngleZ;
    ui->widget->BoolRot=true;
    ui->widget->updateGL();
}

void MainWindow::Gouraud()
{
    ui->widget->Gouraud=true;
    ui->widget->Phong=false;
    ui->widget->Poligon=false;
    ui->widget->Mall=false;
    ui->widget->Punt=false;
    ui->widget->Texture=false;
    ui->widget->updateGL();

}
void MainWindow::Phong()
{
    ui->widget->Gouraud=false;
    ui->widget->Phong=true;
    ui->widget->Poligon=false;
    ui->widget->Mall=false;
    ui->widget->Punt=false;
    ui->widget->Texture=false;
    ui->widget->updateGL();

}
void MainWindow::Poligonos()
{
    ui->widget->Gouraud=false;
    ui->widget->Phong=false;
    ui->widget->Poligon=true;
    ui->widget->Mall=false;
    ui->widget->Punt=false;
    ui->widget->Texture=false;
    ui->widget->updateGL();
}

void MainWindow::Mallas()
{
    ui->widget->Gouraud=false;
    ui->widget->Phong=false;
    ui->widget->Poligon=false;
    ui->widget->Mall=true;
    ui->widget->Punt=false;
    ui->widget->Texture=false;
    ui->widget->updateGL();
}


void MainWindow::Puntos()
{
    ui->widget->Gouraud=false;
    ui->widget->Phong=false;
    ui->widget->Poligon=false;
    ui->widget->Mall=false;
    ui->widget->Punt=true;
    ui->widget->Texture=false;
    ui->widget->updateGL();
}


void MainWindow::Textura()
{
    ui->widget->Gouraud=false;
    ui->widget->Phong=false;
    ui->widget->Poligon=false;
    ui->widget->Mall=false;
    ui->widget->Punt=false;
    ui->widget->Texture=true;
    ui->widget->updateGL();

}

//********************************//

MainWindow::~MainWindow(){

    delete ui;
}

