//
// Created by Diana Pislaru on 03/12/2019.
//

#include <stdlib.h>
#include <cglm/cglm.h>

GLuint axisList;
GLuint floorList;

int AXIS_SIZE= 800;
bool AXIS_ENABLED = true;
bool FLOOR_ENABLED = true;
bool HOUSES = true;
bool TEXTURE = true;

int SIZE = 20;
int PARTICLES_PER_MS = 1;
int MAX_PARTICLES = 100;
int LIFETIME = 1; // seconds

int N = 0;
int TIME_ELAPSED = 0;
float FRAME_RATE = 0.0f;

struct Point {
    float x, y, z;
};

struct Location {
    int x, z;
    float y;
};

vec3 position = {400.0f, 200.0f, 700.0f};
vec3 direction = {0.0f, 0.0f, -1.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

float myRandom()
{
    return (rand()/(float)RAND_MAX);
}

void makeAxes() {
    // Create a display list for drawing coord axis
    axisList = glGenLists(1);
    glNewList(axisList, GL_COMPILE);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);       // X axis - red
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(AXIS_SIZE, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);       // Y axis - green
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, AXIS_SIZE, 0.0);
    glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
    glEndList();
}

void makeFloor() {
    floorList = glGenLists(1);
    glNewList(floorList, GL_COMPILE);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor4f(1.0, 1.0, 1.0, 0.1);
    for(int i = 10; i < AXIS_SIZE; i += 10) {
        glVertex3f(i, 0.0, 0.0);
        glVertex3f(i, 0.0, AXIS_SIZE);
        glVertex3f(0.0, 0.0, i);
        glVertex3f(AXIS_SIZE, 0.0, i);
    }
    glEnd();
    glEndList();
}
