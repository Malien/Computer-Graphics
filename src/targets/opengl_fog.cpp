#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "model.hpp"
#include <optional>
#include <stdlib.h>

static int slices = 32;
static int stacks = 32;

std::optional<VertexBuffer> vb;

static void resize(int width, int height) {
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void display() {
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t * 90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1, 0, 0);

    glPushMatrix();
    glTranslated(0, 0, -6 * (1 + sinf(a / 200)));
    glTranslated(-2.4, 1.5, -6);
    glRotated(60, 1, 0, 0);
    glRotated(a, 0, 0, 1);
    glutSolidSphere(1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, -6 * (1 + sinf(a / 200)));
    glTranslated(0, 1.5, -6);
    glRotated(60, 1, 0, 0);
    glRotated(a, 0, 0, 1);
    glutSolidCone(1, 1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, -6 * (1 + sinf(a / 200)));
    glTranslated(2.4, 1.5, -6);
    glRotated(60, 1, 0, 0);
    glRotated(a, 0, 0, 1);
    glutSolidTorus(0.2, 0.8, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, -6 * (1 + sinf(a / 200)));
    glTranslated(1.2, -1.5, -6);
    glRotated(60, 1, 0, 0);
    glRotated(a, 0, 0, 1);
    glutSolidTeapot(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, -6 * (1 + sinf(a / 200)));
    glTranslated(-2, -3, -8);
    glRotated(-a, 0, 1, 0);
    glBegin(GL_TRIANGLES);

    for (const auto &face : *vb) {
        const auto sideA = face[1] - face[0];
        const auto sideB = face[2] - face[0];
        const auto normal = sideA.cross(sideB);
        glNormal3f(normal[0], normal[1], normal[2]);
        for (const auto &vert : face) {
            glVertex3f(vert[0], vert[1], vert[2]);
        }
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
    case 'q':
        exit(0);
        break;

    case '+':
        slices++;
        stacks++;
        break;

    case '-':
        if (slices > 3 && stacks > 3) {
            slices--;
            stacks--;
        }
        break;
    }

    glutPostRedisplay();
}

const GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {2.0f, 5.0f, 5.0f, 0.0f};

const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

const GLfloat density = 0.3;

// const GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; // gray
// const GLfloat fogColor[4] = {1, 0.93, 0.83, 1.0}; // papayawhip
// const GLfloat fogColor[4] = {1, 0.85, 0.72, 1.0}; // peachpuff
const GLfloat fogColor[4] = {0.65, 0.68, 0.69, 1.0}; // morning fog


int main(int argc, char *argv[]) {
    const auto model = Model::fromObjectFile("stormtrooper.obj").unwrap();
    vb = model.vertexBuffer();
    const auto data = reinterpret_cast<const float *>(vb->data());

    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Fog");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(glutPostRedisplay);

    // glClearColor(0.5, 0.5 , 0.5, 1);
    glClearColor(fogColor[0], fogColor[1], fogColor[2], 1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.1f);
    glHint(GL_FOG_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
