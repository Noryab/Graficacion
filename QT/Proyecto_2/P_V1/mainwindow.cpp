#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderObj.h"

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

void MainWindow::RotateX(int AngleX){
    ui->widget->Angle_X=(float)AngleX;
    ui->widget->BoolRot=true;
}

void MainWindow::RotateY(int AngleY){
    ui->widget->Angle_Y=(float)AngleY;
    ui->widget->BoolRot=true;
}

void MainWindow::RotateZ(int AngleZ){
    ui->widget->Angle_Z=(float)AngleZ;
    ui->widget->BoolRot=true;
}

//********************************//

MainWindow::~MainWindow(){

    delete ui;
}

