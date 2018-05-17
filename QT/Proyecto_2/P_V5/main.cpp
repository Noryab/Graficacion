#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{    

    QApplication a(argc, argv);
    MainWindow w;

    w.resize(680,540);
    w.move(600,30);
    w.setWindowTitle("Ray Tracing");
    w.show();

    return a.exec();
}
