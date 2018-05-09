#include <QStandardItemModel>

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

#include "renderObj.h"
#include "Camera.h"
#include "Sphere.h"
#include "Light.h"
#include "Plane.h"

using namespace std;

//Recursividad de los rayos
int    depthMax = 1;


    QVector3D Tracing(vector<Object*> &Scene,vector<Light*> light_sources,const QVector3D &rayorig,const QVector3D &raydir,const int depth);

//Constructor
    renderObj::renderObj()
    {
    // Informacion de Camara
        dpi = 72;
        width = 640;
        height = 480;
        n = width*height;
        aspectratio = (double)width/(double)height;
        ambientlight = 0.1;
        depth=1;

        O=QVector3D(0,0,0);
        X=QVector3D(1,0,0);
        Y=QVector3D(0,1,0);
        Z=QVector3D(0,0,1);

        CamPos=QVector3D(0, 1, -2);
        CamDir=QVector3D(0, 0, 1);
        CamDir=(CamPos-CamDir).normalized();
        CamRight=(QVector3D::crossProduct(Y,CamDir)).normalized();
        CamDown =(QVector3D::crossProduct(CamRight,CamDir)).normalized();

    }

    renderObj::~renderObj()
    {}

    QVector3D Tracing(vector<Object*> &Scene,vector<Light*> light_sources,const QVector3D &rayorig,const QVector3D &raydir,const int depth)
    {
        QVector3D IntColor(0,0,0); // Color de Inter
        int idxIntnear=0;

        // Se verifica si el rayo intersecta algún objeto de la escena
        bool intersected=false;
        float InterDepth;
        float InterDepth_near=INFINITY;

        for(int numObj=0;numObj<Scene.size();numObj++)
        {
            if(Scene.at(numObj)->intersect(rayorig,raydir,InterDepth))
            {
                //cout<<"InterDepth:"<<InterDepth<<endl;
                if(InterDepth_near>InterDepth)
                {
                    InterDepth_near=InterDepth;
                    idxIntnear=numObj;
                    intersected=true;
                }
            }
        }
        // si no hay intersección retorna color de fondo
        if(intersected==false)
            return QVector3D(0,0,0);

        if(depthMax>depth)
        {                        
            QVector3D InterPoint = rayorig + raydir * InterDepth_near; //Punto de intersección

            QVector3D InterPoint_normal;
            // Normal del punto intersectado
            Scene.at(idxIntnear)->getNormal(InterPoint,raydir,InterPoint_normal);
            // Vector Reflejado
            QVector3D raydirR = raydir - 2 * QVector3D::dotProduct(raydir,InterPoint_normal) * InterPoint_normal;
            raydirR.normalize();
            QVector3D rayorigR=InterPoint + InterPoint_normal;
            // Rayo Recursivo
            IntColor =Tracing(Scene,light_sources,rayorigR, raydirR, depth + 1);

            //  reflection * fresneleffect
            float NdotView = std::max(0.f, QVector3D::dotProduct(InterPoint_normal,-raydirR));
            IntColor += Scene.at(idxIntnear)->getColor() * NdotView;

        }else
        {
          QVector3D InterPoint = rayorig + raydir * InterDepth_near; //Punto de intersección
          QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
          lightDirection.normalize();
          // Normal del punto intersectado
          QVector3D InterPoint_normal;
          Scene.at(idxIntnear)->getNormal(InterPoint,raydir,InterPoint_normal);

          IntColor= Scene.at(idxIntnear)->getColor() * std::max(float(0.0),QVector3D::dotProduct(InterPoint_normal,lightDirection)) * light_sources.at(0)->getLightColor();

        }
        double alllight = IntColor.x() + IntColor.y() + IntColor.z();
        double excesslight = alllight - 3;

        if (excesslight > 0)
        {
            IntColor.setX(IntColor.x() + excesslight*(IntColor.x()/alllight));
            IntColor.setY(IntColor.y() + excesslight*(IntColor.y()/alllight));
            IntColor.setZ(IntColor.z() + excesslight*(IntColor.z()/alllight));
        }
        if (IntColor.x() > 1) {IntColor.setX(1);}
        if (IntColor.y() > 1) {IntColor.setY(1);}
        if (IntColor.z() > 1) {IntColor.setZ(1);}
        if (IntColor.x() < 0) {IntColor.setX(0);}
        if (IntColor.y() < 0) {IntColor.setY(0);}
        if (IntColor.z() < 0) {IntColor.setZ(0);}

        return IntColor;
    }

    void renderObj::RenderObj(QPainter &painter)
    {
        // Variables para objetos de la escena
        vector<Object*> Scene;
        // Se deja el color negro por defecto
        this->CP_rgb=qRgb(0,0,0);
        colorPoint.setBrush((QBrush)CP_rgb);
        // Creación de Escena
            //Build(Scene);
            // Colores de objetos, paredes, pisos y otros
                QVector3D CSphere_Speculate (0.8, 0.85, 0.8);
                QVector3D CSphere_Diffuse (0.7, 0.6, 0.5);

                QVector3D CLeft_Wall (0.5, 0.1, 0.5);
                QVector3D CRight_Wall (0.98, 0.95, 0.85);
                QVector3D CFront_Wall (0.5, 0.8, 0.5);
                QVector3D CBack_Wall (0, 0, 0);

                QVector3D CUP_Floor (0.5, 0.8, 0.5);
                QVector3D CDown_Floor (0.98, 0.95, 0.8);

                QVector3D Gray (0.5, 0.5, 0.5);
                QVector3D Black (0, 0, 0);
                QVector3D White(0.98, 0.95, 0.85);

                QVector3D light_position (-1,1,1);
                Light scene_light (light_position, White);
                vector<Light*> light_sources;
                light_sources.push_back(dynamic_cast<Light*>(&scene_light));

            // Objetos de Escena
                QVector3D Sphere_Location_Speculate (-0.75, 0, 3);
                QVector3D Sphere_Location_Diffuse   (1.5, 0, 2);

                //Sphere::Sphere (QVector3D _Center, float _Radius, QVector3D _Color,float _Refl,float _Spec,float _Diff)
                float _Tran=1;
                float _Spec=1;
                float _Diff=1;
                Sphere Scene_Sphere_S (Sphere_Location_Speculate, 0.5, CSphere_Speculate,0,_Spec,0);
                Sphere Scene_Sphere_D (Sphere_Location_Diffuse, 0.5, CSphere_Diffuse,0,0,_Diff);

                QVector3D normal_plane_DF (0,-1,0);
                Plane Down_Floor(normal_plane_DF,-8,CDown_Floor);

                QVector3D normal_plane_TF (0,1,0);
                Plane UP_Floor (normal_plane_TF,-5.5,CUP_Floor);

                QVector3D normal_plane_FW (0,0,5);
                Plane Front_Wall (normal_plane_FW, -1, CFront_Wall);

                QVector3D normal_plane_BW (0,0,-1);
                Plane Back_wall (normal_plane_BW,-5,CBack_Wall);

                QVector3D normal_plane_LW (1,0,0);
                Plane Left_wall (normal_plane_LW,-3.5,CLeft_Wall);

                QVector3D normal_plane_RW (-1,0,0);
                Plane Right_wall (normal_plane_RW,3.5,CRight_Wall);


                Scene.push_back(dynamic_cast<Object*>(&Scene_Sphere_S));
                Scene.push_back(dynamic_cast<Object*>(&Scene_Sphere_D));
                //Scene.push_back(dynamic_cast<Object*>(&Down_Floor));
                //Scene.push_back(dynamic_cast<Object*>(&Right_wall));

        //Camera (QVector3D _Cpos, QVector3D _Cdir, QVector3D _Cright, QVector3D _Cdown);
        Camera CamRayTraicing(CamPos,CamDir,CamRight,CamDown);

        fprintf(stderr, "-------- Renderizando -------\n");

        //Definimos las propiedades de la vista (perspectiva)
        float aspectratio = width / float(height);
        float invHeight = 1 / float(height);
        float invWidth =  1 / float(width);
        float angle = 0;
        float fov = 100;

        angle = tan(M_PI * 0.5 * fov / 200.);

        QVector3D RayDir;
        QVector3D RayOri;

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                //Centro del pixel y lanzamos el rayo hacia esa dirección
                float x = (2 * ((i + 0.5) * invWidth) - 1) * angle * aspectratio;
                float y = (1 - 2 * ((j + 0.5) * invHeight)) * angle;
                // Se envía un rayo en el pixel actual
                RayDir=QVector3D(x,y,2);
                RayDir.normalize();
                RayOri=CamRayTraicing.getCameraPosition();

                // Se verifica si el rayo intersecta algún objeto de la escena
                bool intersected=false;
                float InterDepth;

                for(int numObj=0;numObj<Scene.size();numObj++)
                {
                    if(Scene.at(numObj)->intersect(RayOri,RayDir,InterDepth))
                        intersected=true;
                }

                QVector3D Color;
                // Si hay una intersección con algún objeto calculamos las trayectorias de los rayos
                if ( intersected )
                {
                    Color=Tracing(Scene,light_sources,RayOri,RayDir,depth);
                    //cout<<Color.x()<<endl;
                }else
                {
                    Color=QVector3D(0,0,0); //Negro
                }

                CP_rgb=qRgb(Color.x()*255,Color.y()*255,Color.z()*255);
                colorPoint.setBrush((QBrush)CP_rgb);
                painter.setPen(colorPoint);
                painter.drawPoint(i+20,j+40);
            }
        }

    }

