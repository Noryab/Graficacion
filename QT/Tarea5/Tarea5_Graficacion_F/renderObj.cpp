
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <iostream>
#include "renderObj.h"

static GLuint program;

bool CompileShader(string nvert,string ngeom, string nfrag,GLuint *prog)
{
 int compiledv = 0,
     compiledg = 0,
     compiledf = 0;
  bool f_vert = false,
       f_geom = false,
       f_frag = false;
  GLint length;
  GLchar *progSrc=0;

  std::ifstream inputFile;
  GLuint vertexShader = 0,
         geomShader = 0,
         fragmentShader = 0;


  GLenum err = glewInit();

  std::cout << "Shader Version:"<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  if(nvert.size() > 0)
      f_vert = true;
  if(ngeom.size() > 0){
     if(GL_EXT_geometry_shader4){
        f_geom = true;
       }
     else{
        std::cout<<"Geometry Shaders not Supported"<<std::endl;
       }
    }
  if(nfrag.size() > 0)
      f_frag = true;

  /*
   *
   * Reading and compiling the vertex shader
   *
   */

 if(f_vert == true){
    inputFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    try{
        inputFile.open(nvert.c_str(), std::ios::in | std::ios::binary);

       }catch(std::ifstream::failure e){
        std::cout << "An exception occurred while reading Vertex Shader. Exception Nr. " << e.what() << std::endl;
       }
    if(inputFile.fail() == true){
       std::cout<<"I/O Error in the Vertex Program"<<std::endl;
      }
    else{

       inputFile.seekg (0, std::ios_base::end);
       length = inputFile.tellg();
       if(length > 0){

          progSrc = new GLchar[length]();
          inputFile.seekg (0, std::ios_base::beg);
          inputFile.read(progSrc, length);
          inputFile.close();

          vertexShader = glCreateShader(GL_VERTEX_SHADER);
          std::cout << "Shader Version 3:"<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
          glShaderSource(vertexShader, 1, &progSrc,&length);

          std::cout<<"Vertex Program:"<<std::endl<<progSrc;
          delete[] progSrc;

          glCompileShader(vertexShader);
          // Checking if the compilation was a success
          glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&compiledv);
          if(compiledv == 0){
             int infologLength = 0;

             glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infologLength);
             if(infologLength > 0){
                int charsWritten  = 0;
                char *infoLog = new char[infologLength]();
                glGetShaderInfoLog(vertexShader, infologLength, &charsWritten, infoLog);
                std::cout<<"Vertex Shader Log Info: "<<infoLog<<std::endl;
                delete[]infoLog;
               }
            }
         }
      }
   }
 /*
  *
  * Reading and compiling the vertex shader
  *
  */
 std::cout << "Shader Version:"<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
if(f_geom == true){
   inputFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
   try{
       inputFile.open(ngeom.c_str(), std::ios::in | std::ios::binary);
      }catch(std::ifstream::failure e){
       std::cout << "An exception occurred while reading Geometry Shader. Exception Nr. " << e.what() << std::endl;
      }
   if(inputFile.fail() == true){
      std::cout<<"I/O Error in the Geometry Program"<<std::endl;
     }
   else{
      inputFile.seekg (0, std::ios_base::end);
      length = inputFile.tellg();
      if(length > 0){
         progSrc = new GLchar[length]();
         inputFile.seekg (0, std::ios_base::beg);
         inputFile.read(progSrc, length);
         inputFile.close();
         geomShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
         glShaderSource(geomShader, 1, &progSrc,NULL);
         delete[] progSrc;

         glCompileShader(geomShader);
         // Checking if the compilation was a success
         glGetShaderiv(geomShader,GL_COMPILE_STATUS,&compiledv);
         if(compiledv == 0){
            int infologLength = 0;

            glGetShaderiv(geomShader, GL_INFO_LOG_LENGTH, &infologLength);
            if(infologLength > 0){
               int charsWritten  = 0;
               char *infoLog = new char[infologLength]();
               glGetShaderInfoLog(geomShader, infologLength, &charsWritten, infoLog);
               std::cout<<"Log Info Geometry Shader: "<<infoLog<<std::endl;
               delete[]infoLog;
              }
           }
        }
     }
  }
 /*
  *
  * Reading and compiling the fragment shader
  *
  */
 if(f_frag == true){
    try{
        inputFile.open(nfrag.c_str(), std::ios::in | std::ios::binary);
       }catch(std::ifstream::failure e){
        std::cout << "An exception occurred while reading Fragment Shader. Exception Nr. " << e.what() << std::endl;
       }
    if(inputFile.fail() == true){
       std::cout<<"I/O Error in the Fragment Program"<<std::endl;
      }
    else{
       inputFile.seekg (0, std::ios_base::end);
       length = inputFile.tellg();
       if(length > 0){
          progSrc = new char[length]();
          inputFile.seekg (0, std::ios_base::beg);
          inputFile.read((char*)progSrc, length*sizeof(char)-1);
          inputFile.close();

          fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
          glShaderSource(fragmentShader, 1, &progSrc,NULL);
          delete[] progSrc;

          glCompileShader(fragmentShader);
          // Checking if the compilation was a success
          glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&compiledf);
          if(compiledf == 0){
             int infologLength = 0;

             glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infologLength);
             if(infologLength > 0){
                int charsWritten  = 0;
                GLchar *infoLog = new GLchar[infologLength]();
                glGetShaderInfoLog(fragmentShader, infologLength, &charsWritten, infoLog);
                std::cout<<"Fragment Shader Log Info: "<<infoLog<<std::endl;
                delete[]infoLog;
               }
            }
         }
      }
   }
    /*
     *
     * Linking the shader program
     *
     */
 GLuint tprog = glCreateProgram();
 if(tprog == 0)
    return false;

 if((f_vert == true) && (compiledv == 1)){
    glAttachShader(tprog,vertexShader);
   }
 if((f_geom == true) && (compiledg == 1)){
    glAttachShader(tprog,geomShader);
   }
 if((f_frag == true) && (compiledf == 1)){
    glAttachShader(tprog,fragmentShader);
   }
 if((f_vert == true) || (f_geom == true) || (f_frag == true)){
    int linked = 0;

    glLinkProgram(tprog);

    glGetProgramiv(tprog,GL_LINK_STATUS,&linked);
    if(linked == 0){
       int infologLength = 0;

          glGetProgramiv(tprog, GL_INFO_LOG_LENGTH,&infologLength);
          if(infologLength > 0){
             int charsWritten  = 0;
             char *infoLog = new char[infologLength]();
             glGetProgramInfoLog(tprog, infologLength, &charsWritten, infoLog);
             std::cout<<"Shader Program Log Info: "<<infoLog<<std::endl;
             delete[]infoLog;
            }
         }
       *prog = tprog;
      }
 return true;
}




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
    imag[0].load("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/texturaConejoG.jpg");
    imag[1].load("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/Conejo.jpg");
    imag[2].load("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/textura2.jpg");
    imag[3].load("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/texturaConejo.jpg");
    imag[4].load("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/Piel.jpg");
    imag[5].load("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/textura.jpg");

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
    glEnable(GL_LIGHTING);


    glEnable(GL_LIGHT0);
    GLfloat light_Pos0[4] = {2.0,2.0,5.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_Pos0);
    GLfloat light_ambient0[4]  = {0.1,0.1,0.1,1.0};
    GLfloat light_diffuse0[4]  = {0.5,0.7,0.7,1.0};
    GLfloat light_specular0[4] = {0.5,0.5,0.5,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse0);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular0);

    glEnable(GL_LIGHT1);
    GLfloat light_Pos1[4] = {-2.0,2.0,-5.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,light_Pos1);
    GLfloat light_ambient1[4]  = {0.7,0.1,0.1,1.0};
    GLfloat light_diffuse1[4]  = {0.7,0.2,0.7,1.0};
    GLfloat light_specular1[4] = {0.7,0.5,0.5,1.0};
    glLightfv(GL_LIGHT1,GL_AMBIENT,light_ambient1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse1);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light_specular1);


    GLfloat attenuation[3] = {4.0f,4.2f,4.2f};
    glLightfv(GL_LIGHT0,GL_CONSTANT_ATTENUATION,attenuation);
    glLightfv(GL_LIGHT1,GL_CONSTANT_ATTENUATION,attenuation);
glEnable(GL_COLOR_MATERIAL);

    fprintf(stderr, "-------- Cargando Modelo -------\n");
    M_Obj.cargarModelo("/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/bunny.obj");

    this->Gouraud=false;
    this->Phong=true;
    this->Poligon=false;
    this->Mall=false;
    this->Punt=false;
    this->Texture=false;
    this->RecalMatri=true;
    this->numTexture=0;

    string nvert="/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/shaders/Phong.vert";
    string ngeom="";
    string nfrag="/home/noryab/Graficacion/QT/Tarea5/Tarea5_Graficacion_F/shaders/Phong.frag";
    GLuint *prog;
    CompileShader(nvert, ngeom, nfrag,prog);
    program=*prog;
}

void renderObj::paintGL()
{    
    RenderObj();
}


void renderObj::RenderObj()
{

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble fovy=70;
    GLdouble aspect=(float)width()/height();
    GLdouble zNear=1;
    GLdouble zFar=10;
    gluPerspective(fovy,aspect,zNear,zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,1.0,5.0,  0.0,1.0,0.0,  0,1,0);

    if(BoolRot)
    {
        glRotatef(Angle_X,1,0,0);
        glRotatef(Angle_Y,0,1,0);
        glRotatef(Angle_Z,0,0,1);
    }
    BoolRot=false;

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

        glShadeModel(GL_SMOOTH);
        glUseProgram(program);



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

        GLfloat EMISS[] = {0.2f, 0.3f, 0.2f, 1.0f};
        GLfloat mAmbient[] = {0.58f, 0.68f, 0.68f, 1.0f};
        GLfloat mDiffuse[] = {0.88f, 0.88f, 0.88f, 1.0f};
        GLfloat mSpecular[] = {0.6f, 0.78f, 0.58f, 1.0f};
        float shininessLevel = 16.0f;

          glMaterialfv( GL_FRONT,  GL_AMBIENT, mAmbient);
          glMaterialfv( GL_FRONT,  GL_DIFFUSE, mDiffuse);
          glMaterialfv( GL_FRONT,  GL_SPECULAR, mSpecular);
          glMaterialf( GL_FRONT,  GL_SHININESS, shininessLevel);
          glMaterialfv( GL_FRONT,  GL_EMISSION, EMISS);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

            glEnable(GL_CULL_FACE);
            glVertexPointer(3,GL_FLOAT,0,Vertx);
            glNormalPointer(GL_FLOAT,0,Norms);
            glDrawArrays(GL_TRIANGLES,0,(M_Obj.getNC()*3));
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        glUseProgram(0);


    }

    fprintf(stderr, "-------- Renderizado -------\n");

}
