#ifndef OBJ_H
#define OBJ_H

#include <QVector2D>
#include <QVector3D>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>

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

        vector<QVector3D> modeloVertxW;
        vector<QVector3D> modeloVertxC;
        vector<QVector2D> modeloVertxP;

        vector<QVector3D> modeloCaras;
        vector<QVector3D> modeloNormalesVW; /// Normales por vertice en el mundo
        vector<QVector3D> modeloNormalesVC; /// Normales por vertice en la camara
        vector<QVector3D> modeloNormalesFW; /// Normales por Cara en el mundo
        vector<QVector3D> modeloNormalesFC; /// Normales por Cara en la camara

    private:

       long  int NumCaras,NumVertx;
        float Theta;
        float Phi;
        float Alpha;

    public:
        int getNC(){ return NumCaras; }
        int getNV(){ return NumVertx; }



};
#endif // OBJ_H
