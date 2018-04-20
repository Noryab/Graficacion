#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;

    w.resize(1500,1000);
    w.move(200,0);
    w.setWindowTitle("Render");
    w.show();

    return a.exec();
}
