#include <QStandardItemModel>

#include "renderObj.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Light.h"
#include "Ray.h"
#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"
#include "Object.h"
#include "Color.h"

using namespace std;

// Variables para objetos de la escena
    vector<Object*> Objects;
    vector<Color*> Lights;


//Constructor
renderObj::renderObj()
{    
/// Informacion de Camara
    dpi = 72;
    width = 640;
    height = 480;
    n = width*height;
    aadepth = 2;
    aathreshold = 0.1;
    aspectratio = (double)width/(double)height;
    ambientlight = 0.1;
    accuracy = 0.0001;

    O=QVector3D(0,0,0);
    X=QVector3D(1,0,0);
    Y=QVector3D(0,1,0);
    Z=QVector3D(0,0,1);

    CamPos=QVector3D(0, 0, -5);
    CamDir=QVector3D(0, 0, 1);
    CamDir=(CamPos-CamDir).normalized();

    CamRight= (QVector3D::crossProduct(Y,CamDir)).normalized();
    CamDown = (QVector3D::crossProduct(CamRight,CamDir)).normalized();

    Build();
}

renderObj::~renderObj()
{}

QVector3D renderObj::Tracing( const QVector3D &rayorig,const QVector3D &raydir,const int depth)
{
    Ray ray(rayorig,raydir);
    vector<double> intersections;

    for(int i=0;i<Objects.size();i++)
    {
        intersections.push_back(Objects.at(i)->findIntersection(ray));
    }



}

void renderObj::RenderObj(ModeloOBJ &M_Obj,QPainter &painter)
{        
    fprintf(stderr, "-------- Renderizando -------\n");
    /*Definimos las propiedades de la vista (perspectiva)*/
    float invWidth = 1 / float(width);
    float invHeight = 1 / float(height);
    float fov = 50;
    int depth=5;
    float angle;

    angle = tan(M_PI * 0.5 * fov / 200.);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            //Centro del pixel y lanzamos el rayo hacia esa dirección
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;

            QVector3D RayDir(xx,yy,0);
            QVector3D RayOri;

            RayOri=CamPos;

            QVector3D Color=Tracing(RayDir,RayOri,depth);

            //CP_rgb=qRgb(avgRed*255,avgGreen*255,avgBlue*255);
            colorPoint.setBrush((QBrush)CP_rgb);
            painter.setPen(colorPoint);
            painter.drawPoint(x+20,y+40);
        }
    }

}

/*Método para fabricar la escena que se va a renderizar*/
void renderObj::Build()
{
    /// Colores de objetos, paredes, pisos y otros
    QColor CSphere_Speculate (0.8, 0.8, 0.8, 0);
    QColor CSphere_Diffuse (0.5, 0.8, 0.5, 0);

    QColor Left_Wall (0.5, 0.8, 0.5, 0);
    QColor Right_Wall (0.5, 0.8, 0.5, 0);
    QColor Front_Wall (0.5, 0.8, 0.5, 0);
    QColor Back_Wall (0, 0, 0, 0);

    QColor UP_floor (0.5, 0.8, 0.5, 0);
    QColor Down_floor (0.5, 0.8, 0.5, 0);

    QColor Gray (0.5, 0.5, 0.5, 0);
    QColor Black (0, 0, 0, 0);
    QColor White(1, 1, 1, 0);

    QVector3D light_position (0,1,2.5);
    Light scene_light (light_position, White);
    Lights.push_back(dynamic_cast<Color*>(&scene_light));

    // Objetos de Escena
    QVector3D Sphere_Location_Speculate (0.75, 0, 3);
    QVector3D Sphere_Location_Diffuse   (1.95, 0, 2);

    Sphere Scene_Sphere_S (Sphere_Location_Speculate, 1, CSphere_Speculate);
    Sphere Scene_Sphere_D (Sphere_Location_Diffuse, 1, CSphere_Diffuse);

    QVector3D normal_plane_UF (0,1,0);
    Plane Top_wall (normal_plane_UF,-5.5,UP_floor);
    QVector3D normal_plane_DF (0,-1,0);
    Plane Back_wall (normal_plane_DF,-5,Down_floor);

    QVector3D normal_plane_BW (0,0,-1);
    Plane Back_plane (normal_plane_BW,-8,Back_Wall);
    QVector3D normal_plane_FW (0,0,5);
    Plane scene_plane (normal_plane_FW, -1, Front_Wall);
    QVector3D normal_plane_LW (1,0,0);
    Plane Left_wall (normal_plane_LW,-3.5,Left_Wall);
    QVector3D normal_plane_RW (1,0,0);
    Plane Right_wall (normal_plane_RW,3.5,Right_Wall);

    Objects.push_back(dynamic_cast<Object*>(&Scene_Sphere_D));
    Objects.push_back(dynamic_cast<Object*>(&Scene_Sphere_S));
    Objects.push_back(dynamic_cast<Object*>(&scene_plane));
    Objects.push_back(dynamic_cast<Object*>(&Back_plane));
    Objects.push_back(dynamic_cast<Object*>(&Top_wall));
    Objects.push_back(dynamic_cast<Object*>(&Back_wall));
    Objects.push_back(dynamic_cast<Object*>(&Left_wall));
    Objects.push_back(dynamic_cast<Object*>(&Right_wall));

    // Se deja el color negro por defecto
    CP_rgb=qRgb(0,0,0);
    colorPoint.setBrush((QBrush)CP_rgb);
}
