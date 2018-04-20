#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cube.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette(MainWindow::palette());

    palette.setColor(backgroundRole(), Qt::black);
    setPalette(palette);
}

void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);
    QPen colorPoint(Qt::red);
    colorPoint.setWidth(2);
    painter.setPen(colorPoint);

    QPoint PosOrigen1(200,100);
    QPoint PosOrigen2(350,350);
    QPoint PosOrigen[2];

    PosOrigen[0]=PosOrigen1;
    PosOrigen[1]=PosOrigen2;

    int width=100;
    int high=100;

    Cube Cube_1(high,width,PosOrigen);
    Cube_1.drawCube(painter);

}


MainWindow::~MainWindow(){

    delete ui;
}

