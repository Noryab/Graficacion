#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>

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

private:
    Ui::MainWindow *ui;



protected:
    void paintEvent(QPaintEvent *e ) override;

public slots:
    void RotateX(int AngleX);
    void RotateY(int AngleY);
    void RotateZ(int AngleZ);


};

#endif // MAINWINDOW_H
