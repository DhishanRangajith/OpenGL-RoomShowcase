#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void sun_moon_action(int distance_of_sun,int distance_of_moon,GLfloat *sun_light,GLfloat *moon_light,int spin){
    GLfloat position[] = { distance_of_sun, 0.0, 0.0, 0.0 };
    GLfloat position2[] = { distance_of_moon, 0.0, 0.0, 0.0 };
    //sun
    glPushMatrix ();
        glRotated ((GLdouble) spin, 0.0, 0.0, 1.0);
        glLightfv (GL_LIGHT0, GL_POSITION, position);
        GLfloat mat_shininess1[] = { 1.0 };
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess1);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light);
        glTranslated (distance_of_sun, 0.0, 0.0);//
        glDisable (GL_LIGHTING);
        glColor3f (0.0, 1.0, 1.0);//sun color
        glutSolidSphere (0.2, 20, 16);//sun
        glEnable (GL_LIGHTING);
    glPopMatrix ();
    //moon
    glPushMatrix ();
    glRotated ((GLdouble) spin+180, 0.0, 0.0, 1.0);
    GLfloat mat_shininess2[] = { 100.0 };
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
    glLightfv (GL_LIGHT1, GL_POSITION, position2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, moon_light);
    glTranslated (distance_of_moon, 0.0, 0.0);
    glDisable (GL_LIGHTING);
    glColor3f (1.5, 1.5, 0.0);//moon color
    glutSolidSphere (0.3, 20, 16);//moon
    glEnable (GL_LIGHTING);
    glPopMatrix ();
}
