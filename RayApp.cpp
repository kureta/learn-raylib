#include "RayApp.h"

float t = 0.0f;
int idx = 0;
float mult = 1.0f;


void RayApp::setup()
{
    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - bouncing ball");

    this->pause = false;
    this->framesCounter = 0;

    // pytorch stuff
    try
    {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        this->module = torch::jit::load("/home/kureta/Documents/repos/compile-biggan/biggan-deep-256.pt");
    }
    catch (const c10::Error& e)
    {
        std::cerr << "error loading the model\n";
        std::cerr << e.msg();
    }

    std::cout << "ok\n";

    this->z = torch::zeros({1, 128}, at::TensorOptions(c10::Device(at::kCUDA, 0)));
    this->label = torch::zeros({1, 1000}, at::TensorOptions(c10::Device(at::kCUDA, 0)));
    this->label[0][idx] = 1.0f;
    inputs.emplace_back(this->z);
    inputs.emplace_back(this->label);
    inputs.emplace_back(this->trunc);

    this->img.width = 256;
    this->img.height = 256;
    this->img.mipmaps = 1;
    this->img.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32;

    const int *vals = LoadRandomSequence(128, -314159265, 314159265);
    for (int i = 0; i < 128; i++)
    {
        this->phases[i] = static_cast<float>(vals[i]) / 100000000.0f;
    }
}

void RayApp::update()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        pause = !pause;
    }
    else if (IsKeyPressed(KEY_L))
    {
        this->label[0][idx] = 0.0f;
        idx = (idx + 1) % 1000;
        this->label[0][idx] = 1.0f;
    }
    else if (IsKeyPressed(KEY_K))
    {
        this->label[0][idx] = 0.0f;
        idx = (idx - 1) % 1000;
        this->label[0][idx] = 1.0f;
    }
    else if (IsKeyPressed(KEY_P))
    {
        const int *vals = LoadRandomSequence(128, -314159265, 314159265);
        for (int i = 0; i < 128; i++)
        {
            this->phases[i] = static_cast<float>(vals[i]) / 100000000.0f;
        }
    }
    else if (IsKeyPressed(KEY_Q))
    {
        mult = 0.0f;
    }
    else if (IsKeyPressed(KEY_W))
    {
        mult += 0.1f;
    }

    if (!pause)
    {
        {
            torch::NoGradGuard no_grad;
            this->output = this->module.forward(inputs).toTensor();
            this->output = (this->output + 1.0f) / 2.0f;
            this->img.data = this->output[0].permute({1, 2, 0}).contiguous().to(torch::kCPU).data_ptr();
        }

        for (int i = 0; i < 128; i++)
        {
            this->z[0][i] = std::cos(t + this->phases[i]) * mult;
        }
        t += 0.03;
    }
    else framesCounter++;
}

void RayApp::draw() const
{
    ClearBackground(RAYWHITE);
    DrawText("PRESS SPACE to PAUSE", 10, GetScreenHeight() - 25, 20, LIGHTGRAY);

    const Texture2D texture = LoadTextureFromImage(this->img);
    DrawTextureEx(texture, {256, 128}, 0.0f, 2.0f, WHITE);

    // On pause, we draw a blinking message
    if (this->pause && this->framesCounter / 30 % 2)
        DrawText("PAUSED", 350, 200, 30, GRAY);

    DrawFPS(10, 10);
}
