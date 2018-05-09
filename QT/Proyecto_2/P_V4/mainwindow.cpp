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

}

// *** Eventos de la interfaz ***//
void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);
    if(RDraw==true){
        Render.RenderObj(painter);
        RDraw=false;
    }
}

//********************************//

MainWindow::~MainWindow(){

    delete ui;
}

