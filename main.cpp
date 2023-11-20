#include "RayApp.h"


int main() {
    RayApp app = RayApp();
    app.setup();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        app.update();
        BeginDrawing();
        app.draw();
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
