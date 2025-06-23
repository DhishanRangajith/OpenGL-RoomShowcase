#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <assert.h>
#include "materials.h"
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>
//////////////////////////////////////////////////////////////moon_sun_data
GLfloat distance_of_sun=-10.0;
GLfloat distance_of_moon=-10.0;
GLfloat sun_light[] ={100.5, 10.5, 7.0, 5.0};
GLfloat moon_light[] ={0.6, 0.6, 0.6, 0.0};
int time_speed_2=100;//from timing (miliSecond)
int time_speed_1=1;//from rounding
int spin = 0;

void book();
int view_camera=0;//1-cameraA 2-cameraB 3-cameraC 4-Moving/RotatingCamera
int start=1;
void update(int value);
void output(GLfloat x, GLfloat y, char *format,...);
void notify();
void menu(int value);
void head_mirror_set(GLfloat x);
void front_back_mirror_room(int a,int d,GLfloat scale_z);
void con_mirror();
void con_mirror();
void materials( );
GLuint texture[5];
void idleFunc( );
void displayFunc( );
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void initialize( );
void normal_mirror( GLfloat val );
void main_room();
void round_border(GLfloat height);
void pic_concav_mirror();
void pic_convax_mirror();
void right_mirror();
void left_mirror();
void back_mirror();
void head_mirror();
void left_mirror_room_main();
void right_mirror_room_main();


#define INC_VAL     2.0f
#ifndef M_PI
#define M_PI        3.14159265359
#endif

GLsizei g_width = 1100;
GLsizei g_height = 360;

GLboolean g_rotate = GL_TRUE;

// light 0 position
GLfloat g_light0_pos[4] = { 10.0f, 5.0f, 0.0f, 1.0f };
// clipping planes
GLdouble eqn1[4] = { 0.0, 0.0, 1.0, 0.0 };
GLdouble eqn2[4] = { 0.0, 0.0, -1.0, 0.0 };
GLdouble eqn3[4] = { 0.0, 0.0, 0.0, 0.0 };
GLdouble eqn4[4] = { 0.0, 0.0, 0.0, 0.0 };

GLfloat g_inc = 0.0f;

typedef struct Image Image;
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

GLfloat tx=0.0,ty=0.0,tz=3.0;
GLfloat qx=0.0,qy=0.0,qz=0.0;
GLfloat wx=9.0,wy=0.0;

GLfloat val = 1.5;
GLint buffers = GL_NONE;

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
void update(int value){
    spin = (spin + time_speed_1) % 360;
    glutPostRedisplay();
    glutTimerFunc(time_speed_2,update,0);
}
void notify(){
glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_LINE_SMOOTH);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 10000, 0, 7000);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
//
//  output(80, 6500, "x = %f",tx+qx);
//  output(80, 6000, "y = %f",ty+qy);
//  output(80, 5500, "z = %f",tz+qz);
//  output(80, 5000, "xi = %f",qx);
//  output(80, 4500, "yi = %f",qy);
//  output(80, 4000, "zi = %f",qz);
//  output(80, 3500, "Rotate x = %f",wx);
//  output(80, 3000, "Rotate y = %f",wy);


  glColor3f(0.0,0.0,1.0);
    if(view_camera==1){
        output(80, 6800, "Camera C");
    }else if(view_camera==2){
        output(80, 6800, "Camera D");
    }else if(view_camera==3){
        output(80, 6800, "Camera E");
    }else if(view_camera==4){
        output(80, 6800, "Moving/Rotating Camera");
        output(500, 100, "<A>-left <D>-right <W>-up <S>-down <Z>-zoom in <X>-zoom out <U>-rotate up <J>-rotate down <H>-rotate left <L>-rotate right");
    }
    if(start==1){
            glColor3f(0.0,1.0,0.0);
            output(3000, 100, "You can select a camera from clicking left mouse button");
    }

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopAttrib();
  glutSwapBuffers();

}
void output(GLfloat x, GLfloat y, char *format,...){
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

void makeCheckImage(void){
    int i, j, c;
    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}

int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
    char temp; // temporary color storage for bgr-rgb conversion.
    // make sure the file is there.
    file = fopen(filename, "rb");
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    // read the width
    i = fread(&image->sizeX, 4, 1, file);
    //printf("Width of %s: %lu\n", filename, image->sizeX);
    // read the height
    i = fread(&image->sizeY, 4, 1, file);
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    // read the planes
    fread(&planes, 2, 1, file);
    // read the bitsperpixel
    i = fread(&bpp, 2, 1, file);
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    // read the data.
    image->data = (char *) malloc(size);

    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    return 1;
}

Image * loadTexture(){
    Image *image1;
    // allocate space for textur
    image1 = (Image *) malloc(sizeof(Image));
    if (!ImageLoad("floorr.bmp", image1)) {
        exit(1);
    }
    return image1;
}
Image * loadTexture2(){
    Image *image2;
    // allocate space for textur
    image2 = (Image *) malloc(sizeof(Image));

    if (!ImageLoad("flo.bmp", image2)) {
        exit(1);
    }
    return image2;
}
Image * loadTexture3(){
    Image *image3;
    // allocate space for textur
    image3 = (Image *) malloc(sizeof(Image));

    if (!ImageLoad("pages.bmp", image3)) {
        exit(1);
    }
    return image3;
}
Image * loadTexture4(){
    Image *image4;
    // allocate space for textur
    image4 = (Image *) malloc(sizeof(Image));

    if (!ImageLoad("pages.bmp", image4)) {
        exit(1);
    }
    return image4;
}
Image * loadTexture5(){
    Image *image5;
    // allocate space for textur
    image5 = (Image *) malloc(sizeof(Image));

    if (!ImageLoad("bcover.bmp", image5)) {
        exit(1);
    }
    return image5;
}

void keyboardFunc (unsigned char key, int x, int y){
    if(view_camera==4){
        switch (key){
            case 'D':
            case 'd':   qx+=0.1;//left move
                        glutPostRedisplay();
                        break;
            case 'A':
            case 'a':   qx-=0.1;//right move
                        glutPostRedisplay();
                        break;
            case 'W':
            case 'w':   qy+=0.1;//up move
                        glutPostRedisplay();
                        break;
            case 'S':
            case 's':   qy-=0.1;//down move
                        glutPostRedisplay();
                        break;
            case 'Z':
            case 'z':   qz-=0.1;//zoom in
                        glutPostRedisplay();
                        break;
            case 'X':
            case 'x':   qz+=0.1;//zoom out
                        glutPostRedisplay();
                        break;
            case 'K':
            case 'k':   wx-=1.0;//rotate down
                        glutPostRedisplay();
                        break;
            case 'I':
            case 'i':   wx+=1.0;//rotate up
                        glutPostRedisplay();
                        break;
            case 'L':
            case 'l':   wy-=1.0;//rotate right
                        glutPostRedisplay();
                        break;
            case 'J':
            case 'j':   wy+=1.0;//rotate left
                        glutPostRedisplay();
                        break;
            default:    break;
        }
    }
}
void menu(int value){
    switch(value){
        case 1:view_camera=1;start=0;//camera A
                glutPostRedisplay();
            break;
        case 2:view_camera=2;start=0;//camera B
                glutPostRedisplay();
            break;
        case 3:view_camera=3;start=0;//camera C
                glutPostRedisplay();
            break;
        case 4:view_camera=4;start=0;//MR camera
                glutPostRedisplay();
            break;
        default:view_camera=0;start=1;//
                glutPostRedisplay();
            break;
    }
}

int main( int argc, char ** argv ){
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL );
    glutInitWindowSize( 1300, 700 );
    glutCreateWindow( "Mirror Sample" );
    glutReshapeFunc( reshapeFunc );
    glutDisplayFunc( displayFunc );

    glutCreateMenu(menu);
      glutAddMenuEntry("use camera A",1);
      glutAddMenuEntry("use camera B",2);
      glutAddMenuEntry("use camera C",3);
      glutAddMenuEntry("use moving/rotating camera",4);
      glutAddMenuEntry("reset window",5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutKeyboardFunc( keyboardFunc );
    initialize();
    glutTimerFunc(time_speed_2,update,0);
    glutMainLoop();
    return 0;
}

void initialize()
{
    glClearColor( 0.0f, 0.0f,0.0f, 1.0f );
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glFrontFace( GL_CCW );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glLineWidth( 2.0f );
    glEnable( GL_LIGHTING );
    glLightModeli( GL_FRONT, GL_TRUE );
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_LIGHT0 );

     glDepthFunc(GL_LESS);
    Image *image1 = loadTexture();
    Image *image2 = loadTexture2();
    Image *image3 = loadTexture3();
    Image *image4 = loadTexture4();
    Image *image5 = loadTexture5();

    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Create Texture
    glGenTextures(5, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image2->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image3->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image4->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image5->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    //glEnable(GL_TEXTURE_2D);
   // glShadeModel(GL_FLAT);
}

void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    g_width = w; g_height = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( 45.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0 );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
}

void idleFunc( )
{
    glutPostRedisplay( );
}

void displayFunc( )
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
    glLoadIdentity ();
    if(wx==-1){wx=359;}else if(wx==361){wx=1;}
    if(wy==-1){wy=359;}else if(wy==361){wy=1;}
    if(view_camera==4){//move camera
        gluLookAt(tx+qx,ty+qy,tz+qz,qx,qy,qz,0.0,1.0,0.0);
        glRotatef(wx,1.0,0.0,0.0);
        glRotatef(wy,0.0,1.0,0.0);
    }else if(view_camera==1){
        gluLookAt(0.1,0.2,3.1,0.1,0.2,-4.9,0.0,1.0,0.0);
        glRotatef(43,1.0,0.0,0.0);
        glRotatef(46,0.0,1.0,0.0);
    }else if(view_camera==2){
        gluLookAt(0.1,0.0,3.2,0.1,0.0,-4.8,0.0,1.0,0.0);
        glRotatef(51.0,1.0,0.0,0.0);
        glRotatef(-127.0,0.0,1.0,0.0);
    }else if(view_camera==3){
        gluLookAt(0.0,-0.2,2.5,0.0,-0.2,-5.5,0.0,1.0,0.0);
        glRotatef(-17.0,1.0,0.0,0.0);
        glRotatef(-40.0,0.0,1.0,0.0);
    }else{
        gluLookAt(0.0,0.7,11.2,0.0,0.7,8.2,0.0,1.0,0.0);
        glRotatef(58.0,1.0,0.0,0.0);
        tx=0.0;ty=0.0;tz=3.0;qx=0.0;qy=0.7;qz=8.2;wx=58.0;wy=0.0;
    }

////////////////////////////////////////////////////////////////////////
//    // get the current color buffer being drawn to
    glGetIntegerv( GL_DRAW_BUFFER, &buffers );


	    main_room();
notify();
    glFlush( );

   // glutSwapBuffers( );

}

void normal_mirror( GLfloat val ){
    glVertex3f( val+0.2, val, 0.0f );
    glVertex3f( -val-0.2, val, 0.0f );
    glVertex3f( -val-0.2, -val, 0.0f );
    glVertex3f( val+0.2, -val, 0.0f );
}

void materials( ){
        //glLightfv( GL_LIGHT0, GL_POSITION, g_light0_pos );

        glPushMatrix();
            glEnable (GL_BLEND);
           // glDepthMask (GL_FALSE);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);
            glTranslatef( 0.7,0.1,-0.2 );
            glColor3f( 1.0f, 1.0f, 1.5f );
            glutSolidCube( .2 );
           // glDepthMask (GL_TRUE);
            glDisable (GL_BLEND);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-0.4,0,0.2);
            glPushMatrix();
                glColor3f(0.5f,1.0f,0.5f);
                glScaled(0.4,0.5,0.5);
                glRotatef(0,0,0,0);
                glTranslatef( 0,0.21,0 );
                glBindTexture(GL_TEXTURE_2D, texture[4]);
                book();
            glPopMatrix();
            glPushMatrix();
                glColor3f(1.0f,0.5f,0.5f);
                glScaled(0.41,0.3,0.51);
                glRotatef(15,0,1,0);
                glTranslatef( 0,0.56,0 );
                glBindTexture(GL_TEXTURE_2D, texture[4]);
                book();
            glPopMatrix();
            glPushMatrix();
                glColor3f(0.5f,0.5f,1.0f);
                glScaled(0.31,0.3,0.31);
                glRotatef(15,0,1,0);
                glTranslatef( 0,0.77,0 );
                glBindTexture(GL_TEXTURE_2D, texture[4]);
                book();
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            table();
        glPopMatrix();
}
void main_room(){
    //sun & moon
    sun_moon_action(distance_of_sun,distance_of_moon,sun_light,moon_light,spin);
//tx=0.0;ty=0.0;tz=3.0;qx=0.0;qy=0.7;qz=8.2;wx=58.0;wy=0.0;
    int hide_left_mirror=0,hide_right_mirror=0,hide_head_mirror=0,hide_back_mirror=0;
    if(qx>3.0 || tz>2.0 || qx<-3.0 || tz<-2){
        if(wy>180 && wy<360){hide_left_mirror=1;}
        if(wy>0 && wy<180){hide_right_mirror=1;}
        if(wy>90 && wy<270){hide_head_mirror=1;}
        if(wy>0 && wy<90){hide_back_mirror=1;}
    }
    left_mirror();
    right_mirror();
    head_mirror();
    back_mirror();






    glDrawBuffer( (GLenum) buffers );
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    glPushMatrix();
        glTranslatef(0,-0.22,0);
        materials();
    glPopMatrix();
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glScaled(1.2,1.1,1.2);
        room(1,1,1,1,0);
    glPopMatrix();
//    //convex mirror
////    glPushMatrix();
////        glTranslated(3.54,0.73,0);
////        glRotated(-90,0,1,0);
////        con_mirror();
////    glPopMatrix();
    glPushMatrix();
        glTranslated(2.24,0.73,0);
        glRotated(-90,0,1,0);
        round_border(0.04);
    glPopMatrix();
    //concave mirror
//    glPushMatrix();
//        glColor3f(0,1,0);
//        glTranslated(-0.65,0.73,0);
//        glRotated(-90,0,1,0);
//        con_mirror();
//    glPopMatrix();
    glPushMatrix();
        glTranslated(-1.97,0.73,0);
        glRotated(-90,0,1,0);
        round_border(0.25);
    glPopMatrix();

////

////

}
void con_mirror(){
    glColor3f(0.5,0.5,0.5);
    glPushMatrix();
        glScaled(2.1,2.1,1.2);
        GLdouble eqn[4] = {0.0, 0.0, 1.0, -1.1};
        glClipPlane (GL_CLIP_PLANE0, eqn);
        glEnable (GL_CLIP_PLANE0);
        glutSolidSphere(1.3, 100, 150);
        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}
void pic_con_mirror(){
    //glBindTexture(GL_TEXTURE_2D, texture[2]);
    //glEnable(GL_TEXTURE_2D);
    //GLUquadric *quad;
//    quad = gluNewQuadric();
//    glPushMatrix();
//        gluQuadricTexture(quad,-10);
//        glScaled(2.1,2.1,1.2);
//        GLdouble eqn[4] = {0.0, 0.0, 1.0, -1.1};
//        glClipPlane (GL_CLIP_PLANE0, eqn);
//        glEnable (GL_CLIP_PLANE0);
//        gluSphere(quad,1.3, 100, 150);
//        glDisable(GL_CLIP_PLANE0);
//    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);
    con_mirror();
}
void pic_concav_mirror(){

}

void round_border(GLfloat height){
  glPushMatrix();
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    glColor3f(0.4, 0.4, 1.0);
    gluCylinder(quadObj,1.453,1.453,height,100, 8);
  glPopMatrix();
}
void head_mirror(){
    glPushMatrix();
        GLfloat x1=0,y1=0.7,z1=-2.215,rx1=0,ry1=0,rz1=0,ra1=0;
        glClear( GL_STENCIL_BUFFER_BIT );
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
        glDrawBuffer( GL_NONE );
        glEnable( GL_STENCIL_TEST );

        glPushMatrix();
            glTranslatef(x1,y1,z1);
            glRotated(ra1,rx1,ry1,rz1);
            glBegin( GL_QUADS );
                normal_mirror( val );
            glEnd();
        glPopMatrix();

        glDrawBuffer( (GLenum) buffers );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

        glPushMatrix();
            glScalef( 1.0f, 1.0f, -1.0f );
            glClipPlane( GL_CLIP_PLANE0, eqn1 );
            glEnable( GL_CLIP_PLANE0 );
            glStencilFunc( GL_EQUAL, 0x1, 0x1 );

            glPushMatrix();//---------------------------------------------
                glTranslated(0,0,0);
                head_mirror_set(-1);
            glPopMatrix();

            glDisable( GL_CLIP_PLANE0 );
        glPopMatrix( );

        glDisable( GL_STENCIL_TEST );
        glDrawBuffer( GL_NONE );
    ////////////////////////////////----------------------
        glTranslatef(x1,y1,z1);
        glRotated(ra1,rx1,ry1,rz1);
        glPushMatrix();
            glBegin( GL_QUADS );
                normal_mirror( val );
            glEnd();
        glPopMatrix();

        glDrawBuffer( (GLenum) buffers );
        glPushMatrix();
            glColor3f( 0.4f, 0.4f, 1.0f );
            glBegin( GL_LINE_LOOP );
                normal_mirror( val );
            glEnd( );
        glPopMatrix();
    glPopMatrix();
}
void back_mirror(){
        glPushMatrix();
        GLfloat x2=0,y2=0.7,z2=2.215,rx2=0,ry2=0,rz2=0,ra2=0;

        glClear( GL_STENCIL_BUFFER_BIT );
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
        glDrawBuffer( GL_NONE );
        glEnable( GL_STENCIL_TEST );

        glPushMatrix();
            glTranslatef(x2,y2,z2);
            glRotated(ra2,rx2,ry2,rz2);
            glBegin( GL_QUADS );
                normal_mirror( val );
            glEnd();
        glPopMatrix();

        glDrawBuffer( (GLenum) buffers );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

        glPushMatrix();
            glScalef( 1.0f, 1.0f, -1.0f );
            glClipPlane( GL_CLIP_PLANE1, eqn2 );
            glEnable( GL_CLIP_PLANE1 );
            glStencilFunc( GL_EQUAL, 0x1, 0x1 );

            glPushMatrix();
                //glTranslated(0,0,-6);
                head_mirror_set(1);
            glPopMatrix();

            glDisable( GL_CLIP_PLANE1 );
        glPopMatrix( );

        glDisable( GL_STENCIL_TEST );
        glDrawBuffer( GL_NONE );
    ////////////////////////////////----------------------
        glPushMatrix();
            glTranslatef(x2,y2,z2);
            glRotated(ra2,rx2,ry2,rz2);
            glBegin( GL_QUADS );
                normal_mirror( val );
            glEnd();
        glPopMatrix();

        glDrawBuffer( (GLenum) buffers );
        glPushMatrix();
            glTranslatef(x2,y2,z2);
            glRotated(ra2,rx2,ry2,rz2);
            glColor3f( 0.4f, 0.4f, 1.0f );
            glBegin( GL_LINE_LOOP );
                normal_mirror( val );
            glEnd( );
        glPopMatrix();
    glPopMatrix();
}
void left_mirror(){
        glPushMatrix();
        GLfloat x3=-0.65,y3=0.73,z3=0,rx3=0,ry3=1,rz3=0,ra3=-90;

        glClear( GL_STENCIL_BUFFER_BIT );
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
        glDrawBuffer( GL_NONE );
        glEnable( GL_STENCIL_TEST );

        glPushMatrix();
            glTranslatef(x3,y3,z3);
            glRotated(ra3,rx3,ry3,rz3);
            con_mirror();
        glPopMatrix();

        glDrawBuffer( (GLenum) buffers );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

        glPushMatrix();
            glScalef( 1.0f, 1.0f, -1.0f );
            glClipPlane( GL_CLIP_PLANE3, eqn3 );
            glEnable( GL_CLIP_PLANE3 );
            glStencilFunc( GL_EQUAL, 0x1, 0x1 );

            glPushMatrix();
                glPushMatrix();
                    glTranslated(-4.2,0,0);
                    left_mirror_room_main();
                glPopMatrix();
            glPopMatrix();

            glDisable( GL_CLIP_PLANE3 );
        glPopMatrix( );

        glDisable( GL_STENCIL_TEST );
        glDrawBuffer( GL_NONE );
    ////////////////////////////////----------------------
        glPushMatrix();
            glTranslatef(x3,y3,z3);
            glRotated(ra3,rx3,ry3,rz3);
            con_mirror();
        glPopMatrix();
    glPopMatrix();

}
void right_mirror(){
    glPushMatrix();
        GLfloat x4=3.54,y4=0.73,z4=0,rx4=0,ry4=1,rz4=0,ra4=-90;

        glClear( GL_STENCIL_BUFFER_BIT );
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
        glDrawBuffer( GL_NONE );
        glEnable( GL_STENCIL_TEST );

        glPushMatrix();
            glTranslatef(x4,y4,z4);
            glRotated(ra4,rx4,ry4,rz4);
            con_mirror();
        glPopMatrix();

        glDrawBuffer( (GLenum) buffers );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

        glPushMatrix();
            glScalef( 1.0f, 1.0f, -1.0f );
            glClipPlane( GL_CLIP_PLANE4, eqn4 );
            glEnable( GL_CLIP_PLANE4 );
            glStencilFunc( GL_EQUAL, 0x1, 0x1 );

            glPushMatrix();
                glTranslated(4.0,0,0);
                right_mirror_room_main();
            glPopMatrix();

            glDisable( GL_CLIP_PLANE4 );
        glPopMatrix( );

        glDisable( GL_STENCIL_TEST );
        glDrawBuffer( GL_NONE );
    ////////////////////////////////----------------------
        glPushMatrix();
            glTranslatef(x4,y4,z4);
            glRotated(ra4,rx4,ry4,rz4);
            con_mirror();
        glPopMatrix();
    glPopMatrix();
}
void front_back_mirror_room(int a,int d,GLfloat scale_z){
    glPushMatrix();
        glScaled(1,1,scale_z);
        glPushMatrix();
            glTranslatef(0,-0.22,0);
            materials();
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glScaled(1.2,1.1,1.2);
            room(a,1,1,d,1);
        glPopMatrix();
        //convex mirror// picture_to
        glPushMatrix();
            glTranslated(3.54,0.73,0);
            glRotated(-90,0,1,0);
            con_mirror();
        glPopMatrix();
        glPushMatrix();
            glTranslated(2.24,0.73,0);
            glRotated(-90,0,1,0);
            round_border(0.04);
        glPopMatrix();
        //concave mirror//picture_to
        glPushMatrix();
            glColor3f(0,1,0);
            glTranslated(-0.65,0.73,0);
            glRotated(-90,0,1,0);
            pic_con_mirror();
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1.97,0.73,0);
            glRotated(-90,0,1,0);
            round_border(0.25);
        glPopMatrix();
    glPopMatrix();
}
void left_mirror_room_main(){
    glPushMatrix();
        glScaled(-1,1,-1);
        glPushMatrix();
            glTranslatef(0,-0.22,0);
            materials();
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glScaled(1.2,1.1,1.2);
            room(1,1,0,1,1);
        glPopMatrix();
        //convex mirror// picture_to
        glPushMatrix();
            glTranslated(3.54,0.73,0);
            glRotated(-90,0,1,0);
            con_mirror();
        glPopMatrix();
        glPushMatrix();
            glTranslated(2.24,0.73,0);
            glRotated(-90,0,1,0);
            round_border(0.04);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glScaled(-1,1,-1);
        glTranslatef(0,0,-4.7*1);
        front_back_mirror_room(1,1,-1);
    glPopMatrix();
    glPushMatrix();
        glScaled(-1,1,-1);
        glTranslatef(0,0,-4.7*-1);
        front_back_mirror_room(1,1,-1);
    glPopMatrix();
}
void right_mirror_room_main(){
    glPushMatrix();
        glScaled(-1,1,-1);
        glPushMatrix();
            glTranslatef(0,-0.22,0);
            materials();
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glScaled(1.2,1.1,1.2);
            room(1,0,1,1,1);
        glPopMatrix();
        //concave mirror//picture_to
        glPushMatrix();
            glColor3f(0,1,0);
            glTranslated(-0.65,0.73,0);
            glRotated(-90,0,1,0);
            con_mirror();
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1.97,0.73,0);
            glRotated(-90,0,1,0);
            round_border(0.25);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glScaled(-1,1,-1);
        glTranslatef(0,0,-4.7*1);
        front_back_mirror_room(1,1,-1);
    glPopMatrix();
    glPushMatrix();
        glScaled(-1,1,-1);
        glTranslatef(0,0,-4.7*-1);
        front_back_mirror_room(1,1,-1);
    glPopMatrix();
}
void head_mirror_set(GLfloat x){
    glPushMatrix();
        glTranslatef(0,0,-4.7*x);
        if(x==1){
                front_back_mirror_room(0,1,1);
        }else{
            front_back_mirror_room(1,0,1);
        }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,-4.7*2*x);
        front_back_mirror_room(1,1,-1);
    glPopMatrix();

}
void book(){
glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[4]);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(-0.5,0,-0.5);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.5, 0,-0.5);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(0.5,0,0.5);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-0.5,0,0.5);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glBegin(GL_QUADS);
                glVertex3f(-0.5,0,-0.5);
                glVertex3f(-0.5, 0,0.5);
                glVertex3f(-0.5, -0.2,0.5);
                glVertex3f(-0.5,-0.2,-0.5);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(-0.5,-0.2,-0.5);
                glVertex3f(0.5, -0.2,-0.5);
                glVertex3f(0.5,-0.2,0.5);
                glVertex3f(-0.5,-0.2,0.5);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.45,0,-0.45);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.45,-0.2,-0.45);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(0.45,-0.2,0.45);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(0.45,0,0.45);
            glEnd();
            glBegin(GL_QUADS);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(-0.5,0,0.45);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.45,0,0.45);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.45,-0.2,0.45);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-0.5,-0.2,0.45);
            glEnd();
            glBegin(GL_QUADS);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(-0.5,0,-0.45);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.45,0,-0.45);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.45,-0.2,-0.45);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-0.5,-0.2,-0.45);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

}

