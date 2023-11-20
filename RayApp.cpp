#include "RayApp.h"

void RayApp::setup() {
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - bouncing ball");

    this->ballPosition = {
        static_cast<float>(GetScreenWidth()) / 2.0f,
        static_cast<float>(GetScreenHeight()) / 2.0f
    };
    this->ballSpeed = {10.0f, 8.0f};
    this->ballRadius = 20.0f;

    this->pause = false;
    this->framesCounter = 0;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
}

void RayApp::update() {
    if (IsKeyPressed(KEY_SPACE)) pause = !pause;

    if (!pause) {
        this->ballPosition.x += this->ballSpeed.x;
        this->ballPosition.y += this->ballSpeed.y;

        // Check walls collision for bouncing
        if (this->ballPosition.x >= static_cast<float>(GetScreenWidth()) - this->ballRadius
            || this->ballPosition.x <= this->ballRadius)
            this->ballSpeed.x *= -1.0f;
        if (this->ballPosition.y >= static_cast<float>(GetScreenHeight()) - this->ballRadius
            || this->ballPosition.y <= this->ballRadius)
            this->ballSpeed.y *= -1.0f;
    }
    else framesCounter++;
}

void RayApp::draw() const {
    ClearBackground(RAYWHITE);

    DrawCircleV(this->ballPosition, this->ballRadius, MAROON);
    DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25, 20, LIGHTGRAY);

    // On pause, we draw a blinking message
    if (this->pause && this->framesCounter / 30 % 2)
        DrawText("PAUSED", 350, 200, 30, GRAY);

    DrawFPS(10, 10);
}
