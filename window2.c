//
// Created by Diana Pislaru on 03/12/2019.
//
#include <GLUT/glut.h>
#include <stdio.h>
#include "utils.h"
#include <string.h>

struct Point WHITE = {1.0f, 1.0f, 1.0f};
struct Point RED = {1.0f, 0.0f, 0.0f};
struct Point GREEN = {0.0f, 1.0f, 0.0f};
struct Point BLUE = {0.0f, 0.0f, 1.0f};

struct Point COLOR;

void reset() {
    N = 0;
    RESET_TIME = TIME_ELAPSED;
    TIME_ELAPSED = 0;
    SIZE_QUAD = 20;
    SIZE_POINT = 1;
    PARTICLES_PER_FR = 1;
    MAX_PARTICLES = 200;
    LIFETIME = 2;
    GRAVITY = -4.9f;
    VELOCITY.y = -1.0f;

    position[0] = 400.0f;
    position[1] = 200.0f;
    position[2] = 700.0f;
    direction[0] = 0.0f;
    direction[1] = 0.0f;
    direction[2] = -1.0f;
    up[0] = 0.0f;
    up[1] = 1.0f;
    up[2] = 0.0f;
}

void changeVelocity() {
    if (VELOCITY.y == -1.0) {
        VELOCITY.y = -1.5f;
    } else if (VELOCITY.y == -1.5f) {
        VELOCITY.y = -3.0f;
    } else if (VELOCITY.y == -3.0f) {
        VELOCITY.y = -10.0f;
    } else {
        VELOCITY.y = -1.0f;
    }
}

void changeGravity() {
    if (GRAVITY == -4.9f) {
        GRAVITY = -9.8f;
    } else if (GRAVITY == -9.8f) {
        GRAVITY = -14.7f;
    } else {
        GRAVITY = -4.9f;
    }
}

void changeParticles() {
    if (MAX_PARTICLES == 200) {
        MAX_PARTICLES = 1000;
    } else if (MAX_PARTICLES == 1000) {
        MAX_PARTICLES = 1000000;
    } else {
        MAX_PARTICLES = 200;
    }
}

void changeParticlesPerFrame() {
    if (PARTICLES_PER_FR == 100) {
        PARTICLES_PER_FR = 1;
    } else {
        PARTICLES_PER_FR *= 10;
    }
}

void changeLifetime() {
    if (LIFETIME == 2) {
        LIFETIME = 10;
        return;
    }
    if (LIFETIME == 10) {
        LIFETIME = 500;
        return;
    }
    LIFETIME = 2;
}

void changeSize() {
    if (TEXTURE) {
        if (SIZE_QUAD == 20) {
            SIZE_QUAD = 40;
        } else if (SIZE_QUAD == 40) {
            SIZE_QUAD = 70;
        } else {
            SIZE_QUAD = 20;
        }
    } else {
        if (SIZE_POINT == 1) {
            SIZE_POINT = 5;
        } else if (SIZE_POINT == 5) {
            SIZE_POINT = 20;
        } else {
            SIZE_POINT = 1;
        }
    }

}

char* decodeColor() {
    if (COLOR.x == WHITE.x && COLOR.y == WHITE.y && COLOR.z == WHITE.z)
        return "WHITE";
    if (COLOR.x == RED.x && COLOR.y == RED.y && COLOR.z == RED.z)
        return "RED";
    if (COLOR.x == GREEN.x && COLOR.y == GREEN.y && COLOR.z == GREEN.z)
        return "GREEN";
    return "BLUE";
}

void changeColor() {
    if (strcmp(decodeColor(), "WHITE") == 0) {
        COLOR = RED;
        return;
    }
    if (strcmp(decodeColor(), "RED") == 0) {
        COLOR = GREEN;
        return;
    }
    if (strcmp(decodeColor(), "GREEN") == 0) {
        COLOR = BLUE;
        return;
    }
    COLOR = WHITE;
}

void changeTexture() {
    TEXTURE = !TEXTURE;
    SIZE_POINT = 1;
    SIZE_QUAD = 20;
}

void changeHouses() {
    HOUSES = !HOUSES;
}

void writeText(float row, char *text) {
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(-1, row);
    char *c;
    for (c=text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

void mouseMove(int xpos, int ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = lastX - xpos;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction[1] = sin(glm_rad(pitch));
    direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize(direction);
}

void mousePress(int button, int state, int xpos, int ypos) {
    if(state == GLUT_UP) {
        firstMouse = true;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) exit(0);
    if (key == 32) reset();
    if (key == 'v') changeVelocity();
    if (key == 'c') changeColor();
    if (key == 's') changeSize();
    if (key == 'g') changeGravity();
    if (key == 'p') changeParticles();
    if (key == 'f') changeParticlesPerFrame();
    if (key == 'l') changeLifetime();
    if (key == 't') changeTexture();
    if (key == 'h') changeHouses();
}

void arrows(int key, int x, int y) {
    vec3 temp = (vec3){0.0f, 0.0f, 0.0f};
    float speed = 2.0f;
    if (key == GLUT_KEY_LEFT) {
        glm_vec3_cross(direction, up, temp);
        position[0] -= speed * temp[0];
        position[1] -= speed * temp[1];
        position[2] -= speed * temp[2];
        return;
    }
    if (key == GLUT_KEY_RIGHT) {
        glm_vec3_cross(direction, up, temp);
        position[0] += speed * temp[0];
        position[1] += speed * temp[1];
        position[2] += speed * temp[2];
        return;
    }
    if (key == GLUT_KEY_UP) {
        position[0] += speed * direction[0];
        position[1] += speed * direction[1];
        position[2] += speed * direction[2];
        return;
    }
    if (key == GLUT_KEY_DOWN) {
        position[0] -= speed * direction[0];
        position[1] -= speed * direction[1];
        position[2] -= speed * direction[2];
        return;
    }
}

void display2() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    char text[255];
    float row = 1.0f;
    writeText(row-=0.1, "SYSTEM:");
    // PARTICLES
        sprintf(text, "  PARTICLES: %d", N);
        writeText(row-=0.1, text);
    // FRAME RATE
        sprintf(text, "  FRAME RATE: %d fps", FRAME_RATE);
        writeText(row-=0.1, text);
    // TIME
        sprintf(text, "  TIME ELAPSED: %d ms", glutGet(GLUT_ELAPSED_TIME)-RESET_TIME);
        writeText(row-=0.1, text);


    writeText(row-=0.2, "PROPERTIES:");
    // MAX
        sprintf(text, "  (p) MAX PARTICLES: %d", MAX_PARTICLES);
        writeText(row-=0.1, text);
    // PARTICLES PER FRAME
        sprintf(text, "  (f) PARTICLES PER FRAME: %d", PARTICLES_PER_FR);
        writeText(row-=0.1, text);
    // SIZE
        if (TEXTURE) sprintf(text, "  (s) SIZE: %d", SIZE_QUAD);
        else         sprintf(text, "  (s) SIZE: %d", SIZE_POINT);
        writeText(row-=0.1, text);
    // COLOR
        sprintf(text, "  (c) COLOR: %s", decodeColor());
        writeText(row-=0.1, text);
    // LIFETIME
        sprintf(text, "  (l) LIFETIME: %d", LIFETIME*10);
        writeText(row-=0.1, text);
    // VELOCITY
        sprintf(text, "  (v) VELOCITY: %2.2f", VELOCITY.y);
        writeText(row-=0.1, text);
    // GRAVITY
        sprintf(text, "  (g) GRAVITY: %2.1f", GRAVITY*2);
        writeText(row-=0.1, text);
    // TEXTURE
        if (TEXTURE) writeText(row-=0.1, "  (t) TEXTURE: ON");
        else         writeText(row-=0.1, "  (t) TEXTURE: OFF");
    // HOUSES
        if (HOUSES) writeText(row-=0.1, "  (h) HOUSES: ON");
        else         writeText(row-=0.1, "  (h) HOUSES: OFF");

    // RESET
    writeText(row-=0.3, "RESET SYSTEM: SPACE");

    glutSwapBuffers();
    glutPostRedisplay();
}

void initControlWindow() {
    glutInitWindowSize(300, 600);
    glutInitWindowPosition(1000, 0);
    glutCreateWindow("Control");
    glutDisplayFunc(display2);
}