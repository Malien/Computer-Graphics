#include "result.hpp"
#include <algorithm>
#include <cstring>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <optional>

#include "argparse.hpp"
#include "model.hpp"
#include "util.hpp"

std::optional<VertexBuffer> vb;

const GLfloat specularEffect = 0.25f;
float lightingAngle = 0;
bool rotateLight = false;
bool rotateModel = false;
bool shouldAdjust = false;

const GLfloat lightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat lightSpecular[] =
    {specularEffect, specularEffect, specularEffect, specularEffect};
const GLfloat lightPosition[] = {2.0f, 5.0f, 5.0f, 0.0f};

const GLfloat matAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat matDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat highShininess[] = {100.0f};

void resize(int width, int height) {
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t * 90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.6, 0.6, 0.6);

    float finalLightAngle = rotateLight ? a / 80 : lightingAngle;
    const GLfloat light_position[] =
        {2.0f * sinf(finalLightAngle), 0.0f, 2.0f * cosf(finalLightAngle), 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glPushMatrix();

    if (shouldAdjust) {
        glTranslated(0, -1.75, -4);
    } else {
        glTranslated(0, 0, -3);
    }

    if (rotateModel) {
        glRotated(a, 0, 1, 0);
    }
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
        lightingAngle += 0.1;
        break;

    case '-':
        lightingAngle -= 0.1;
        break;

    case 'm':
        rotateModel = !rotateModel;
        break;

    case 'l':
        rotateLight = !rotateLight;
        break;
    }

    glutPostRedisplay();
}

struct ProgramArgs {
    char const *pathname;
    bool shouldAdjust = false;
};

Result<ProgramArgs, const char *> parseArgs(int argc, const char *argv[]) {
    if (argc < 2) {
        return Result<ProgramArgs, const char *>::error("Not enough arguments provided");
    }
    ProgramArgs args;
    if (strcmp(argv[1], "--adjust-position") == 0) {
        args.shouldAdjust = true;
        args.pathname = argv[2];
    } else {
        args.pathname = argv[1];
        if (argc == 3 && strcmp(argv[2], "--adjust-position") == 0) {
            args.shouldAdjust = true;
        }
    }
    return Result<ProgramArgs, const char *>::ok(args);
}

int main(int argc, const char *argv[]) {
    const auto pathname = parseArgs(argc, argv).match<const char *>()(
        [](const ProgramArgs &args) {
            shouldAdjust = args.shouldAdjust;
            return args.pathname;
        },
        [](const char *error) {
            std::cerr << error << '\n';
            exit(1);
            // Yeah, this is not that great
            return error;
        }
    );

    const auto model = Model::fromObjectFile(pathname).unwrap();
    vb = model.vertexBuffer();
    const auto data = reinterpret_cast<const float *>(vb->data());

    glutInit(&argc, const_cast<char **>(argv));
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Shapes with lighting");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(glutPostRedisplay);

    // glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, highShininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
