#ifndef MYRENDER_H
#define MYRENDER_H

#include <QGLWidget>
#include <QKeyEvent> //Para el manejo de teclas
#include "modelo3d.h"

class renderObj : public QGLWidget
{
    Q_OBJECT

    protected:

    public:
        explicit renderObj(QWidget *parent = 0);

        void initializeGL();
        void paintGL();
        void resizeGL(int w,int h);

    private:
        Modelo3d sphereModel;

};

#endif // MYRENDER_H
