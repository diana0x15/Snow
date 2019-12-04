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
    TIME_ELAPSED = 0;
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
    // if (VELOCITY == 0.0) {
    //     VELOCITY = -0.5;
    // } else if (VELOCITY == -0.5) {
    //     VELOCITY = -1.0;
    // } else if (VELOCITY == -1.0){
    //   VELOCITY = -4.0;
    // } else {
    //   VELOCITY = 0.0;
    // }
}

void changeParticles() {
    if (MAX_PARTICLES == 1000000000) {
        MAX_PARTICLES = 100;
    } else {
        MAX_PARTICLES *= 10;
    }
}

void changeLifetime() {
    if (LIFETIME == 1) {
        LIFETIME = 2;
        return;
    }
    if (LIFETIME == 2) {
        LIFETIME = 6;
        return;
    }
    if (LIFETIME == 6) {
        LIFETIME = 30;
        return;
    }
    LIFETIME = 1;
}

void changeSize() {
     if (SIZE == 20) {
         SIZE = 40;
     } else if (SIZE == 40) {
         SIZE = 70;
     } else {
         SIZE = 20;
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
    if (key == 'p') changeParticles();
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
    sprintf(text, "PARTICLES: %d", N);
    writeText(0.9, text);
    sprintf(text, "FRAME RATE: %2.2f", FRAME_RATE);
    writeText(0.8, text);
    sprintf(text, "SNOWFLAKE SIZE: %d", SIZE);
    writeText(0.7, text);
    sprintf(text, "SNOWFLAKE COLOR: %s", decodeColor());
    writeText(0.6, text);
    sprintf(text, "NUMBER OF PARTICLES: %d", MAX_PARTICLES);
    writeText(0.5, text);
    sprintf(text, "LIFETIME: %d", LIFETIME*10);
    writeText(0.4, text);
    if (TEXTURE) writeText(0.3, "TEXTURE: ON");
    else         writeText(0.3, "TEXTURE: OFF");

    glutSwapBuffers();
    glutPostRedisplay();
}

void initControlWindow() {
    glutInitWindowSize(300, 600);
    glutInitWindowPosition(1000, 0);
    glutCreateWindow("Control");
    glutDisplayFunc(display2);
}