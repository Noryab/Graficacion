#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>

#include "obj.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        float Angle_X,Angle_Y,Angle_Z;
        bool RDraw;
        ModeloOBJ M_Obj;

    private:
        Ui::MainWindow *ui;

    protected:
        void paintEvent(QPaintEvent *e ) override;

    public slots:
        void RotateX(int AngleX);
        void RotateY(int AngleY);
        void RotateZ(int AngleZ);

        void Gouraud();
        void Phong();
        void Poligonos();
        void Mallas();
        void Puntos();
};

#endif // MAINWINDOW_H
