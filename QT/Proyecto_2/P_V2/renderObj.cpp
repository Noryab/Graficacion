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

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

using namespace std;

struct RGBType {
    double r;
    double g;
    double b;
};

void savebmp (const char *filename, int w, int h, int dpi, RGBType *data) {
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bmpfileheader[ 2] = (unsigned char)(filesize);
    bmpfileheader[ 3] = (unsigned char)(filesize>>8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);

    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f = fopen(filename,"wb");

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for (int i = 0; i < k; i++) {
        RGBType rgb = data[i];

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

        fwrite(color,1,3,f);
    }

    fclose(f);
}


int winningObjectIndex(vector<double> object_intersections) {
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
        if (max > 0) {
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

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight) {

    Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
    Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

    Color final_color = winning_object_color.colorScalar(ambientlight);

    if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
        // reflection from objects with specular intensity
        double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
        Vect scalar1 = winning_object_normal.vectMult(dot1);
        Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
        Vect scalar2 = add1.vectMult(2);
        Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
        Vect reflection_direction = add2.normalize();

        Ray reflection_ray (intersection_position, reflection_direction);

        // determine what the ray intersects with first
        vector<double> reflection_intersections;

        for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
            reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
        }

        int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

        if (index_of_winning_object_with_reflection != -1) {
            // reflection ray missed everthing else
            if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
                // determine the position and direction at the point of intersection with the reflection ray
                // the ray only affects the color if it reflected off something

                Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
                Vect reflection_intersection_ray_direction = reflection_direction;

                Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);

                final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
            }
        }
    }

    for (int light_index = 0; light_index < light_sources.size(); light_index++) {
        Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

        float cosine_angle = winning_object_normal.dotProduct(light_direction);

        if (cosine_angle > 0) {
            // test for shadows
            bool shadowed = false;

            Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();

            Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

            vector<double> secondary_intersections;

            for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
                secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
            }

            for (int c = 0; c < secondary_intersections.size(); c++) {
                if (secondary_intersections.at(c) > accuracy) {
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                }
                break;
            }

            if (shadowed == false) {
                final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

                if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
                    // special [0-1]
                    double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
                    Vect scalar1 = winning_object_normal.vectMult(dot1);
                    Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
                    Vect scalar2 = add1.vectMult(2);
                    Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
                    Vect reflection_direction = add2.normalize();

                    double specular = reflection_direction.dotProduct(light_direction);
                    if (specular > 0) {
                        specular = pow(specular, 4);
                        final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
                    }
                }

            }

        }
    }

    return final_color.clip();
}



//Constructor
renderObj::renderObj()
{    

    QVector3D O2 (0,0,0);
    QVector3D X2 (1,0,0);
    QVector3D Y2 (0,1,0);
    QVector3D Z2 (0,0,1);

    QVector3D campos2 (-3, -2, -4);

    QVector3D look_at2 (0, -2, 0);
QVector3D diff_btw2 (campos2.x() - look_at2.x(), campos2.y() - look_at2.y(), campos2.z() - look_at2.z());
    QVector3D camdir2 = diff_btw2.normalized();
    QVector3D camright2 = QVector3D::crossProduct(Y2,camdir2);
              camright2.normalize();
    QVector3D camdown2 = QVector3D::crossProduct(camright2,camdir2);

    QColor white_light2 (0.8, 0.8, 0.8, 0);
    QColor pretty_green2 (0.5, 0.8, 0.5, 0);
    QColor maroon2 (0.5, 0.8, 0.5, 0);
    QColor tile_floor2 (0.5, 0.8, 0.5, 0);
    QColor gray2 (0.5, 0.8, 0.5, 0);
    QColor black2 (0.5, 0.8, 0.5, 0);

    dpi = 72;
    width = 640;
    height = 480;
    n = width*height;
    aadepth = 2;
    aathreshold = 0.1;
    aspectratio = (double)width/(double)height;
    ambientlight = 0.1;
    accuracy = 0.0001;

}

renderObj::~renderObj()
{

}

void renderObj::RenderObj(ModeloOBJ &M_Obj,QPainter &painter)
{        

    RGBType *pixels = new RGBType[n];


    Vect O (0,0,0);
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);

    Vect campos (0, -2, -5);


    Vect look_at (0, -2, 0);
    Vect diff_btw (campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());

    Vect camdir = diff_btw.negative().normalize();
    Vect camright = Y.crossProduct(camdir).normalize();
    Vect camdown = camright.crossProduct(camdir);
    Camera scene_cam (campos, camdir, camright, camdown);


    Color white_light (0.5, 0.5, 0.5, 0);
    Color pretty_green (0.9, 1.0, 0.9, 0.2);
    Color maroon (0.8, 0.65, 0.65, 0);
    Color tile_floor (0.98, 0.95, 0.89, 0);
    Color CT_wall (0.98, 0.95, 0.89, 0);
    Color CL_wall (0.98, 0.0, 0.0, 0);
    Color CR_wall (0.0, 0.95, 0.0, 0);
    Color gray (0.0, 0.0, 0.0, 0.5);
    Color black (0.0, 0.0, 0.0, 0);

    Vect light_position (0,-3,1.5);
    Light scene_light (light_position, white_light);
    vector<Source*> light_sources;
    light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    // scene objects
    QVector3D new_sphere_location2(1.75, -0.25, 0);
    Vect new_sphere_locationE (-1.95, 0, 3);
    Vect new_sphere_locationD (1.75, 0, 2);
    Sphere scene_sphere (new_sphere_locationE, 1, pretty_green);
    Sphere scene_sphere2 (new_sphere_locationD, 1, maroon);

    Vect normal_plane_B (0,0,1);
    Plane Back_plane (normal_plane_B,-5,black);
    Vect normal_plane_F (0,-1,0);
    Plane scene_plane (normal_plane_F, -1, tile_floor);
    Vect normal_plane_TW (0,1,0);
    Plane Top_wall (normal_plane_TW,-5.5,CT_wall);
    Vect normal_plane_BW (0,0,-1);
    Plane Back_wall (normal_plane_BW,-5,tile_floor);
    Vect normal_plane_LW (-1,0,0);
    Plane Left_wall (normal_plane_LW,-3.5,CL_wall);
    Vect normal_plane_RW (-1,0,0);
    Plane Right_wall (normal_plane_RW,3.5,CR_wall);


    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));


    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
    scene_objects.push_back(dynamic_cast<Object*>(&Back_plane));
    scene_objects.push_back(dynamic_cast<Object*>(&Top_wall));
    scene_objects.push_back(dynamic_cast<Object*>(&Back_wall));
    scene_objects.push_back(dynamic_cast<Object*>(&Left_wall));
    scene_objects.push_back(dynamic_cast<Object*>(&Right_wall));

    int thisone, aa_index;
    double xamnt, yamnt;
    double tempRed, tempGreen, tempBlue;


    fprintf(stderr, "-------- Renderizando -------\n");

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            thisone = y*width + x;

            // start with a blank pixel
            double tempRed[aadepth*aadepth];
            double tempGreen[aadepth*aadepth];
            double tempBlue[aadepth*aadepth];

            for (int aax = 0; aax < aadepth; aax++) {
                for (int aay = 0; aay < aadepth; aay++) {

                    aa_index = aay*aadepth + aax;

                    srand(time(0));

                    // create the ray from the camera to this pixel
                    if (aadepth == 1) {
                        // start with no anti-aliasing
                            // the image is wider than it is tall
                            xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
                            yamnt = ((height - y) + 0.5)/height;
                    }
                    else {
                        // anti-aliasing
                            // the image is wider than it is tall
                            xamnt = ((x + (double)aax/((double)aadepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
                            yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
                    }

                    Vect cam_ray_origin = scene_cam.getCameraPosition();
                    Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();

                    Ray cam_ray (cam_ray_origin, cam_ray_direction);

                    vector<double> intersections;

                    for (int index = 0; index < scene_objects.size(); index++) {
                        intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
                    }

                    int index_of_winning_object = winningObjectIndex(intersections);

                    if (index_of_winning_object == -1) {
                        // set the backgroung black
                        tempRed[aa_index] = 0;
                        tempGreen[aa_index] = 0;
                        tempBlue[aa_index] = 0;
                    }
                    else{
                        // index coresponds to an object in our scene
                     if (intersections.at(index_of_winning_object) > accuracy) {
                            // determine the position and direction vectors at the point of intersection

                            Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
                            Vect intersecting_ray_direction = cam_ray_direction;

                            Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);

                            tempRed[aa_index] = intersection_color.getColorRed();
                            tempGreen[aa_index] = intersection_color.getColorGreen();
                            tempBlue[aa_index] = intersection_color.getColorBlue();
                        }
                    }
                }
            }
            // average the pixel color
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;

            for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
                totalRed = totalRed + tempRed[iRed];
            }
            for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
                totalGreen = totalGreen + tempGreen[iGreen];
            }
            for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
                totalBlue = totalBlue + tempBlue[iBlue];
            }

            double avgRed = totalRed/(aadepth*aadepth);
            double avgGreen = totalGreen/(aadepth*aadepth);
            double avgBlue = totalBlue/(aadepth*aadepth);

            pixels[thisone].r = avgRed;
            pixels[thisone].g = avgGreen;
            pixels[thisone].b = avgBlue;

            QPen colorPoint;
            QRgb CP_rgb;

            CP_rgb=qRgb(avgRed*255,avgGreen*255,avgBlue*255);
            colorPoint.setBrush((QBrush)CP_rgb);

            painter.setPen(colorPoint);
            painter.drawPoint(x+20,y+40);
        }
    }

    savebmp("scene_anti-aliased.bmp",width,height,dpi,pixels);

    delete pixels, tempRed, tempGreen, tempBlue;

}

/*Método para fabricar la escena que se va a renderizar*/
void renderObj::Build()
{


}
