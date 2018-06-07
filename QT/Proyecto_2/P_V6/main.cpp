#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{    

    QApplication a(argc, argv);
    MainWindow w;

    w.resize(880,740);
    w.move(500,30);
    w.setWindowTitle("Ray Tracing");
    w.show();

    return a.exec();
}
