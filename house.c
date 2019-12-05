
#include <GLUT/glut.h>

GLuint house_texture;

short HOUSE_PLAN[800][800];

int house1_E = 700;
int house1_S = 200;
int house1_V = 500;
int house1_N = 100;
int house1_H = 100;

int house2_E = 200;
int house2_S = 300;
int house2_V = 100;
int house2_N = 200;
int house2_H = 60;

void loadHouseTexture() {
    house_texture = SOIL_load_OGL_texture(
            "../brick2.jpeg",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y);
}

void initHousePlan() {
    for(int i = 0; i < 800; ++i) {
        for(int j = 0; j < 800; ++j) {
            HOUSE_PLAN[i][j] = 0;
            if (i >= house1_V && i <= house1_E && j >= house1_N && j <= house1_S) {
                HOUSE_PLAN[i][j] = 1;
            }
            if (i >= house2_V && i <= house2_E && j >= house2_N && j <= house2_S) {
                HOUSE_PLAN[i][j] = 2;
            }
        }
    }
}

void draw_house(int house_E, int house_S, int house_V, int house_N, int house_H) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, house_texture);

    // BACK
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  1.0,  1.0 );
    glTexCoord2f(0.0, 1.0);
    glVertex3f(  house_E, 0.0f, house_N );
    glTexCoord2f(0.0, 0.0);
    glVertex3f(  house_E,  house_H, house_N );
    glTexCoord2f(1.0, 0.0);
    glVertex3f( house_V,  house_H, house_N );
    glTexCoord2f(1.0, 1.0);
    glVertex3f( house_V, 0.0f, house_N );
    glEnd();

    // RIGHT
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  1.0,  1.0 );
    glTexCoord2f(0.0, 1.0);
    glVertex3f( house_E, 0.0f, house_N );
    glTexCoord2f(0.0, 0.0);
    glVertex3f( house_E,  house_H, house_N );
    glTexCoord2f(1.0, 0.0);
    glVertex3f( house_E,  house_H,  house_S );
    glTexCoord2f(1.0, 1.0);
    glVertex3f( house_E, 0.0f,  house_S );
    glEnd();

    // LEFT
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  1.0,  1.0 );
    glTexCoord2f(0.0, 1.0);
    glVertex3f( house_V, 0.0f,  house_S );
    glTexCoord2f(0.0, 0.0);
    glVertex3f( house_V,  house_H,  house_S );
    glTexCoord2f(1.0, 0.0);
    glVertex3f( house_V,  house_H, house_N );
    glTexCoord2f(1.0, 1.0);
    glVertex3f( house_V, 0.0f, house_N );
    glEnd();

    // FRONT
    glBegin(GL_POLYGON);
    glColor3f(1.0,1.0,1.0);
    glTexCoord2f(0.0,1.0);
    glVertex3f(house_E,0.0f, house_S);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(house_E, house_H, house_S);
    glTexCoord2f(1.0,0.0);
    glVertex3f(house_V, house_H, house_S);
    glTexCoord2f(1.0,1.0);
    glVertex3f(house_V,0.0f, house_S );
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // TOP
    glBegin(GL_POLYGON);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(house_E, house_H, house_S);
    glVertex3f(house_V, house_H, house_S);
    glVertex3f(house_V, house_H, house_N);
    glVertex3f(house_E, house_H, house_N);
    glEnd();
}

