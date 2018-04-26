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

void renderObj::OpenTexture()
{
    imag[0].load("/home/noryab/Graficacion/QT/Tarea4/Tarea4_Graficacion/texturaConejoG.jpg");
    imag[1].load("/home/noryab/Graficacion/QT/Tarea4/Tarea4_Graficacion/Conejo.jpg");
    imag[2].load("/home/noryab/Graficacion/QT/Tarea4/Tarea4_Graficacion/textura2.jpg");
    imag[3].load("/home/noryab/Graficacion/QT/Tarea4/Tarea4_Graficacion/texturaConejo.jpg");
    imag[4].load("/home/noryab/Graficacion/QT/Tarea4/Tarea4_Graficacion/Piel.jpg");
    imag[5].load("/home/noryab/Graficacion/QT/Tarea4/Tarea4_Graficacion/textura.jpg");

    for(int i=0;i<6;i++)
    {
        QImage imageTexture = QGLWidget::convertToGLFormat(imag[i]);
        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, imageTexture.width(), imageTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageTexture.bits());

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

}

void renderObj::resizeGL(int widht,int height){

    glViewport(0, 0,widht, height);
}

void renderObj::initializeGL(){
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);
    GLfloat light_position0[] = {5.0,5.0,-5.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
    GLfloat light_ambient_color0[]  = {0.1,0.1,0.1,1.0};
    GLfloat light_diffuse_color0[]  = {0.1,0.7,0.7,1.0};
    GLfloat light_specular_color0[] = {0.5,0.5,0.5,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient_color0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse_color0);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular_color0);

    glEnable(GL_LIGHT1);
    GLfloat light_position1[] = {-5.0,5.0,5.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
    GLfloat light_ambient_color1[]  = {0.1,0.1,0.1,1.0};
    GLfloat light_diffuse_color1[]  = {0.5,0.5,0.3,1.0};
    GLfloat light_specular_color1[] = {1,1,1,1.0};
    glLightfv(GL_LIGHT1,GL_AMBIENT,light_ambient_color1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse_color1);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light_specular_color1);

    GLfloat at[] = {2.0f,2.2f,2.2f};
    glLightfv(GL_LIGHT0,GL_CONSTANT_ATTENUATION,at);    
    glLightfv(GL_LIGHT1,GL_CONSTANT_ATTENUATION,at);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    fprintf(stderr, "-------- Cargando Modelo -------\n");
    M_Obj.cargarModelo("/usr/bunny.obj");

    this->Gouraud=true;
    this->Phong=false;
    this->Poligon=false;
    this->Mall=false;
    this->Punt=false;
    this->Texture=false;
    this->RecalMatri=true;
    this->numTexture=0;
}

void renderObj::paintGL()
{    
    RenderObj();
}


void renderObj::RenderObj()
{        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80,(float)width()/height(),1.5,40);
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

    GLfloat vObjectCompColor[] = {0.5,0.5,0.5,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, vObjectCompColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, vObjectCompColor);

    /// TEXTURA
    if(Texture==true)
    {        
         glShadeModel(GL_SMOOTH);

         glEnable(GL_TEXTURE_2D);
         OpenTexture();
         glBindTexture(GL_TEXTURE_2D, texture[numTexture]);


         glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


         glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
         glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
         glEnable(GL_TEXTURE_GEN_S);
         glEnable(GL_TEXTURE_GEN_T);

                long sizeM = (M_Obj.getNC()-1) * 9;
            GLfloat Vertx[sizeM];
            GLfloat Norms[sizeM];

            int vnum = 0;

            vector<QVector3D> V1;
            vector<QVector3D> V2;
            vector<QVector3D> V3;
            vector<QVector3D> N1;
            vector<QVector3D> N2;
            vector<QVector3D> N3;
            for(int f=0 ; f < M_Obj.getNC() ; f++){
                V1.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].x()-1]);
                V2.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].y()-1]);
                V3.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].z()-1]);

                N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
                N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
                N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

                QVector3D V;
                    V=V1[0];
                    V1.clear();
                    Vertx[vnum]   = V.x();
                    Vertx[vnum+1] = V.y();
                    Vertx[vnum+2] = V.z();

                    V=V2[0];
                    V2.clear();
                    Vertx[vnum+3] = V.x();
                    Vertx[vnum+4] = V.y();
                    Vertx[vnum+5] = V.z();

                    V=V3[0];
                    V3.clear();
                    Vertx[vnum+6] = V.x();
                    Vertx[vnum+7] = V.y();
                    Vertx[vnum+8] = V.z();

                 QVector3D N;
                    N=N1[0];
                    N1.clear();
                    Norms[vnum]   = N.x();
                    Norms[vnum+1] = N.y();
                    Norms[vnum+2] = N.z();

                    N=N2[0];
                    N2.clear();
                    Norms[vnum+3] = N.x();
                    Norms[vnum+4] = N.y();
                    Norms[vnum+5] = N.z();

                    N=N3[0];
                    N3.clear();
                    Norms[vnum+6] =N.x();
                    Norms[vnum+7] =N.y();
                    Norms[vnum+8] =N.z();

                    vnum += 9;
                }

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3,GL_FLOAT,0,Vertx);
            glNormalPointer(GL_FLOAT,0,Norms);

            glDrawArrays(GL_TRIANGLES,0,(M_Obj.getNC()*3));
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisable(GL_TEXTURE_2D);

    }
    /// RELLENO DE SUPERFICIE Poligonos
    if(Poligon==true)
    {

        glShadeModel(GL_FLAT);

               long sizeM = (M_Obj.getNC()-1) * 9;

           GLfloat Vertx[sizeM];
           GLfloat Norms[sizeM];

           int vnum = 0;

           vector<QVector3D> V1;
           vector<QVector3D> V2;
           vector<QVector3D> V3;
           vector<QVector3D> N1;
           vector<QVector3D> N2;
           vector<QVector3D> N3;

           for(int f=0 ; f < M_Obj.getNC() ; f++){
               V1.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].x()-1]);
               V2.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].y()-1]);
               V3.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].z()-1]);

               N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
               N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
               N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

               QVector3D V;
                   V=V1[0];
                   V1.clear();
                   Vertx[vnum]   = V.x();
                   Vertx[vnum+1] = V.y();
                   Vertx[vnum+2] = V.z();

                   V=V2[0];
                   V2.clear();
                   Vertx[vnum+3] = V.x();
                   Vertx[vnum+4] = V.y();
                   Vertx[vnum+5] = V.z();

                   V=V3[0];
                   V3.clear();
                   Vertx[vnum+6] = V.x();
                   Vertx[vnum+7] = V.y();
                   Vertx[vnum+8] = V.z();

                QVector3D N;
                   N=N1[0];
                   N1.clear();
                   Norms[vnum]   = N.x();
                   Norms[vnum+1] = N.y();
                   Norms[vnum+2] = N.z();

                   N=N2[0];
                   N2.clear();
                   Norms[vnum+3] = N.x();
                   Norms[vnum+4] = N.y();
                   Norms[vnum+5] = N.z();

                   N=N3[0];
                   N3.clear();
                   Norms[vnum+6] =N.x();
                   Norms[vnum+7] =N.y();
                   Norms[vnum+8] =N.z();

                   vnum += 9;
               }

           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);

           glVertexPointer(3,GL_FLOAT,0,Vertx);
           glNormalPointer(GL_FLOAT,0,Norms);
           glDrawArrays(GL_TRIANGLES,0,M_Obj.getNC()*3);
           glDisableClientState(GL_VERTEX_ARRAY);
           glDisableClientState(GL_NORMAL_ARRAY);
    }    
    /// NUBE DE PUNTOS
    if(Punt==true)
    {        
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);


               long sizeM = (M_Obj.getNC()-1) * 9;

           GLfloat Vertx[sizeM];
           GLfloat Norms[sizeM];

           int vnum = 0;

           vector<QVector3D> V1;
           vector<QVector3D> V2;
           vector<QVector3D> V3;
           vector<QVector3D> N1;
           vector<QVector3D> N2;
           vector<QVector3D> N3;

           for(int f=0 ; f < M_Obj.getNC() ; f++){
               V1.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].x()-1]);
               V2.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].y()-1]);
               V3.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].z()-1]);

               N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
               N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
               N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

               QVector3D V;
                   V=V1[0];
                   V1.clear();
                   Vertx[vnum]   = V.x();
                   Vertx[vnum+1] = V.y();
                   Vertx[vnum+2] = V.z();


                   V=V2[0];
                   V2.clear();
                   Vertx[vnum+3] = V.x();
                   Vertx[vnum+4] = V.y();
                   Vertx[vnum+5] = V.z();

                   V=V3[0];
                   V3.clear();
                   Vertx[vnum+6] = V.x();
                   Vertx[vnum+7] = V.y();
                   Vertx[vnum+8] = V.z();

                QVector3D N;
                   N=N1[0];
                   N1.clear();
                   Norms[vnum]   = N.x();
                   Norms[vnum+1] = N.y();
                   Norms[vnum+2] = N.z();

                   N=N2[0];
                   N2.clear();
                   Norms[vnum+3] = N.x();
                   Norms[vnum+4] = N.y();
                   Norms[vnum+5] = N.z();

                   N=N3[0];
                   N3.clear();
                   Norms[vnum+6] =N.x();
                   Norms[vnum+7] =N.y();
                   Norms[vnum+8] =N.z();

                   vnum += 9;
               }

           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);

           glVertexPointer(3,GL_FLOAT,0,Vertx);
           glNormalPointer(GL_FLOAT,0,Norms);
           glDrawArrays(GL_POINTS,0,M_Obj.getNC()*3);
           glPolygonMode(GL_FRONT, GL_FILL);
           glPolygonMode(GL_BACK, GL_FILL);
           glDisableClientState(GL_VERTEX_ARRAY);
           glDisableClientState(GL_NORMAL_ARRAY);
    }
    /// MALLA
    if(Mall==true)
    {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        glShadeModel(GL_FLAT);

               long sizeM = (M_Obj.getNC()-1) * 9;

           GLfloat Vertx[sizeM];
           GLfloat Norms[sizeM];

           int vnum = 0;

           vector<QVector3D> V1;
           vector<QVector3D> V2;
           vector<QVector3D> V3;
           vector<QVector3D> N1;
           vector<QVector3D> N2;
           vector<QVector3D> N3;
           for(int f=0 ; f < M_Obj.getNC() ; f++){
               V1.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].x()-1]);
               V2.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].y()-1]);
               V3.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].z()-1]);

               N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
               N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
               N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

               QVector3D V;
                   V=V1[0];
                   V1.clear();
                   Vertx[vnum]   = V.x();
                   Vertx[vnum+1] = V.y();
                   Vertx[vnum+2] = V.z();

                   V=V2[0];
                   V2.clear();
                   Vertx[vnum+3] = V.x();
                   Vertx[vnum+4] = V.y();
                   Vertx[vnum+5] = V.z();

                   V=V3[0];
                   V3.clear();
                   Vertx[vnum+6] = V.x();
                   Vertx[vnum+7] = V.y();
                   Vertx[vnum+8] = V.z();

                QVector3D N;
                   N=N1[0];
                   N1.clear();
                   Norms[vnum]   = N.x();
                   Norms[vnum+1] = N.y();
                   Norms[vnum+2] = N.z();

                   N=N2[0];
                   N2.clear();
                   Norms[vnum+3] = N.x();
                   Norms[vnum+4] = N.y();
                   Norms[vnum+5] = N.z();

                   N=N3[0];
                   N3.clear();
                   Norms[vnum+6] =N.x();
                   Norms[vnum+7] =N.y();
                   Norms[vnum+8] =N.z();

                   vnum += 9;
               }

           glEnableClientState(GL_VERTEX_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);
           glVertexPointer(3,GL_FLOAT,0,Vertx);
           glNormalPointer(GL_FLOAT,0,Norms);

           glDrawArrays(GL_TRIANGLES,0,M_Obj.getNC()*3);
           glPolygonMode(GL_FRONT, GL_FILL);
           glPolygonMode(GL_BACK, GL_FILL);
           glDisableClientState(GL_VERTEX_ARRAY);
           glDisableClientState(GL_NORMAL_ARRAY);

    }
    /// RELLENO DE SUPERFICIE Gouraud
    if(Gouraud==true)
    {
         glShadeModel(GL_SMOOTH);

            long int sizeM = (M_Obj.getNC()-1) * 9;
            GLfloat Vertx[sizeM];
            GLfloat Norms[sizeM];

            long int vnum = 0;
            vector<QVector3D> V1;
            vector<QVector3D> V2;
            vector<QVector3D> V3;
            vector<QVector3D> N1;
            vector<QVector3D> N2;
            vector<QVector3D> N3;

            for(long int f=0 ; f < M_Obj.getNC() ; f++){
                V1.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].x()-1]);
                V2.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].y()-1]);
                V3.push_back(M_Obj.modeloVertxW[M_Obj.modeloCaras[f].z()-1]);

                N1.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].x()-1]);
                N2.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].y()-1]);
                N3.push_back(M_Obj.modeloNormalesVW[M_Obj.modeloCaras[f].z()-1]);

                QVector3D V;
                    V=V1[0];
                    V1.clear();
                    Vertx[vnum]   = V.x();
                    Vertx[vnum+1] = V.y();
                    Vertx[vnum+2] = V.z();
                    V=V2[0];
                    V2.clear();
                    Vertx[vnum+3] = V.x();
                    Vertx[vnum+4] = V.y();
                    Vertx[vnum+5] = V.z();

                    V=V3[0];
                    V3.clear();
                    Vertx[vnum+6] = V.x();
                    Vertx[vnum+7] = V.y();
                    Vertx[vnum+8] = V.z();

                 QVector3D N;
                    N=N1[0];
                    N1.clear();
                    Norms[vnum]   = N.x();
                    Norms[vnum+1] = N.y();
                    Norms[vnum+2] = N.z();

                    N=N2[0];
                    N2.clear();
                    Norms[vnum+3] = N.x();
                    Norms[vnum+4] = N.y();
                    Norms[vnum+5] = N.z();

                    N=N3[0];
                    N3.clear();
                    Norms[vnum+6] =N.x();
                    Norms[vnum+7] =N.y();
                    Norms[vnum+8] =N.z();

                    vnum += 9;
                }
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glVertexPointer(3,GL_FLOAT,0,Vertx);
            glNormalPointer(GL_FLOAT,0,Norms);
            glDrawArrays(GL_TRIANGLES,0,(M_Obj.getNC()*3));
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);

    }

    /// RELLENO DE SUPERFICIE Phong
    if(Phong==true)
    {

    }

    fprintf(stderr, "-------- Renderizado -------\n");

}



