// Department of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Libraries used: SOIL (https://www.lonesock.net/soil.html) and CGLM (https://github.com/recp/cglm)
//
// Particle System - Snow
//
// Author: Diana Pislaru
//

#include <stdlib.h>
#include <GLUT/glut.h>
#include <SOIL.h>
#include <sys/time.h>
#include "window2.c"
#include "particles.c"
#include "house.c"

#define MILLION 1000000

GLuint textures[2];

struct Point WORLD = {800.0f, 500.0f, 800.0f};

struct Particle particles[MILLION];

struct Particle createParticle() {
    struct Particle p;

    p.pos.x = myRandom() * WORLD.x;
    p.pos.y = WORLD.y;
    p.pos.z = myRandom() * WORLD.z;

    p.time = 0.0f;

    p.type = 0;
    if (myRandom() > 0.5) p.type = 1;

    return p;
}

void emitParticles() {
    if (N >= MAX_PARTICLES) {
        N = MAX_PARTICLES;
        return;
    }

    int NEW_N = N + PARTICLES_PER_FR;

    for(int i = N; i < NEW_N; ++i) {
        particles[i] = createParticle();
    }

    N = NEW_N;
}

void loadTexture()
{
    textures[0] = SOIL_load_OGL_texture(
            "../snow1.png",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y);

    textures[1] = SOIL_load_OGL_texture(
            "../snow2.png",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y);

    loadHouseTexture();
}

void renderParticles()
{
    struct Particle particle;
    struct Location partPos;
    int type;
    float size;
    if (TEXTURE) size = SIZE_QUAD/2;
    else         size = SIZE_POINT;

    glDepthMask(GL_FALSE);

    for (int i = 0; i < N; ++i)
    {
        particle = particles[i];
        partPos = particle.pos;
        type = particle.type;

        glColor4f(COLOR.x, COLOR.y, COLOR.z, 0.5);

        if (TEXTURE) {
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_ALPHA_TEST);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glBindTexture(GL_TEXTURE_2D, textures[type]);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(partPos.x + size, partPos.y - size, partPos.z);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(partPos.x + size, partPos.y + size, partPos.z);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(partPos.x - size, partPos.y + size, partPos.z);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(partPos.x - size, partPos.y - size, partPos.z);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        } else {
            glPointSize(size);
            glBegin(GL_POINTS);
                glVertex3f(partPos.x, partPos.y,partPos.z);
            glEnd();
        }
    }
    glDepthMask(GL_TRUE);
}


void updateParticles() {
    int ground;
    int dead_particles = 0;
    for(int i = 0; i < N; ++i) {
        particles[i].pos.y += GRAVITY * particles[i].time * particles[i].time + particles[i].time * VELOCITY.y;
        particles[i].time += 0.001;

        ground = 0.0;
        if (HOUSES && HOUSE_PLAN[particles[i].pos.x][particles[i].pos.z] > 0) {
            if (HOUSE_PLAN[particles[i].pos.x][particles[i].pos.z] == 1) {
                ground = house1_H;
            } else {
                ground = house2_H;
            }
        }

        if (particles[i].pos.y <= ground) {
            particles[i].pos.y = ground;
            if (particles[i].time >= LIFETIME) {
                particles[i] = createParticle();
                dead_particles++;
            }
        }
    }

    N-=dead_particles;
    for(int i = 0; i < N; ++i) {
        particles[i] = particles[dead_particles+i];
    }


    glutPostRedisplay();
    renderParticles();
    emitParticles();
}

void calculateFrameRate() {
    FRAME_COUNT++;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (FRAME_COUNT % 10 == 0) {
        FRAME_COUNT = 0;
        FRAME_RATE = 1000 / (currentTime - TIME_ELAPSED);
    }
    TIME_ELAPSED = currentTime;
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    calculateFrameRate();

    glLoadIdentity();

    gluLookAt(position[0], position[1], position[2],
              position[0]+direction[0], position[1]+direction[1], position[2]+direction[2],
              up[0], up[1], up[2]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(AXIS_ENABLED) glCallList(axisList);
    if(FLOOR_ENABLED) glCallList(floorList);
    if(HOUSES) {
        draw_house(house1_E, house1_S, house1_V, house1_N, house1_H);
        draw_house(house2_E, house2_S, house2_V, house2_N, house2_H);
    }

    updateParticles();

    glutSwapBuffers();
}

/*
 ################################################## MAIN ##################################################
 */

void initMainWindow() {
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(10, 100);
    glutCreateWindow("COMP37111 Particles");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
    glClearColor(0.1,0.1,0.1,1.0);
}

void initGraphics(int argc, char *argv[]) {
    glutInit(&argc, argv);
    COLOR = WHITE;

    // Main Window
    initMainWindow();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutSpecialFunc(arrows);
    glutMotionFunc(mouseMove);
    glutMouseFunc(mousePress);
    makeAxes();
    makeFloor();
    emitParticles();
    loadTexture();
    initHousePlan();

    // Control Window
    initControlWindow();
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrows);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    initGraphics(argc, argv);
    glutMainLoop();

    return 0;
}
