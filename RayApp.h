//
// Created by kureta on 11/20/23.
//

#ifndef RAYAPP_H
#define RAYAPP_H

#include "raylib.h"


class RayApp {
public:
    void setup();
    void update();
    void draw() const;

private:
    Vector2 ballPosition = {};
    float ballRadius = 0.0f;
    Vector2 ballSpeed = {};
    bool pause = false;
    int framesCounter = 0;
};


#endif //RAYAPP_H
