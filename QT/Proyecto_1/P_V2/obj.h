#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include <QtGui>
#include <QtCore>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <vector>
#include <Eigen/Dense>

#include <Eigen/Geometry>

using namespace Eigen;

using namespace std;

#define PI 3.1416

class ModeloOBJ{

    public:
        ModeloOBJ();

        void cargarModelo(const char *path);
        int split(const std::string &txt, std::vector<std::string> &strs, char ch);

        void RotateX(float _Theta);
        void RotateY(float _Phi);
        void RotateZ(float _Alpha);
        void NormalesCarasW();
        void NormalesCarasC();

        vector<QVector3D> modeloVerticesW;
        vector<QVector3D> modeloVerticesC;
        vector<QVector2D> modeloVerticesP;

        vector<QVector3D> modeloCaras;
        vector<QVector3D> modeloNormalesVW; /// Normales por vertice en el mundo
        vector<QVector3D> modeloNormalesVC; /// Normales por vertice en la camara
        vector<QVector3D> modeloNormalesFW; /// Normales por Cara en el mundo
        vector<QVector3D> modeloNormalesFC; /// Normales por Cara en la camara
        QVector3D ModeloRGB_A;
        QVector3D ModeloRGB_D;
        QVector3D ModeloRGB_E;

    private:

        int NumCaras,NumVertices;
        int r,g,b; 	//Componentes de color
        double ka; 	//Coeficiente de reflexión ambiental
        double kd; 	//Coeficiente de reflexión difusa
        double ks; 	//Coeficiente de reflexión especular
        float Theta;
        float Phi;
        float Alpha;

    public:
        int getNC(){ return NumCaras; }
        int getNV(){ return NumVertices; }



};
#endif // OBJ_H
