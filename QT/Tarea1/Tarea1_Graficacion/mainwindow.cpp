#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sizeRect=100;
    numRand=10;

}


void MainWindow::CambiarColores(){


    QGraphicsScene* scene=new QGraphicsScene();
    scene->setSceneRect(0,0,sizeRect,sizeRect);

    QImage imagen(sizeRect, sizeRect, QImage::Format_RGB888);
    imagen.fill(QColor(Qt::black).rgb());

     for (int y = 0; y < numRand; ++y) {
       argb=qRgb(randNumber(0,255), randNumber(0,255),randNumber(0,255));
       imagen.setPixel(randNumber(0,sizeRect-1), randNumber(0,sizeRect-1), argb);
     }

    scene->addPixmap(QPixmap::fromImage(imagen));

    ui->graphicsView->setScene(scene);
    ui->graphicsView->adjustSize();
    scene->update();
    ui->graphicsView->update();
}

int MainWindow::randNumber(int low, int high)
{

     return qrand() % ((high + 1) - low) + low;

}

MainWindow::~MainWindow()
{
    delete ui;
}
