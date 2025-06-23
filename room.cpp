#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
void full_brik();
void half_brik();
void full_brik_line();
void half_brik_line();
void wall();
void full_half_brik_line();
void brick_wall();
void brick_font_space_wall();
void brick_back_space_wall();

void room(int a,int b,int c,int d,int m){
    glPushMatrix();
    glEnable (GL_BLEND);
    glDepthMask (GL_FALSE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);//roof
    glTranslatef (0.0, 2.3, 0.0);
    glScalef(1.0,0.02,1.0);
    glutSolidCube (4.3);
    glDepthMask (GL_TRUE);
    glDisable (GL_BLEND);
    glPopMatrix();

    if(a==1){//front mirror
        glPushMatrix();
        //glRotated(90,0,1,0);
        if(m==1){
            glTranslatef (0.26,-0.9, 1.7);
            brick_font_space_wall();
        }else{
            glTranslatef (0.26,-0.9, 2.0);
            brick_wall();
        }
        glPopMatrix();
    }
    if(b==1){
        glPushMatrix();
        glTranslatef (2.0,-0.9, -0.25);//right wall
        glRotated(90,0,1,0);
        brick_wall();
        glPopMatrix();
    }
    if(c==1){
        glPushMatrix();
        glTranslatef (-2.0,-0.9, -0.25);//left wall
        glRotated(90,0,1,0);
        brick_wall();
        glPopMatrix();
    }
    if(d==1){
        glPushMatrix();
        glTranslatef (0.26,-0.9, -2.0);//backward wall
        if(m==1){
            brick_font_space_wall();
        }else{
            brick_wall();
        }
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef (2.0,0.65, 2.0);//forward right line
    glScalef(1.2,11.08,1.1);
    glutSolidCube (0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef (-2.0,0.65, 2.0);//forward left line
    glScalef(1.2,11.08,1.1);
    glutSolidCube (0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef (-2.0,0.65, -2.0);//backward left line
    glScalef(1.2,11.08,1.1);
    glutSolidCube (0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef (2.0,0.65, -2.0);//backward right line
    glScalef(1.2,11.08,1.1);
    glutSolidCube (0.3);
    glPopMatrix();

    //flour
    glPushMatrix();
    glColor3f(1.0f,1.0f,1.0f);
    glScalef(1.4,0.01,1.4);
    glTranslated (0.0, -100.8, 0.0);
    glutSolidCube (3);
    glPopMatrix();
}

void brick_wall(){
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 2.0);
            glVertex3f(1.6, 3.2, 0.15);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.6, -0.1, 0.15);
            glTexCoord2f(2.0, 0.0);
            glVertex3f(-2.1,-0.1,0.15);
            glTexCoord2f(2.0, 2.0);
            glVertex3f(-2.1, 3.2,0.15);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 2.0);
            glVertex3f(1.6, 3.2, -0.15);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.6, -0.1, -0.15);
            glTexCoord2f(2.0, 0.0);
            glVertex3f(-2.1,-0.1,-0.15);
            glTexCoord2f(2.0, 2.0);
            glVertex3f(-2.1, 3.2,-0.15);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        glColor3d(2.0f,2.0f,2.0f);
        glBegin(GL_QUADS);
            //glTexCoord2f(0.0, 2.0);
            glVertex3f(1.6, 3.2, 0.15);
            //glTexCoord2f(0.0, 0.0);
            glVertex3f(-2.1,3.2, 0.15);
            //glTexCoord2f(2.0, 0.0);
            glVertex3f(-2.1,3.2,-0.15);
            //glTexCoord2f(2.0, 2.0);
            glVertex3f(1.6, 3.2,-0.15);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(1.6, -0.1, 0.15);
            glVertex3f(-2.1,-0.1, 0.15);
            glVertex3f(-2.1,-0.1,-0.15);
            glVertex3f(1.6, -0.1,-0.15);
        glEnd();
    glPopMatrix();
}
void brick_font_space_wall(){
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 2.0);
            glVertex3f(1.6, 3.2, 0.15);
            glTexCoord2f(0.0, 1.81);
            glVertex3f(1.6, 2.9, 0.15);
            glTexCoord2f(2.0, 1.81);
            glVertex3f(-2.1,2.9,0.15);
            glTexCoord2f(2.0, 2.0);
            glVertex3f(-2.1, 3.2,0.15);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 2.0);
            glVertex3f(1.6, 0.17, 0.15);
            glTexCoord2f(0.0, 1.81);
            glVertex3f(1.6, -0.1, 0.15);
            glTexCoord2f(2.0, 1.81);
            glVertex3f(-2.1,-0.1,0.15);
            glTexCoord2f(2.0, 2.0);
            glVertex3f(-2.1, 0.17,0.15);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.5);
            glVertex3f(1.6, 2.9, 0.15);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.6, 0.17, 0.15);
            glTexCoord2f(0.27, 0.0);
            glVertex3f(1.165, 0.17, 0.15);
            glTexCoord2f(0.27, 1.5);
            glVertex3f(1.165, 2.9, 0.15);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.5);
            glVertex3f(-1.6, 2.9, 0.15);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-1.6, 0.17, 0.15);
            glTexCoord2f(0.27, 0.0);
            glVertex3f(-2.1, 0.17, 0.15);
            glTexCoord2f(0.27, 1.5);
            glVertex3f(-2.1, 2.9, 0.15);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
