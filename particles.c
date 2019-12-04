//
// Created by Diana Pislaru on 03/12/2019.
//

float GRAVITY = -4.9;   //9.8 * 0.5
struct Point VELOCITY = {0.0, -1.0, 0.0};

struct Particle {
    struct Location pos;
    struct Point prevPos;
    struct Point velocity;
    struct Point acceleration;
    struct Point color;
    float size;
    float time;
    int type;
};



