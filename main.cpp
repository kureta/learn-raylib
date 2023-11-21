#include "RayApp.h"


double t = 0.0;
double dt = 0.05;

double currentTime = 0.0;
double accumulator = 0.0;


int main()
{
    RayApp app = RayApp();
    app.setup();

    currentTime = GetTime();
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        const double newTime = GetTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25)
            frameTime = 0.25;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= dt)
        {
            app.update(t, dt);
            accumulator -= dt;
            t += dt;
        }

        BeginDrawing();
        app.draw();
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
