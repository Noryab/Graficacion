#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>


using namespace std;




namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public slots:
    void CambiarColores();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int randNumber(int low, int high);

private:
    Ui::MainWindow *ui;
    QRgb argb;
    int sizeRect;
    int numRand;
};

#endif // MAINWINDOW_H
