/*
As of now for debugging purposes, pressing SPACE while the ball
is moving will stop it, and pressing it again will resume

It keeps the same speed as before
*/

#include "raylib.h"

// Ball structure
typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    Vector2 savedSpeed; // Store speed when paused
    float radius;
    Color color;
    bool moving;        // Is the ball currently moving???
} Ball;

// Function to draw the ball
void DrawBall(Ball *ball) {
    DrawCircleV(ball->position, ball->radius, ball->color);
}

// Function to move the ball if it's moving
void MoveBall(Ball *ball, float screenWidth, float screenHeight) {
    if (!ball->moving) return; // Don't move if paused

    float dt = GetFrameTime();

    // Update position
    ball->position.x += ball->speed.x * dt;
    ball->position.y += ball->speed.y * dt;

    // Bounce off walls
    if ((ball->position.x - ball->radius < 0) || (ball->position.x + ball->radius > screenWidth))
        ball->speed.x *= -1;
    if ((ball->position.y - ball->radius < 0) || (ball->position.y + ball->radius > screenHeight))
        ball->speed.y *= -1;
}

// Function to start moving the ball
void StartBall(Ball *ball, Vector2 initialSpeed) {
    ball->speed = initialSpeed;
    ball->moving = true;
}

// Function to stop the ball
void StopBall(Ball *ball) {
    ball->savedSpeed = ball->speed; // Save current speed
    ball->speed = (Vector2){0, 0};  // Set speed to 0
    ball->moving = false;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Physics Simulator");
    SetTargetFPS(60);

    // Initialize a ball at a starting position with zero speed
    Ball myBall = { {400, 300}, {0, 0}, {0, 0}, 30, MAROON, false };

    // Start the ball moving with a specific speed
    StartBall(&myBall, (Vector2){200, 150});

    while (!WindowShouldClose()) {

        // Sample code to stop moving the ball if space bar is pressed
        if (IsKeyPressed(KEY_SPACE)) {
            if (myBall.moving)
                StopBall(&myBall);
            else
                StartBall(&myBall, myBall.savedSpeed); // Resume with previous speed
        }

        // Update ball position
        MoveBall(&myBall, screenWidth, screenHeight);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBall(&myBall);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}