#include "raylib.h"
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define BALLRADIUS 15.0f
#define MAX_BALLS 100


// GLOBAL VARIABLES
float BallCountSliderValue = 1;
float GravitySliderValue = -9.8f;

typedef struct {
    float x;
    float y;
    float velocityx;
    float velocityy;
    float mass;
    float accelerationy;
    float restitution;
} Ball;

Ball balls[MAX_BALLS];
int currentBallCount = 0;
bool paused = false;


// FUNCTION DECLARATIONS (FIXED POSITION)

// Initializes all balls with default/random values
void InitBalls(Ball *balls, int count);

// Updates all balls each frame
void UpdateBalls(Ball *balls, int count, float deltaTime);

// Applies gravity to a ball
void ApplyGravity(Ball *ball, float gravity);

// Handles wall collisions
void HandleWallCollision(Ball *ball);

// Handles ball-ball collisions
void HandleBallCollisions(Ball *balls, int count);

// Draws balls
void DrawBalls(Ball *balls, int count);

// Applies damping
void ApplyDamping(Ball *ball);

// Resets simulation
void ResetSimulation(Ball *balls, int count);


// GUI FUNCTION
void drawCustomWindow(Texture GridTexture , Font FontToUse){

    DrawRectangle(0, 0, 1280, 720, GRAY);
    DrawRectangle(0, 0, 1280, 720, WHITE);

    DrawTextureRec(GridTexture, (Rectangle){0.0,0.0,874.0,716.0}, (Vector2){0.0, 0.0}, WHITE);

    DrawLineEx((Vector2){874.0,716.0},(Vector2){874.0,0.0},5.0,BLACK);

    DrawTextPro(FontToUse, "Options", (Vector2){900.0, 0.0}, (Vector2){0.0, 0.0}, 0.0, 24.0, 1.0, BLACK);

    DrawLineEx((Vector2){889.0, 20.0}, (Vector2){990.0, 20.0}, 2.0, BLACK );

    GuiSlider((Rectangle){900.0, 60.0, 100.0, 10.0},"1", "5",&BallCountSliderValue, 1.0, 5.0);

    DrawTextPro(FontToUse, TextFormat("Ball Amount: %d",(int)BallCountSliderValue),
                (Vector2){900.0, 40.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 1.0, BLACK);

    DrawTextPro(FontToUse,
        TextFormat("Gravity (Downwards accelerations): %.1f",GravitySliderValue),
        (Vector2){900.0, 100.0}, (Vector2){0.0, 0.0},
        0.0, 18.0, 1.0, BLACK);

    GuiSlider((Rectangle){900.0, 150.0, 100.0, 20.0},
              "-100", "100",
              &GravitySliderValue, -100.0f, 100.0f);

    // Pause Button
    if (GuiButton((Rectangle){900, 200, 120, 30}, paused ? "Play" : "Pause")) {
        paused = !paused;
    }

    // Reset Button
    if (GuiButton((Rectangle){900, 240, 120, 30}, "Reset")) {
        currentBallCount = (int)BallCountSliderValue;
        InitBalls(balls, currentBallCount);
    }
}


// MAIN FUNCTION
int main (){

    InitWindow(1280, 716, "PHYSICS SIMULATOR");
    SetTargetFPS(60);

    Texture GridTexture = LoadTexture("GridBG.png");
    Font FontToUse = GetFontDefault();

    while (!WindowShouldClose()){

        int desiredCount = (int)BallCountSliderValue;

        if (desiredCount != currentBallCount) {
            currentBallCount = desiredCount;
            InitBalls(balls, currentBallCount);
        }

        float dt = GetFrameTime();

        if (!paused) {
            UpdateBalls(balls, currentBallCount, dt);
            HandleBallCollisions(balls, currentBallCount);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawCustomWindow(GridTexture, FontToUse);
        DrawBalls(balls, currentBallCount);

        EndDrawing();
    }

    UnloadTexture(GridTexture);
    CloseWindow();

    return 0;
}


// System Functions
void InitBalls(Ball *balls, int count) {
    // Purpose:
    // Set initial positions, velocities, and physical properties for each ball

    for (int i = 0; i < count; i++) {
        balls[i].x = GetRandomValue(50, 800);
        balls[i].y = GetRandomValue(50, 600);

        balls[i].velocityx = GetRandomValue(-100, 100) / 50.0f;
        balls[i].velocityy = 0;

        balls[i].mass = 1.0f;
        balls[i].accelerationy = GravitySliderValue;
        balls[i].restitution = 0.8f;
    }
}

void UpdateBalls(Ball *balls, int count, float deltaTime) {
    // Purpose:
    // Update each ball's position using velocity and apply physics forces

    for (int i = 0; i < count; i++) {

        ApplyGravity(&balls[i], GravitySliderValue);

        balls[i].x += balls[i].velocityx * 100 * deltaTime;
        balls[i].y += balls[i].velocityy * 100 * deltaTime;

        ApplyDamping(&balls[i]);

        HandleWallCollision(&balls[i]);
    }
}

void ApplyGravity(Ball *ball, float gravity) {
    // Purpose:
    // Apply vertical acceleration due to gravity

    ball->accelerationy = gravity;
    ball->velocityy += ball->accelerationy * GetFrameTime();
}

void HandleWallCollision(Ball *ball) {
    // Purpose:
    // Detect and respond to collisions with screen boundaries

    // Floor
    if (ball->y + BALLRADIUS > 716) {
        ball->y = 716 - BALLRADIUS;
        ball->velocityy *= -ball->restitution;
    }

    // Ceiling
    if (ball->y - BALLRADIUS < 0) {
        ball->y = BALLRADIUS;
        ball->velocityy *= -ball->restitution;
    }

    // Left wall
    if (ball->x - BALLRADIUS < 0) {
        ball->x = BALLRADIUS;
        ball->velocityx *= -ball->restitution;
    }

    // Right wall (grid edge)
    if (ball->x + BALLRADIUS > 874) {
        ball->x = 874 - BALLRADIUS;
        ball->velocityx *= -ball->restitution;
    }
}

void HandleBallCollisions(Ball *balls, int count) {
    // Purpose:
    // Detect and resolve collisions between balls

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {

            float dx = balls[j].x - balls[i].x;
            float dy = balls[j].y - balls[i].y;

            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < BALLRADIUS * 2 && distance > 0) {

                // Normal vector
                float nx = dx / distance;
                float ny = dy / distance;

                // Relative velocity
                float vx = balls[j].velocityx - balls[i].velocityx;
                float vy = balls[j].velocityy - balls[i].velocityy;

                float dot = vx * nx + vy * ny;

                // Only resolve if moving towards each other
                if (dot > 0) continue;

                float restitution = 0.8f;

                float impulse = -(1 + restitution) * dot / 2;

                balls[i].velocityx -= impulse * nx;
                balls[i].velocityy -= impulse * ny;

                balls[j].velocityx += impulse * nx;
                balls[j].velocityy += impulse * ny;

                // Separate overlapping balls
                float overlap = (BALLRADIUS * 2 - distance) / 2;

                balls[i].x -= nx * overlap;
                balls[i].y -= ny * overlap;

                balls[j].x += nx * overlap;
                balls[j].y += ny * overlap;
            }
        }
    }
}

void DrawBalls(Ball *balls, int count) {
    // Purpose:
    // Render all balls to the screen

    for (int i = 0; i < count; i++) {
        DrawCircle((int)balls[i].x, (int)balls[i].y, BALLRADIUS, RED);
    }
}

void ApplyDamping(Ball *ball) {
    // Purpose:
    // Reduce velocity slightly over time to simulate friction

    ball->velocityx *= 0.999f;
    ball->velocityy *= 0.999f;
}

void ResetSimulation(Ball *balls, int count) {
    // Purpose:
    // Reset all balls to initial state

    InitBalls(balls, count);
}