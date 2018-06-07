#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include "renderObj.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    renderObj Render;
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;


    public slots:
        void RotateX(int AngleX);
        void RotateY(int AngleY);
        void RotateZ(int AngleZ);        

};

#endif // MAINWINDOW_H
