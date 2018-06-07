#include <QStandardItemModel>

#include "renderObj.h"

//Constructor
renderObj::renderObj(QWidget *parent):QWidget(parent)
{
    camOrig.setX(0);
    camOrig.setY(0);
    camOrig.setZ(0);

    qPoint.setX(0);
    qPoint.setY(0);
    qPoint.setZ(0);
    theta=0.0;
    Angle_X=0.0;
    Angle_Y=0.0;
    Angle_Z=0.0;
    BoolRot=false;

    Width=640;
    Height=480;

    fprintf(stderr, "-------- Cargando Modelo -------\n");
    M_Obj.cargarModelo("/home/noryab/Graficacion/QT/Proyecto_2/P_V1/bunny.obj");
}

renderObj::~renderObj()
{

}

void renderObj::RenderObj()
{        
    fprintf(stderr, "-------- Renderizado -------\n");



}

/*Método para fabricar la escena que se va a renderizar*/
void renderObj::Build()
{


}
