#include "obj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <fstream>
#include <string>
#include <stdlib.h>


using namespace std;

ModeloOBJ::ModeloOBJ(){
    ModeloRGB_A =QVector3D(0.3,0.4,0.3);
    ModeloRGB_D =QVector3D(0.2,0.2,0.1);
    ModeloRGB_E =QVector3D(0.2,0.2,0.2);
}

void ModeloOBJ::cargarModelo(const char *path)
{
    //ifstream file("/home/noryab/QT/Proyecto_1/P_V1/bunny.obj");
    //ifstream file("/home/noryab/QT/Proyecto_1/P_V1/tyra.obj");
    string str;
    int vNumber = 0, vnNumber = 0, fNumber = 0;

    double X,Y,Z;
    vector<string> v;

    while (getline(file, str)){
        if(str.length()==0){            //Línea en blanco
            continue;
        }else if(str[0] == 'v' && str[1] == 'n' ){//Vertices normalizados
            split(str,v,' ');
            X = atof(v[1].c_str());
            Y = atof(v[2].c_str());
            Z = atof(v[3].c_str());
            modeloNormalesVW.push_back(QVector3D(X,Y,Z));
            vnNumber++;
        }else if( str[0] == 'v'){//vertices
            split(str,v,' ');
            X = atof(v[1].c_str());
            Y = atof(v[2].c_str());
            Z = atof(v[3].c_str());
            modeloVerticesW.push_back(QVector3D(X,Y,Z));
            vNumber++;
        }else if(str[0] == 'f'){
            split(str,v,' ');
            X = atof(v[1].c_str());
            Y = atof(v[2].c_str());
            Z = atof(v[3].c_str());
            modeloCaras.push_back(QVector3D(X,Y,Z));
            fNumber++;
        }
    }
    NumCaras = fNumber;
    NumVertices = vNumber;
    /// Se calculan las normales para las caras
    //NormalesCarasW();
}

int ModeloOBJ::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    int pos = txt.find( ch );
        int initialPos = 0;
        strs.clear();

        //Decompose statement
        while( (unsigned)pos != (unsigned)std::string::npos ) {
            strs.push_back( txt.substr( initialPos, pos - initialPos ) );
            initialPos = pos + 1;

            pos = txt.find( ch, initialPos );
        }
        //Add the last one
    unsigned int min = pos;
    if(txt.size() < min){ min = txt.size(); }
        strs.push_back( txt.substr( initialPos, min - initialPos ) );

        return strs.size();
}

void ModeloOBJ::RotateX(float _Theta)
{
    /// Rot X
    float sinTheta = sin((Theta-_Theta)*PI/180);
    float cosTheta = cos((Theta-_Theta)*PI/180);
    Theta=_Theta;
    for (int n=0; n<NumVertices; n++) {
        QVector3D node = modeloVerticesW[n];
        float y = node.y();
        float z = node.z();
        modeloVerticesW[n].setY(y*cosTheta - z*sinTheta);
        modeloVerticesW[n].setZ(z*cosTheta + y*sinTheta);
    }
        /// Se calculan las normales para las caras
        //NormalesCarasW();
}

void ModeloOBJ::RotateY(float _Phi)
{
    /// Rot Y
    float sinPhi = sin((Phi-_Phi)*PI/180);
    float cosPhi = cos((Phi-_Phi)*PI/180);
    Phi=_Phi;
    for (int n=0; n<NumVertices; n++) {
        QVector3D node = modeloVerticesW[n];
        float x = node.x();
        float z = node.z();
        modeloVerticesW[n].setX(x*cosPhi - z*sinPhi);
        modeloVerticesW[n].setZ(z*cosPhi + x*sinPhi);
    }
        /// Se calculan las normales para las caras
        //NormalesCarasW();
}

void ModeloOBJ::RotateZ(float _Alpha)
{
    /// Rot Z
    double sinAlpha = sin((Alpha-_Alpha)*PI/180);
    double cosAlpha = cos((Alpha-_Alpha)*PI/180);
    Alpha=_Alpha;

    for (int n=0; n<NumVertices; n++) {
        QVector3D node = modeloVerticesW[n];
        double x = node.x();
        double y = node.y();
        modeloVerticesW[n].setX(x*cosAlpha - y*sinAlpha);
        modeloVerticesW[n].setY(y*cosAlpha + x*sinAlpha);
    }
    /// Se calculan las normales para las caras
    //NormalesCarasW();
}

void ModeloOBJ::NormalesCarasC()
{
    modeloNormalesFC.clear();
    for(int f=0;f<NumCaras;f++){
        QVector3D V,W;
        QVector3D Q,P;
        QVector3D NC;
        P.setX(modeloVerticesC[modeloCaras[f].x()-1].x());
        P.setY(modeloVerticesC[modeloCaras[f].x()-1].y());
        P.setZ(modeloVerticesC[modeloCaras[f].x()-1].z());
        Q.setX(modeloVerticesC[modeloCaras[f].y()-1].x());
        Q.setY(modeloVerticesC[modeloCaras[f].y()-1].y());
        Q.setZ(modeloVerticesC[modeloCaras[f].y()-1].z());
        V=Q-P;

        P.setX(modeloVerticesC[modeloCaras[f].x()-1].x());
        P.setY(modeloVerticesC[modeloCaras[f].x()-1].y());
        P.setZ(modeloVerticesC[modeloCaras[f].x()-1].z());
        Q.setX(modeloVerticesC[modeloCaras[f].z()-1].x());
        Q.setY(modeloVerticesC[modeloCaras[f].z()-1].y());
        Q.setZ(modeloVerticesC[modeloCaras[f].z()-1].z());
        W=Q-P;
        NC=QVector3D::crossProduct(W,V);
        NC.normalize();
        modeloNormalesFC.push_back(NC);
    }
}

void ModeloOBJ::NormalesCarasW()
{
    modeloNormalesFW.clear();
    for(int f=0;f<NumCaras;f++){
        QVector3D V,W;
        QVector3D Q,P;
        QVector3D NC;
        P.setX(modeloVerticesW[modeloCaras[f].x()-1].x());
        P.setY(modeloVerticesW[modeloCaras[f].x()-1].y());
        P.setZ(modeloVerticesW[modeloCaras[f].x()-1].z());
        Q.setX(modeloVerticesW[modeloCaras[f].y()-1].x());
        Q.setY(modeloVerticesW[modeloCaras[f].y()-1].y());
        Q.setZ(modeloVerticesW[modeloCaras[f].y()-1].z());
        V=P-Q;

        P.setX(modeloVerticesW[modeloCaras[f].x()-1].x());
        P.setY(modeloVerticesW[modeloCaras[f].x()-1].y());
        P.setZ(modeloVerticesW[modeloCaras[f].x()-1].z());
        Q.setX(modeloVerticesW[modeloCaras[f].z()-1].x());
        Q.setY(modeloVerticesW[modeloCaras[f].z()-1].y());
        Q.setZ(modeloVerticesW[modeloCaras[f].z()-1].z());
        W=P-Q;
        NC=QVector3D::crossProduct(W,V);
        NC.normalize();
        modeloNormalesFW.push_back(NC);
    }
}

