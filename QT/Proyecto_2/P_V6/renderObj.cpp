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

    int    depthMax = 4;
    QVector3D Tracing(std::vector<Object*> &Scene,std::vector<Light*> light_sources,const QVector3D &rayorig,const QVector3D &eye_dir,const int depth);
    int idxObject(std::vector<double> intersections);

    renderObj::renderObj()
    {    
        width = 840;
        height = 680;
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

    QVector3D Tracing(std::vector<Object*> &Scene,std::vector<Light*> light_sources,const QVector3D &rayorig,const QVector3D &eye_dir,const int depth)
    {
        QVector3D IntColor(0,0,0);
        int idxIntnear=0;
        float InterDepth_near=3000;

        std::vector<double> intersections;

        for(int numObj=0;numObj<(int)Scene.size();numObj++)
        {
            intersections.push_back(Scene.at(numObj)->intersect(rayorig,eye_dir));
        }
        idxIntnear = idxObject(intersections);

        if(idxIntnear==-1)
            return QVector3D(0,0,1);

        InterDepth_near=intersections.at(idxIntnear);

        if(depthMax>depth && Scene.at(idxIntnear)->getDiff()>0 )
        {                        
            QVector3D InterPoint = rayorig + eye_dir * InterDepth_near;
            QVector3D InterPoint_normal;            
            Scene.at(idxIntnear)->getNormal(InterPoint,InterPoint_normal);

            QVector3D h =(2 * QVector3D::dotProduct(InterPoint_normal,eye_dir)* InterPoint_normal -eye_dir).normalized();
            QVector3D eye_dirR = (eye_dir - 2 * InterPoint_normal * QVector3D::dotProduct(eye_dir,InterPoint_normal) ).normalized();
            QVector3D rayorigR=InterPoint + InterPoint_normal*1e-4;

                IntColor +=Tracing(Scene,light_sources,rayorigR, h, depth + 1)*Scene.at(idxIntnear)->getDiff();
                /// Shadows
                QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
                QVector3D distance_to_light = (light_sources.at(0)->getLightPosition()-InterPoint);
                float distance_to_light_magnitude=sqrt(distance_to_light.x()*distance_to_light.x()+
                                                       distance_to_light.y()*distance_to_light.y()+
                                                       distance_to_light.z()*distance_to_light.z());
                float View_Vector = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-eye_dir));
                std::vector<double> intersectionsShadows;
                for(int numObj=0;numObj<(int)Scene.size();numObj++)
                {
                    intersectionsShadows.push_back(Scene.at(numObj)->intersect(InterPoint,lightDirection));
                }

                    IntColor+=Scene.at(idxIntnear)->getAmb()*light_sources.at(0)->getLightColor()*Scene.at(idxIntnear)->getColor() +
                             Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*QVector3D::dotProduct(InterPoint_normal,lightDirection) +
                             Scene.at(idxIntnear)->getSpec()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-eye_dir,eye_dirR),2) ;
                    IntColor=(IntColor/(distance_to_light_magnitude))*View_Vector;

        }
        if(depthMax>depth && Scene.at(idxIntnear)->getSpec()>0 )
        {
            QVector3D InterPoint = rayorig + eye_dir * InterDepth_near;
            QVector3D InterPoint_normal;

            Scene.at(idxIntnear)->getNormal(InterPoint,InterPoint_normal);
            QVector3D h =(2 * QVector3D::dotProduct(InterPoint_normal,eye_dir)* InterPoint_normal -eye_dir).normalized();
            QVector3D eye_dirR = (eye_dir - 2 * InterPoint_normal * QVector3D::dotProduct(eye_dir,InterPoint_normal) ).normalized();
            QVector3D rayorigR=InterPoint + InterPoint_normal*1e-4;
            IntColor +=Tracing(Scene,light_sources,rayorigR, eye_dirR, depth + 1)* Scene.at(idxIntnear)->getSpec();

            float View_Vector = std::max(float(0.0), QVector3D::dotProduct(InterPoint_normal,-h));
            QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
            QVector3D distance_to_light = (light_sources.at(0)->getLightPosition()-InterPoint);
            float distance_to_light_magnitude=sqrt(distance_to_light.x()*distance_to_light.x()+
                                                   distance_to_light.y()*distance_to_light.y()+
                                                   distance_to_light.z()*distance_to_light.z());
            std::vector<double> intersectionsShadows;
            for(int numObj=0;numObj<(int)Scene.size();numObj++)
            {
                intersectionsShadows.push_back(Scene.at(numObj)->intersect(InterPoint,lightDirection));
            }
            int idxIntShadow = idxObject(intersectionsShadows);
                if (intersectionsShadows.at(idxIntShadow) <= distance_to_light_magnitude || idxIntnear!=idxIntShadow) {
                    IntColor+= (1/distance_to_light_magnitude*depth)*Scene.at(idxIntnear)->getSpec()*Scene.at(idxIntnear)->getColor() * std::max(float(0.0),QVector3D::dotProduct(InterPoint_normal,lightDirection)) ;
                    IntColor=IntColor*View_Vector;

                }else{
                    IntColor += Scene.at(idxIntnear)->getSpec()*Scene.at(idxIntnear)->getColor()*View_Vector* std::max(float(0.0),QVector3D::dotProduct(InterPoint_normal,lightDirection)) ;
                    IntColor=IntColor*View_Vector;
                }
        }
        else
        {
          QVector3D InterPoint = rayorig + eye_dir * InterDepth_near;
          QVector3D lightDirection = light_sources.at(0)->getLightPosition() - InterPoint;
          lightDirection.normalize();
          QVector3D InterPoint_normal;
          Scene.at(idxIntnear)->getNormal(InterPoint,InterPoint_normal);
          QVector3D eye_dirR = (eye_dir - 2 * InterPoint_normal * QVector3D::dotProduct(eye_dir,InterPoint_normal) ).normalized();
          QVector3D distance_to_light = (light_sources.at(0)->getLightPosition()-InterPoint);
          float distance_to_light_magnitude=sqrt(distance_to_light.x()*distance_to_light.x()+
                                                 distance_to_light.y()*distance_to_light.y()+
                                                 distance_to_light.z()*distance_to_light.z());
          std::vector<double> intersectionsShadows;
          for(int numObj=0;numObj<(int)Scene.size();numObj++)
          {
              intersectionsShadows.push_back(Scene.at(numObj)->intersect(InterPoint,lightDirection));
          }
          int idxIntShadow = idxObject(intersectionsShadows);

          if (intersectionsShadows.at(idxIntShadow) <= distance_to_light_magnitude  && idxIntnear!=idxIntShadow) {
              IntColor=Scene.at(idxIntnear)->getAmb()*light_sources.at(0)->getLightColor()*Scene.at(idxIntnear)->getColor() +
                       Scene.at(idxIntnear)->getSpec()*Scene.at(idxIntnear)->getColor()*QVector3D::dotProduct(InterPoint_normal,lightDirection) +
                       Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-eye_dir,eye_dirR),4) ;
              IntColor*=(1/(distance_to_light_magnitude*depth));

          }else{
              IntColor=Scene.at(idxIntnear)->getAmb()*light_sources.at(0)->getLightColor()*Scene.at(idxIntnear)->getColor() +
                       Scene.at(idxIntnear)->getSpec()*Scene.at(idxIntnear)->getColor()*QVector3D::dotProduct(InterPoint_normal,lightDirection) +
                       Scene.at(idxIntnear)->getDiff()*Scene.at(idxIntnear)->getColor()*pow(QVector3D::dotProduct(-eye_dir,eye_dirR),2) ;
              IntColor/=depth;

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
        std::vector<Object*> Scene;
        this->CP_rgb=qRgb(0,0,0);
        colorPoint.setBrush((QBrush)CP_rgb);
        QVector3D CSphere_Speculate (0.8, 0.85, 0.8);
        QVector3D CSphere_Diffuse (0.6, 0.6, 0.6);

        QVector3D CLeft_Wall (0.905, 0.145, 0.07);
        QVector3D CRight_Wall (0.156, 0.447, 0.2);
        QVector3D CFront_Wall (0.98, 0.95, 0.8);
        QVector3D CBack_Wall (0.1, 0.1, 0.1);

        QVector3D CTop_Floor (0.98, 0.95, 0.8);
        QVector3D CDown_Floor (0.98, 0.95, 0.8);

        QVector3D Gray (0.5, 0.5, 0.5);
        QVector3D Black (0, 0, 0);
        QVector3D White(0.8, 0.8, 0.85);

        QVector3D light_position (0,2.3,1.5);
        Light scene_light (light_position, White);
        std::vector<Light*> light_sources;
        light_sources.push_back(dynamic_cast<Light*>(&scene_light));

        QVector3D Sphere_Location_Speculate (-0.6, 0.6, 2);
        QVector3D Sphere_Location_Diffuse   (0.7, 0.5, 1.6);
        Sphere Scene_Sphere_S (Sphere_Location_Speculate, 0.6, CSphere_Speculate,0.01,0.0,0.9);
        Sphere Scene_Sphere_D (Sphere_Location_Diffuse, 0.5, CSphere_Diffuse,0.7,0.1,0.01);

        QVector3D normal_plane_DF (0,1,0);
        Plane Down_Floor(normal_plane_DF,0,CDown_Floor,0.5,0.5,0.07);
        QVector3D normal_plane_TF (0,-1,0);
        Plane Top_Floor (normal_plane_TF,2.5,CTop_Floor,0.5,0.5,0.01);

        QVector3D normal_plane_BW (0,0,1);
        Plane Back_Wall (normal_plane_BW,0.5,CBack_Wall,1,1,0.02);
        QVector3D normal_plane_FW (0,0,-1);
        Plane Front_Wall (normal_plane_FW, 2.6, CFront_Wall,1,0.1,0.03);

        QVector3D normal_plane_LW (1,0,0);
        Plane Left_Wall (normal_plane_LW,1.5,CLeft_Wall,1,1,0.01);
        QVector3D normal_plane_RW (-1,0,0);
        Plane Right_Wall (normal_plane_RW,1.5,CRight_Wall,1,0.9,0.02);

        Scene.push_back(dynamic_cast<Object*>(&Scene_Sphere_S));
        Scene.push_back(dynamic_cast<Object*>(&Scene_Sphere_D));
        Scene.push_back(dynamic_cast<Object*>(&Down_Floor));
        Scene.push_back(dynamic_cast<Object*>(&Top_Floor));
        Scene.push_back(dynamic_cast<Object*>(&Right_Wall));
        Scene.push_back(dynamic_cast<Object*>(&Left_Wall));
        Scene.push_back(dynamic_cast<Object*>(&Front_Wall));
        Scene.push_back(dynamic_cast<Object*>(&Back_Wall));

        Camera CamRayTraicing(CamPos,CamDir,CamRight,CamDown);
        fprintf(stderr, "-------- Renderizando -------\n");

        float aspectratio = width / float(height);
        float invHeight = 1 / float(height);
        float invWidth =  1 / float(width);
        float fov = 100;
        float angle;
        angle = tan(M_PI * 0.5 * fov / 200.0);

        QVector3D eye_dir;
        QVector3D RayOri;

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                float x = (2.0 * ((i + 0.5) * invWidth) - 1.0) * angle * aspectratio;
                float y = (1.0 - 2.0 * ((j + 0.5) * invHeight)) * angle;

                eye_dir=QVector3D(x,y,1);
                eye_dir.normalize();
                RayOri=CamRayTraicing.getCameraPosition();

                QVector3D Color;
                Color=Tracing(Scene,light_sources,RayOri,eye_dir,depth);

                CP_rgb=qRgb(Color.x()*255,Color.y()*255,Color.z()*255);
                colorPoint.setBrush((QBrush)CP_rgb);
                painter.setPen(colorPoint);
                painter.drawPoint(i+20,j+40);
            }
        }

    }
    int idxObject(std::vector<double> intersections)
    {
        int minimum_value;
        if (intersections.size() == 0) {
            return -1;
        }
        else if (intersections.size() == 1) {
            if (intersections.at(0) > 0) {
                return 0;
            }
            else {
                return -1;
            }
        }
        else {
            double max = 0;
            for (int i = 0; i <(int)intersections.size(); i++) {
                if (max < intersections.at(i)) {
                    max = intersections.at(i);
                }
            }
            if (max >0) {
                for (int index = 0; index < (int)intersections.size(); index++) {
                    if (intersections.at(index) > 0 && intersections.at(index) <= max) {
                        max = intersections.at(index);
                        minimum_value = index;
                    }
                }
                return minimum_value;
            }
            else {
                return -1;
            }
        }
    }
