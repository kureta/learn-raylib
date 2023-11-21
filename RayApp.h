//
// Created by kureta on 11/20/23.
//

#ifndef RAYAPP_H
#define RAYAPP_H

#include "raylib.h"
#include <torch/script.h>


class RayApp
{
public:
    void setup();
    void update(double t, double dt);
    void draw() const;

private:
    bool pause = false;
    int framesCounter = 0;
    torch::jit::script::Module module;
    std::vector<torch::jit::IValue> inputs;
    torch::Tensor z;
    torch::Tensor label;
    const torch::Tensor trunc = torch::ones(1) * 1.0f;
    torch::Tensor output;
    Image img;
    float phases[128];
};


#endif //RAYAPP_H
