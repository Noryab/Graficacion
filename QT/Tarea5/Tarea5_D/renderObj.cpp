#define GLEW_STATIC
#include "GL/glew.h"
#include <GL/glut.h>
#include <QDebug>
#include <QImage>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "myrender.h"
#include "modelo3d.h"
#include "cara.h"
#include "vertice.h"

using namespace std;

/*Para uso de shaders*/
static GLuint program;

string readFile(const char *filePath) {

    string content;
    ifstream fileStream(filePath, ios::in);

    if(!fileStream.is_open()) {
        cout << "Could not read file " << filePath << ". File does not exist." << endl;
        return "";
    }

    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

/*
Método para cargar un SHADER
TOMADO DE : http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
*/
GLuint LoadShader(const char *vertex_path, const char *fragment_path)
{
    /*HAY QUE INICIALIZAR GLEW ANTES DE USAR LAS FUNCIONES POSTERIORES (¬¬) ... !!*/
    GLenum err = glewInit();
    cout << "Status of glewInit: " << err << endl;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    string vertShaderStr = readFile(vertex_path);
    string fragShaderStr = readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader
    cout << "Compiling vertex shader ..." << endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    cout << &vertShaderError[0] << endl;

    // Compile fragment shader
    cout << "Compiling fragment shader ..." << endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    cout << &fragShaderError[0] << endl;

    cout << "Linking program :D ! " << endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    cout << &programError[0] << endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}


//Constructor
renderObj::renderObj(QWidget *parent):QGLWidget(parent)
{}

void renderObj::initializeGL()
{
    sphereModel.loadModelVertices();

    program = LoadShader("/home/noryab/Graficacion/QT/Tarea5/Tarea5_D/shaders/simple.vert", "/home/noryab/Graficacion/QT/Tarea5/Tarea5_D/shaders/simple.frag");

    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);
    GLfloat light_Pos0[4] = {2.0,5.0,-5.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_Pos0);
    GLfloat light_ambient0[4]  = {0.1,0.1,0.1,1.0};
    GLfloat light_diffuse0[4]  = {0.1,0.7,0.7,1.0};
    GLfloat light_specular0[4] = {0.5,0.5,0.5,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse0);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular0);

    glEnable(GL_LIGHT1);
    GLfloat light_Pos1[4] = {-2.0,5.0,5.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,light_Pos1);
    GLfloat light_ambient1[4]  = {0.1,0.1,0.1,1.0};
    GLfloat light_diffuse1[4]  = {0.5,0.5,0.3,1.0};
    GLfloat light_specular1[4] = {1,1,1,1.0};
    glLightfv(GL_LIGHT1,GL_AMBIENT,light_ambient1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse1);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light_specular1);

    GLfloat attenuation[3] = {2.0f,2.2f,2.2f};
    glLightfv(GL_LIGHT0,GL_CONSTANT_ATTENUATION,attenuation);
    glLightfv(GL_LIGHT1,GL_CONSTANT_ATTENUATION,attenuation);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glUseProgram(program);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble fovy=50;
    GLdouble aspect=(float)width()/height();
    GLdouble zNear=1;
    GLdouble zFar=20;
    gluPerspective(fovy,aspect,zNear,zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,2.0,8.0, 0.0,1.0,0.0,0,1,0);
}

void renderObj::paintGL()
{
    const vector<Cara>& caras = sphereModel.getBunnyFaces();
    long numberOfFacesVerticesCoords = (caras.size()-1) * 9;

    //Arreglos de vertices y normales a pintar
    GLfloat vertices[numberOfFacesVerticesCoords];
    GLfloat normals[numberOfFacesVerticesCoords];

    int verticeNumberControl = 0;

    //De todas las caras...
    for(int faceNumber=0 ; faceNumber < (int)caras.size()-1 ; faceNumber++)
    {
        Cara c = caras[faceNumber];
        vector<Vertice> faceVertices = c.getVertices();

        // ... procesamos sus vértices
        for(int vertexNumber=0 ; vertexNumber<(int)faceVertices.size(); vertexNumber++)
        {
            Vertice vProc = faceVertices[vertexNumber];

            //Fabricamos el arreglo de vértices a pintar
            vertices[verticeNumberControl]   = vProc.getX();
            vertices[verticeNumberControl+1] = vProc.getY();
            vertices[verticeNumberControl+2] = vProc.getZ();

            //Fabricamos el arreglo de normales
            normals[verticeNumberControl]   = vProc.getNX();
            normals[verticeNumberControl+1] = vProc.getNY();
            normals[verticeNumberControl+2] = vProc.getNZ();

            verticeNumberControl += 3;
        }
    }

    GLfloat EMISS[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat materialAmbientColor[] = {0.55f, 0.6f, 0.48f, 1.0f};
    GLfloat materialDiffuseProperty[] = {0.6f, 0.6f, 0.5f, 1.0f};
    GLfloat materialSpecularProperty[] = {0.5f, 0.5f, 0.5f, 1.0f};
    float shininessLevel = 16.0f;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_CULL_FACE);
    glVertexPointer(3,GL_FLOAT,0,vertices);
    glNormalPointer(GL_FLOAT,0,normals);
        glMaterialfv( GL_FRONT,  GL_AMBIENT, materialAmbientColor);
        glMaterialfv( GL_FRONT,  GL_DIFFUSE, materialDiffuseProperty);
        glMaterialfv( GL_FRONT,  GL_SPECULAR, materialSpecularProperty);
        glMaterialf( GL_FRONT,  GL_SHININESS, shininessLevel);
        glMaterialfv( GL_FRONT,  GL_EMISSION, EMISS);
        glDrawArrays(GL_TRIANGLES,0,((int)caras.size()*3));
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void renderObj::resizeGL(int w,int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}
