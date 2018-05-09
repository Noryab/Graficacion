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
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        bool RDraw;

    private:
        Ui::MainWindow *ui;

    protected:
        void paintEvent(QPaintEvent *e ) override;

};

#endif // MAINWINDOW_H
