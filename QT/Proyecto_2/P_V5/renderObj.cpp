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
int    depthMax =2;

    QVector3D Tracing(vector<Object*> &Scene,vector<Light*> light_sources,const QVector3D &rayorig,const QVector3D &raydir,const int depth);

    int winningObjectIndex(vector<double> object_intersections)
    {
        // return the index of the winning intersection
        int index_of_minimum_value;

        // prevent unnessary calculations
        if (object_intersections.size() == 0) {
            // if there are no intersections
            return -1;
        }
        else if (object_intersections.size() == 1) {
            if (object_intersections.at(0) > 0) {
                // if that intersection is greater than zero then its our index of minimum value
                return 0;
            }
            else {
                // otherwise the only intersection value is negative
                return -1;
            }
        }
        else {
            // otherwise there is more than one intersection
            // first find the maximum value

            double max = 0;
            for (int i = 0; i < object_intersections.size(); i++) {
                if (max < object_intersections.at(i)) {
                    max = object_intersections.at(i);
                }
            }

            // then starting from the maximum value find the minimum positive value
            if (max >0) {
                // we only want positive intersections
                for (int index = 0; index < object_intersections.size(); index++) {
                    if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
                        max = object_intersections.at(index);
                        index_of_minimum_value = index;
                    }
                }

                return index_of_minimum_value;
            }
            else {
                // all the intersections were negative
                return -1;
            }
        }
    }


//Constructor
    renderObj::renderObj()
    {
    // Informacion de Camara
        width = 640;
        height = 480;
        n = width*height;
        aspectratio = (double)width/(double)height;
        ambientlight = 0.1;
        depth=1;

        O=QVector3D(0,1,0);
        X=QVector3D(1,0,0);
        Y=QVector3D(0,1,0);
        Z=QVector3D(0,0,1);

        CamPos=QVector3D(0, 1, 0);
        CamDir=QVector3D(0, 1, 1);
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
        float InterDepth_near=INFINITY;

        vector<double> intersections;

        for(int numObj=0;numObj<Scene.size();numObj++)
        {
            intersections.push_back(Scene.at(numObj)->intersect(rayorig,raydir));
        }
        idxIntnear = winningObjectIndex(intersections);

        if(idxIntnear==-1)
            return QVector3D(0,0,1);

        InterDepth_near=intersections.at(idxIntnear);

        if(depthMax>depth && Scene.at(idxIntnear)->getDiff()>0 )
        {                        
            QVector3D InterPoint = rayorig + raydir * InterDepth_near; //Punto de intersección
            QVector3D InterPoint_normal;            
            Scene.at(idxIntnear)->getNormal(InterPoint,InterPoint_normal);
            if (QVector3D::dotProduct(-raydir,InterPoint_normal) < 0){
                InterPoint_normal = -InterPoint_normal;
            }
            QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
            QVector3D distance_to_light = (light_sources.at(0)->getLightPosition()-InterPoint);
            float distance_to_light_magnitude=sqrt(distance_to_light.x()*distance_to_light.x()+
                                                   distance_to_light.y()*distance_to_light.y()+
                                                   distance_to_light.z()*distance_to_light.z());
            // Vector Reflejado
            QVector3D h =(2 * QVector3D::dotProduct(InterPoint_normal,raydir)* InterPoint_normal -raydir).normalized();
            // Vector de origen
            // Vector Reflejado
            QVector3D raydirR = (raydir - 2 * InterPoint_normal * QVector3D::dotProduct(raydir,InterPoint_normal) ).normalized();
            QVector3D rayorigR=InterPoint + InterPoint_normal*1e-4;

            // Rayo Recursivo
                IntColor =Tracing(Scene,light_sources,rayorigR, raydirR, depth + 1);
                /// Shadows
                vector<double> intersectionsShadows;
                for(int numObj=0;numObj<Scene.size();numObj++)
                {
                    intersectionsShadows.push_back(Scene.at(numObj)->intersect(InterPoint,lightDirection));
                }
                int idxIntShadow = winningObjectIndex(intersectionsShadows);

                if (intersectionsShadows.at(idxIntShadow) <= distance_to_light_magnitude  && idxIntnear!=idxIntShadow) {
                    h =(2 * QVector3D::dotProduct(InterPoint_normal,lightDirection)* InterPoint_normal -lightDirection).normalized();
                    IntColor+=(depth/distance_to_light_magnitude)*
                             (Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-raydir,h),4) );
                    float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
                    IntColor=(IntColor)*NdotView;
                }else{
                    h =(2 * QVector3D::dotProduct(InterPoint_normal,lightDirection)* InterPoint_normal -lightDirection).normalized();
                    IntColor+=Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-raydir,h),4) ;
                    float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
                    IntColor=(IntColor)*NdotView;
                }
        }
        if(depthMax>depth && Scene.at(idxIntnear)->getSpec()>0 )
        {
            //Punto de intersección
            QVector3D InterPoint = rayorig + raydir * InterDepth_near;
            QVector3D InterPoint_normal;
            // Normal del punto intersectado
            Scene.at(idxIntnear)->getNormal(InterPoint,InterPoint_normal);

            if (QVector3D::dotProduct(-raydir,InterPoint_normal) < 0)
            {
                InterPoint_normal = -InterPoint_normal;
            }
            QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
            // Vector Reflejado
            QVector3D raydirR = (raydir - 2 * InterPoint_normal * QVector3D::dotProduct(raydir,InterPoint_normal) ).normalized();
            QVector3D rayorigR=InterPoint + InterPoint_normal*1e-5;
            QVector3D h=(2*QVector3D::dotProduct(InterPoint_normal,raydir)*InterPoint_normal-raydir).normalized();
            // Rayo Recursivo
            IntColor =Tracing(Scene,light_sources,rayorigR, raydirR, depth + 1);

            /// Shadows
            vector<double> intersectionsShadows;
            for(int numObj=0;numObj<Scene.size();numObj++)
            {
                intersectionsShadows.push_back(Scene.at(numObj)->intersect(InterPoint,lightDirection));
            }
            int idxIntShadow = winningObjectIndex(intersectionsShadows);
            QVector3D distance_to_light = (light_sources.at(0)->getLightPosition()-InterPoint);
            float distance_to_light_magnitude=sqrt(distance_to_light.x()*distance_to_light.x()+
                                                   distance_to_light.y()*distance_to_light.y()+
                                                   distance_to_light.z()*distance_to_light.z());
            if (intersectionsShadows.at(idxIntShadow) <= distance_to_light_magnitude  && idxIntnear!=idxIntShadow) {
                h =(2 * QVector3D::dotProduct(InterPoint_normal,lightDirection)* InterPoint_normal -lightDirection).normalized();
                IntColor+=(1/distance_to_light_magnitude)*Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-raydir,h),4) ;
                float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
                IntColor=(IntColor)*NdotView;

            }else{
                h =(2 * QVector3D::dotProduct(InterPoint_normal,lightDirection)* InterPoint_normal -lightDirection).normalized();
                IntColor+=Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-raydir,h),4) ;
                float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
                IntColor=(IntColor)*NdotView;
            }
            /*float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
            IntColor += (Scene.at(idxIntnear)->getSpec()*Scene.at(idxIntnear)->getColor() * NdotView);*/
        }
        else
        {
          //Punto de intersección
            QVector3D InterPoint = rayorig + raydir * InterDepth_near;
          // Normal del punto intersectado
            QVector3D InterPoint_normal;
            Scene.at(idxIntnear)->getNormal(InterPoint,InterPoint_normal);
            QVector3D raydirR = (raydir - 2 * InterPoint_normal * QVector3D::dotProduct(raydir,InterPoint_normal) ).normalized();

            QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
            lightDirection.normalize();
            if (QVector3D::dotProduct(-raydir,InterPoint_normal) < 0)
            {
                InterPoint_normal = InterPoint_normal;
            }


          /// Shadows
            QVector3D distance_to_light = (light_sources.at(0)->getLightPosition()-InterPoint);
            float distance_to_light_magnitude=sqrt(distance_to_light.x()*distance_to_light.x()+
                                                 distance_to_light.y()*distance_to_light.y()+
                                                 distance_to_light.z()*distance_to_light.z());
          vector<double> intersectionsShadows;
          for(int numObj=0;numObj<Scene.size();numObj++)
          {
              intersectionsShadows.push_back(Scene.at(numObj)->intersect(InterPoint,lightDirection));
          }
          int idxIntShadow = winningObjectIndex(intersectionsShadows);

          if (intersectionsShadows.at(idxIntShadow) <= distance_to_light_magnitude  && idxIntnear!=idxIntShadow) {
              QVector3D h=2*QVector3D::dotProduct(InterPoint_normal,lightDirection)*InterPoint_normal-lightDirection;
              IntColor=(1/distance_to_light_magnitude)*
                       (Scene.at(idxIntnear)->getAmb()*light_sources.at(0)->getLightColor() +
                        Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*QVector3D::dotProduct(InterPoint_normal,lightDirection)+
                        Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-raydir,h),4) );
              float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
              IntColor=(IntColor)*NdotView;
          }else{
              QVector3D h=2*QVector3D::dotProduct(InterPoint_normal,lightDirection)*InterPoint_normal-lightDirection;
              IntColor=Scene.at(idxIntnear)->getAmb()*light_sources.at(0)->getLightColor() +
                       Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*QVector3D::dotProduct(InterPoint_normal,lightDirection) +
                       Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-raydir,h),4) ;
              float NdotView = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-raydir));
              IntColor=(IntColor)*NdotView;
          }
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
                QVector3D CSphere_Speculate (0.8, 0.95, 0.8);
                QVector3D CSphere_Diffuse (0.7, 0.6, 0.6);

                QVector3D CLeft_Wall (0.905, 0.145, 0.07);
                QVector3D CRight_Wall (0.156, 0.447, 0.2);
                QVector3D CFront_Wall (0.98, 0.95, 0.8);
                QVector3D CBack_Wall (0.1, 0.1, 0.1);

                QVector3D CTop_Floor (0.98, 0.95, 0.8);
                QVector3D CDown_Floor (0.98, 0.95, 0.8);

                QVector3D Gray (0.5, 0.5, 0.5);
                QVector3D Black (0, 0, 0);
                QVector3D White(0.8, 0.8, 0.8);

                QVector3D light_position (0,2.2,1);
                Light scene_light (light_position, White);
                vector<Light*> light_sources;
                light_sources.push_back(dynamic_cast<Light*>(&scene_light));

                //Sphere::Sphere (QVector3D _Center, float _Radius, QVector3D _Color,float _Refl,float _Spec,float _Diff)
                float _Amb=0.2;
                float _Spec=0.7;
                float _Diff=0.7;
                // Objetos de Escena
                QVector3D Sphere_Location_Speculate (-0.5, 0.6, 1.8);
                QVector3D Sphere_Location_Diffuse   (0.6, 0.5, 1.6);
                Sphere Scene_Sphere_S (Sphere_Location_Speculate, 0.6, CSphere_Speculate,_Amb,0,_Spec);
                Sphere Scene_Sphere_D (Sphere_Location_Diffuse, 0.5, CSphere_Diffuse,_Amb,_Diff,0);

                QVector3D normal_plane_DF (0,1,0);
                Plane Down_Floor(normal_plane_DF,0,CDown_Floor,_Amb,0,_Diff);
                QVector3D normal_plane_TF (0,-1,0);
                Plane Top_Floor (normal_plane_TF,2.5,CTop_Floor,_Amb,0,_Diff);

                QVector3D normal_plane_BW (0,0,1);
                Plane Back_Wall (normal_plane_BW,0.5,CBack_Wall,_Amb,0,_Diff);
                QVector3D normal_plane_FW (0,0,-1);
                Plane Front_Wall (normal_plane_FW, 2.6, CFront_Wall,_Amb,0,_Diff);

                QVector3D normal_plane_LW (1,0,0);
                Plane Left_Wall (normal_plane_LW,1.5,CLeft_Wall,_Amb,0,_Diff);
                QVector3D normal_plane_RW (-1,0,0);
                Plane Right_Wall (normal_plane_RW,1.5,CRight_Wall,_Amb,0,_Diff);

                Scene.push_back(dynamic_cast<Object*>(&Scene_Sphere_S));
                Scene.push_back(dynamic_cast<Object*>(&Scene_Sphere_D));
                Scene.push_back(dynamic_cast<Object*>(&Down_Floor));
                Scene.push_back(dynamic_cast<Object*>(&Top_Floor));
                Scene.push_back(dynamic_cast<Object*>(&Right_Wall));
                Scene.push_back(dynamic_cast<Object*>(&Left_Wall));
                Scene.push_back(dynamic_cast<Object*>(&Front_Wall));
                Scene.push_back(dynamic_cast<Object*>(&Back_Wall));

        //Camera (QVector3D _Cpos, QVector3D _Cdir, QVector3D _Cright, QVector3D _Cdown);
        Camera CamRayTraicing(CamPos,CamDir,CamRight,CamDown);

        fprintf(stderr, "-------- Renderizando -------\n");

        //Definimos las propiedades de la vista (perspectiva)
        float aspectratio = width / float(height);
        float invHeight = 1 / float(height);
        float invWidth =  1 / float(width);
        float fov = 100;
        float angle;


        angle = tan(M_PI * 0.5 * fov / 200.0);

        QVector3D RayDir;
        QVector3D RayOri;

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                //cout<<"i: "<<i<<"   j: "<<j<<endl;
                //Centro del pixel y lanzamos el rayo hacia esa dirección
                float x = (2.0 * ((i + 0.5) * invWidth) - 1.0) * angle * aspectratio;
                float y = (1.0 - 2.0 * ((j + 0.5) * invHeight)) * angle;

                // Se envía un rayo en el pixel actual
                RayDir=QVector3D(x,y,1);
                RayDir.normalize();
                RayOri=CamRayTraicing.getCameraPosition();

                QVector3D Color;
                // Si hay una intersección con algún objeto calculamos las trayectorias de los rayos
                Color=Tracing(Scene,light_sources,RayOri,RayDir,depth);

                CP_rgb=qRgb(Color.x()*255,Color.y()*255,Color.z()*255);
                colorPoint.setBrush((QBrush)CP_rgb);
                painter.setPen(colorPoint);
                painter.drawPoint(i+20,j+40);
            }
        }

    }
