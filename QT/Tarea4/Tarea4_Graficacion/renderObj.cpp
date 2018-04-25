#include "renderObj.h"

#include <QStandardItemModel>


//Constructor
renderObj::renderObj(QWidget *parent):QGLWidget(parent)
{
    camOrig.setX(0);
    camOrig.setY(0);
    camOrig.setZ(-1);

    qPoint.setX(0);
    qPoint.setY(0);
    qPoint.setZ(1);
    theta=0.5;
    Angle_X=0.0;
    Angle_Y=0.0;
    Angle_Z=0.0;
    BoolRot=false;

}
renderObj::~renderObj()
{

}

void renderObj::resizeGL(int w,int h){
    qDebug() << "Empezando resizeGL()...";
    glViewport(0, 0, (GLint)w, (GLint)h);        
}

void renderObj::initializeGL(){
    qDebug() << "Empezando initializeGL()...";

    //Establecemos el color de "fondo"
    glClearColor(0,0,0,0);

    //Buffer-Z
    glEnable(GL_DEPTH_TEST);

    //Activamos luz 0 (blanca), la tomaremos como "luz ambiental" ...
    glEnable(GL_LIGHT0);
    GLfloat light_position0[] = {5.0,5.0,-5.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
    GLfloat light_ambient_color0[]  = {0.1,0.1,0.0,1.0};
    GLfloat light_diffuse_color0[]  = {0.1,0.7,0.7,1.0};
    GLfloat light_specular_color0[] = {0.1,0.5,0.5,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient_color0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse_color0);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular_color0);


    //Activamos luz 0 (blanca), la tomaremos como "luz ambiental" ...
    glEnable(GL_LIGHT1);
    GLfloat light_position1[] = {-5.0,5.0,5.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
    /*Establecemos los colores de la luz (roja)*/
    GLfloat light_ambient_color1[]  = {0.1,0.1,0.0,1.0};
    GLfloat light_diffuse_color1[]  = {0.7,0.7,0.1,1.0};
    GLfloat light_specular_color1[] = {0.5,0.5,0.1,1.0};
    glLightfv(GL_LIGHT1,GL_AMBIENT,light_ambient_color1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse_color1);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light_specular_color1);


    // ... la atenuamos para que no se vea tan blanco el resplandor
    GLfloat at[] = {2.0f,0.2f,0.2f};
    glLightfv(GL_LIGHT0,GL_CONSTANT_ATTENUATION,at);    
    glLightfv(GL_LIGHT1,GL_CONSTANT_ATTENUATION,at);


    //Le decimos a GL que use cálculos de iluminación
    glEnable(GL_LIGHTING);

    //Habilitamos el color de los objetos
    glEnable(GL_COLOR_MATERIAL);

    //Modelo que vamos a trabajar
    //Cargamos los vértices y normales del modelo
    fprintf(stderr, "-------- Cargando Modelo -------\n");
    M_Obj.cargarModelo("/usr/bunny.obj");

    this->Gouraud=true;
    this->Phong=false;
    this->Poligon=false;
    this->Mall=false;
    this->Punt=false;
    this->Textura=false;
    this->RecalMatri=true;
}

void renderObj::paintGL()
{    
    qDebug() << "Empezando paintGL()...";
    RenderObj();
}


void renderObj::RenderObj()
{    
    qDebug() << "Empezando renderObjGL()...";
    //Cada que pintamos limpiamos la pantalla y el BufferZ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Seteamos los parámetros de la vista/cámara
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Vista 2D en perspectiva: 25, ángulo de enfoque,relaciónVentana,planos
    gluPerspective(80,(float)width()/height(),1.5,20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0,0.0,5.0,  0.0,1.0,0.0,  0,1,0);
    if(BoolRot)
    {
        glRotatef(Angle_X,1,0,0);
        glRotatef(Angle_Y,0,1,0);
        glRotatef(Angle_Z,0,0,1);
    }
    BoolRot=false;



    /*Establecemos el color del objeto de acuerdo a las componentes ambiental,difusa y especular (azul)*/
    GLfloat vObjectCompColor[] = {0.0,0.0,1.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, vObjectCompColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vObjectCompColor);

    /// TEXTURA
    if(Textura==true)
    {
         qDebug() << "Empezando Textura...";



    }
    /// RELLENO DE SUPERFICIE Poligonos
    if(Poligon==true)
    {

        qDebug() << "Empezando Poligon...";
        glShadeModel(GL_FLAT);

               long numberOfFacesVerticesCoords = (M_Obj.getNC()-1) * 9;

           //Arreglos de vertices y normales a pintar
           GLfloat vertices[numberOfFacesVerticesCoords];
           GLfloat normals[numberOfFacesVerticesCoords];

           int verticeNumberControl = 0;

           vector<QVector3D> V1;
           vector<QVector3D> V2;
           vector<QVector3D> V3;
           vector<QVector3D> N1;
           vector<QVector3D> N2;
           vector<QVector3D> N3;

           //De todas las caras...
           for(int f=0 ; f < M_Obj.getNC() ; f++){
               V1.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].x()-1]);
               V2.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].y()-1]);
               V3.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].z()-1]);

               N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
               N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
               N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

               QVector3D V;
                   //Fabricamos el arreglo de vértices a pintar
                   V=V1[0];
                   V1.clear();
                   vertices[verticeNumberControl]   = V.x();
                   vertices[verticeNumberControl+1] = V.y();
                   vertices[verticeNumberControl+2] = V.z();

                   //Fabricamos el arreglo de vértices a pintar
                   V=V2[0];
                   V2.clear();
                   vertices[verticeNumberControl+3] = V.x();
                   vertices[verticeNumberControl+4] = V.y();
                   vertices[verticeNumberControl+5] = V.z();

                   //Fabricamos el arreglo de vértices a pintar
                   V=V3[0];
                   V3.clear();
                   vertices[verticeNumberControl+6] = V.x();
                   vertices[verticeNumberControl+7] = V.y();
                   vertices[verticeNumberControl+8] = V.z();

                QVector3D N;
                   //Fabricamos el arreglo de vértices a pintar
                   N=N1[0];
                   N1.clear();
                   normals[verticeNumberControl]   = N.x();
                   normals[verticeNumberControl+1] = N.y();
                   normals[verticeNumberControl+2] = N.z();

                   N=N2[0];
                   N2.clear();
                   normals[verticeNumberControl+3] = N.x();
                   normals[verticeNumberControl+4] = N.y();
                   normals[verticeNumberControl+5] = N.z();

                   N=N3[0];
                   N3.clear();
                   normals[verticeNumberControl+6] =N.x();
                   normals[verticeNumberControl+7] =N.y();
                   normals[verticeNumberControl+8] =N.z();

                   verticeNumberControl += 9;
               }

           //Habilitamos arreglos de vértices y normales
           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);

           //Enlazamos los arreglos de vértices y normales
           glVertexPointer(3,GL_FLOAT,0,vertices);
           glNormalPointer(GL_FLOAT,0,normals);

           //Pintamos el modelo basado en triángulos (cada cara tiene 3 vértices)
           glDrawArrays(GL_TRIANGLES,0,M_Obj.getNC()*3);

           //Deshabilitamos los arreglos
           glDisableClientState(GL_VERTEX_ARRAY);
           glDisableClientState(GL_NORMAL_ARRAY);

           //Desactivamos las texturas
           glDisable(GL_TEXTURE_2D);

    }    
    /// NUBE DE PUNTOS
    if(Punt==true)
    {        
        qDebug() << "Empezando Punt...";
        qDebug() << "Empezando Poligon...";
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);


               long numberOfFacesVerticesCoords = (M_Obj.getNC()-1) * 9;

           //Arreglos de vertices y normales a pintar
           GLfloat vertices[numberOfFacesVerticesCoords];
           GLfloat normals[numberOfFacesVerticesCoords];

           int verticeNumberControl = 0;

           vector<QVector3D> V1;
           vector<QVector3D> V2;
           vector<QVector3D> V3;
           vector<QVector3D> N1;
           vector<QVector3D> N2;
           vector<QVector3D> N3;

           //De todas las caras...
           for(int f=0 ; f < M_Obj.getNC() ; f++){
               V1.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].x()-1]);
               V2.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].y()-1]);
               V3.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].z()-1]);

               N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
               N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
               N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

               QVector3D V;
                   //Fabricamos el arreglo de vértices a pintar
                   V=V1[0];
                   V1.clear();
                   vertices[verticeNumberControl]   = V.x();
                   vertices[verticeNumberControl+1] = V.y();
                   vertices[verticeNumberControl+2] = V.z();

                   //Fabricamos el arreglo de vértices a pintar
                   V=V2[0];
                   V2.clear();
                   vertices[verticeNumberControl+3] = V.x();
                   vertices[verticeNumberControl+4] = V.y();
                   vertices[verticeNumberControl+5] = V.z();

                   //Fabricamos el arreglo de vértices a pintar
                   V=V3[0];
                   V3.clear();
                   vertices[verticeNumberControl+6] = V.x();
                   vertices[verticeNumberControl+7] = V.y();
                   vertices[verticeNumberControl+8] = V.z();

                QVector3D N;
                   //Fabricamos el arreglo de vértices a pintar
                   N=N1[0];
                   N1.clear();
                   normals[verticeNumberControl]   = N.x();
                   normals[verticeNumberControl+1] = N.y();
                   normals[verticeNumberControl+2] = N.z();

                   N=N2[0];
                   N2.clear();
                   normals[verticeNumberControl+3] = N.x();
                   normals[verticeNumberControl+4] = N.y();
                   normals[verticeNumberControl+5] = N.z();

                   N=N3[0];
                   N3.clear();
                   normals[verticeNumberControl+6] =N.x();
                   normals[verticeNumberControl+7] =N.y();
                   normals[verticeNumberControl+8] =N.z();

                   verticeNumberControl += 9;
               }

           //Habilitamos arreglos de vértices y normales
           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);

           //Enlazamos los arreglos de vértices y normales
           glVertexPointer(3,GL_FLOAT,0,vertices);
           glNormalPointer(GL_FLOAT,0,normals);

           //Pintamos el modelo basado en triángulos (cada cara tiene 3 vértices)
           glDrawArrays(GL_POINTS,0,M_Obj.getNC()*3);
           glPolygonMode(GL_FRONT, GL_FILL);
           glPolygonMode(GL_BACK, GL_FILL);

           //Deshabilitamos los arreglos
           glDisableClientState(GL_VERTEX_ARRAY);
           glDisableClientState(GL_NORMAL_ARRAY);

           //Desactivamos las texturas
           glDisable(GL_TEXTURE_2D);
    }
    /// MALLA
    if(Mall==true)
    {
        qDebug() << "Empezando Malla...";
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        glShadeModel(GL_FLAT);

               long numberOfFacesVerticesCoords = (M_Obj.getNC()-1) * 9;

           //Arreglos de vertices y normales a pintar
           GLfloat vertices[numberOfFacesVerticesCoords];
           GLfloat normals[numberOfFacesVerticesCoords];

           int verticeNumberControl = 0;

           vector<QVector3D> V1;
           vector<QVector3D> V2;
           vector<QVector3D> V3;
           vector<QVector3D> N1;
           vector<QVector3D> N2;
           vector<QVector3D> N3;

           //De todas las caras...
           for(int f=0 ; f < M_Obj.getNC() ; f++){
               V1.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].x()-1]);
               V2.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].y()-1]);
               V3.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].z()-1]);

               N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
               N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
               N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

               QVector3D V;
                   //Fabricamos el arreglo de vértices a pintar
                   V=V1[0];
                   V1.clear();
                   vertices[verticeNumberControl]   = V.x();
                   vertices[verticeNumberControl+1] = V.y();
                   vertices[verticeNumberControl+2] = V.z();

                   //Fabricamos el arreglo de vértices a pintar
                   V=V2[0];
                   V2.clear();
                   vertices[verticeNumberControl+3] = V.x();
                   vertices[verticeNumberControl+4] = V.y();
                   vertices[verticeNumberControl+5] = V.z();

                   //Fabricamos el arreglo de vértices a pintar
                   V=V3[0];
                   V3.clear();
                   vertices[verticeNumberControl+6] = V.x();
                   vertices[verticeNumberControl+7] = V.y();
                   vertices[verticeNumberControl+8] = V.z();

                QVector3D N;
                   //Fabricamos el arreglo de vértices a pintar
                   N=N1[0];
                   N1.clear();
                   normals[verticeNumberControl]   = N.x();
                   normals[verticeNumberControl+1] = N.y();
                   normals[verticeNumberControl+2] = N.z();

                   N=N2[0];
                   N2.clear();
                   normals[verticeNumberControl+3] = N.x();
                   normals[verticeNumberControl+4] = N.y();
                   normals[verticeNumberControl+5] = N.z();

                   N=N3[0];
                   N3.clear();
                   normals[verticeNumberControl+6] =N.x();
                   normals[verticeNumberControl+7] =N.y();
                   normals[verticeNumberControl+8] =N.z();

                   verticeNumberControl += 9;
               }

           //Habilitamos arreglos de vértices y normales
           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);

           //Enlazamos los arreglos de vértices y normales
           glVertexPointer(3,GL_FLOAT,0,vertices);
           glNormalPointer(GL_FLOAT,0,normals);

           //Pintamos el modelo basado en triángulos (cada cara tiene 3 vértices)
           glDrawArrays(GL_TRIANGLES,0,M_Obj.getNC()*3);

           glPolygonMode(GL_FRONT, GL_FILL);
           glPolygonMode(GL_BACK, GL_FILL);

           //Deshabilitamos los arreglos
           glDisableClientState(GL_VERTEX_ARRAY);
           glDisableClientState(GL_NORMAL_ARRAY);

           //Desactivamos las texturas
           glDisable(GL_TEXTURE_2D);

    }
    /// RELLENO DE SUPERFICIE Gouraud
    if(Gouraud==true)
    {
        qDebug() << "Empezando Gouraud...";
         glShadeModel(GL_SMOOTH);

                long numberOfFacesVerticesCoords = (M_Obj.getNC()-1) * 9;

            //Arreglos de vertices y normales a pintar
            GLfloat vertices[numberOfFacesVerticesCoords];
            GLfloat normals[numberOfFacesVerticesCoords];

            int verticeNumberControl = 0;

            vector<QVector3D> V1;
            vector<QVector3D> V2;
            vector<QVector3D> V3;
            vector<QVector3D> N1;
            vector<QVector3D> N2;
            vector<QVector3D> N3;

            //De todas las caras...
            for(int f=0 ; f < M_Obj.getNC() ; f++){
                V1.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].x()-1]);
                V2.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].y()-1]);
                V3.push_back(M_Obj.modeloVerticesW[M_Obj.modeloCaras[f].z()-1]);

                N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
                N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
                N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

                QVector3D V;
                    //Fabricamos el arreglo de vértices a pintar
                    V=V1[0];
                    V1.clear();
                    vertices[verticeNumberControl]   = V.x();
                    vertices[verticeNumberControl+1] = V.y();
                    vertices[verticeNumberControl+2] = V.z();

                    //Fabricamos el arreglo de vértices a pintar
                    V=V2[0];
                    V2.clear();
                    vertices[verticeNumberControl+3] = V.x();
                    vertices[verticeNumberControl+4] = V.y();
                    vertices[verticeNumberControl+5] = V.z();

                    //Fabricamos el arreglo de vértices a pintar
                    V=V3[0];
                    V3.clear();
                    vertices[verticeNumberControl+6] = V.x();
                    vertices[verticeNumberControl+7] = V.y();
                    vertices[verticeNumberControl+8] = V.z();

                 QVector3D N;
                    //Fabricamos el arreglo de vértices a pintar
                    N=N1[0];
                    N1.clear();
                    normals[verticeNumberControl]   = N.x();
                    normals[verticeNumberControl+1] = N.y();
                    normals[verticeNumberControl+2] = N.z();

                    N=N2[0];
                    N2.clear();
                    normals[verticeNumberControl+3] = N.x();
                    normals[verticeNumberControl+4] = N.y();
                    normals[verticeNumberControl+5] = N.z();

                    N=N3[0];
                    N3.clear();
                    normals[verticeNumberControl+6] =N.x();
                    normals[verticeNumberControl+7] =N.y();
                    normals[verticeNumberControl+8] =N.z();

                    verticeNumberControl += 9;
                }

            //Habilitamos arreglos de vértices y normales
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);

            //Enlazamos los arreglos de vértices y normales
            glVertexPointer(3,GL_FLOAT,0,vertices);
            glNormalPointer(GL_FLOAT,0,normals);

            //Pintamos el modelo basado en triángulos (cada cara tiene 3 vértices)
            glDrawArrays(GL_TRIANGLES,0,((int)M_Obj.getNC()*3));

            //Deshabilitamos los arreglos
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);

            //Desactivamos las texturas
            glDisable(GL_TEXTURE_2D);

    }

    /// RELLENO DE SUPERFICIE Phong
    if(Phong==true)
    {
        qDebug() << "Empezando Phong...";
    }

    fprintf(stderr, "-------- Renderizado -------\n");

}


