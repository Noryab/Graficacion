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

ModeloOBJ::ModeloOBJ()
{
}

void ModeloOBJ::cargarModelo(const char *path)
{
    ifstream file(path);

    string str;
    long int vNum = 0;
    long int vnNum = 0;
    long int fNum = 0;

    double X,Y,Z;
    vector<string> v;

    while (getline(file, str)){
        if(str.length()==0){
            continue;
        }else if(str[0] == 'v' && str[1] == 'n' ){//Vertx normalizados
            split(str,v,' ');
            X = atof(v[1].c_str());
            Y = atof(v[2].c_str());
            Z = atof(v[3].c_str());
            //cout<<"VNX:"<<X<<" VNY:"<<Y<<" VNZ:"<<Z<<endl;
            modeloNormalesVW.push_back(QVector3D(X,Y,Z));
            vnNum++;
        }else if( str[0] == 'v' && str[1] != 't'){//Vertx
            split(str,v,' ');
            X = atof(v[1].c_str());
            Y = atof(v[2].c_str());
            Z = atof(v[3].c_str());
            //cout<<"VX:"<<X<<" VY:"<<Y<<" VZ:"<<Z<<endl;
            modeloVertxW.push_back(QVector3D(X,Y,Z));
            vNum++;
        }else if(str[0] == 'f'){
            split(str,v,' ');
            X = atof(v[1].c_str());
            Y = atof(v[2].c_str());
            Z = atof(v[3].c_str());
            //cout<<"F1:"<<X<<" F2:"<<Y<<" F3:"<<Z<<endl;
            modeloCaras.push_back(QVector3D(X,Y,Z));
            fNum++;
        }
    }
    NumCaras = fNum;
    NumVertx = vNum;
}

int ModeloOBJ::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    int pos = txt.find( ch );
        int initialPos = 0;
        strs.clear();        
        while( (unsigned)pos != (unsigned)std::string::npos ) {
            strs.push_back( txt.substr( initialPos, pos - initialPos ) );
            initialPos = pos + 1;

            pos = txt.find( ch, initialPos );
        }
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
    for (int n=0; n<NumVertx; n++) {
        QVector3D node = modeloVertxW[n];
        float y = node.y();
        float z = node.z();
        modeloVertxW[n].setY(y*cosTheta - z*sinTheta);
        modeloVertxW[n].setZ(z*cosTheta + y*sinTheta);

    }
}

void ModeloOBJ::RotateY(float _Phi)
{
    /// Rot Y
    float sinPhi = sin((Phi-_Phi)*PI/180);
    float cosPhi = cos((Phi-_Phi)*PI/180);
    Phi=_Phi;
    for (int n=0; n<NumVertx; n++) {
        QVector3D node = modeloVertxW[n];
        float x = node.x();
        float z = node.z();
        modeloVertxW[n].setX(x*cosPhi - z*sinPhi);
        modeloVertxW[n].setZ(z*cosPhi + x*sinPhi);
    }
}

void ModeloOBJ::RotateZ(float _Alpha)
{
    /// Rot Z
    double sinAlpha = sin((Alpha-_Alpha)*PI/180);
    double cosAlpha = cos((Alpha-_Alpha)*PI/180);
    Alpha=_Alpha;

    for (int n=0; n<NumVertx; n++) {
        QVector3D node = modeloVertxW[n];
        double x = node.x();
        double y = node.y();
        modeloVertxW[n].setX(x*cosAlpha - y*sinAlpha);
        modeloVertxW[n].setY(y*cosAlpha + x*sinAlpha);
    }
}

