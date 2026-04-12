// Import raylib
#include "./external/raylib-5.5/src/raylib.h"

// Import required C libraries
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


// Import global variables
#include "globals.h"

// TODO
// Modify and implement physics backend so it works for the frontend.


//=========================================
// START OF BACKEND FUNCTIONS FROM IBRAHEEM
//=========================================

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

void apply_gravity(Ball *b);
void update(Ball *b, float dt);
void check_collision(Ball *b);
void check_ball_collisions(Ball balls[], int num_balls);
void simulation_step(Ball balls[], int num_balls, float dt);

// ============================================================================
// PHYSICS LAYER
// ============================================================================

/**
 * accumulates gravity force onto a ball using newtons 2nd law:
 *     F = mg (g is just the acceleration)
 * keeping force accumulation separate means other forces (drag, wind, etc)
 * can be added here in future without changing the integrator.
 */
void apply_gravity(Ball *b) {
    b->forcey += b->mass * GRAVITY;
}

/**
 * integrates forces into velocity and position using integration:
 *     a  = F / m
 *     v += a * dt
 *     x += v * dt
 * forces are reset to zero after integration, ready for the next frame.
 */
void update(Ball *b, float dt) {
    float ax = b->forcex / b->mass;
    float ay = b->forcey / b->mass;

    b->velocityx += ax * dt;
    b->velocityy += ay * dt;
    b->x         += b->velocityx * dt;
    b->y         += b->velocityy * dt;

    b->forcex = 0;
    b->forcey = 0;
}

/**
 * resolves collisions between a ball and walls.
 *
 * each wall clamps the ball's position to keep it inside the box, then
 * reverses and scales the relevant velocity component by restitution.
 *
 * on the floor, very small vertical velocities are zeroed to prevent
 * infinite low energy jitter from rounding errors.
 */
void check_collision(Ball *b) {
    // LEFT WALL
    if (b->x <= b->radius) {
        b->x         = b->radius;
        b->velocityx = -b->velocityx * b->restitution;
    }

    // RIGHT WALL
    if (b->x >= screenWidth - b->radius) {
        b->x         = screenWidth - b->radius;
        b->velocityx = -b->velocityx * b->restitution;
    }

    // FLOOR
    if (b->y <= b->radius) {
        b->y         = b->radius;
        b->velocityy = -b->velocityy * b->restitution;

        if (fabs(b->velocityy) < 0.1f)
            b->velocityy = 0;
    }

    // CEILING
    if (b->y >= screenHeight - b->radius) {
        b->y         = screenHeight - b->radius;
        b->velocityy = -b->velocityy * b->restitution;
    }
}

/**
 * Advances the simulation by one timestep for all balls:
 *   1. apply gravity to accumulate force.
 *   2. integrate forces into velocity and position.
 *   3. resolve wall and ceiling collisions.
 * Then, once all balls are updated, resolve ball to ball collisions
 * and record trail positions with the final post-collision state.
 *
 * trail recording happens last so the stored positions always reflect
 * the true final position after all physics has been applied.
 */
void simulation_step(Ball balls[], int num_balls, float dt) {
    for (int i = 0; i < num_balls; i++) {
        apply_gravity(&balls[i]);
        update(&balls[i], dt);
        check_collision(&balls[i]);
    }
}

//=========================================
// BACKEND INPUT FUNCTIONS
//=========================================
/**
 * asks users for launch speed and angle for a single ball
 * converts speed and angle into x and y velocity vector componens using trig:
 *      velocityx = speed * cos(angle)
 *      velocityy = speed * sin(angle)
 * this allows the simulation to work with vector-based velocities
 */

void input_speed_angle(Ball *b) {
    float speed, angle_deg;
    printf("Enter launch speed (m/s): ");
    scanf("%f", &speed);
    printf("Enter launch angle (degrees): ");
    scanf("%f", &angle_deg);

    float angle_rad = angle_deg * M_PI / 180.0;
    b->velocityx = speed * cos(angle_rad);
    b->velocityy = speed * sin(angle_rad);

    printf("Calculated vx = %.2f m/s\n", b->velocityx);
    printf("Calculated vy = %.2f m/s\n", b->velocityy);
}


/**
 * asks user to input velocity components directly:
 * - velocityx: horizontal speed
 * - velocityy: vertical speed
 * allows users to skip the speed/angle step and directly define motion vector.
 */
void input_velocity_components(Ball *b) {
    printf("Enter velocity x (m/s): ");
    scanf("%f", &b->velocityx);

    printf("Enter velocity y (m/s): ");
    scanf("%f", &b->velocityy);
}



void get_user_inputs(Ball balls[]) {

    num_balls = 1;


    for(int i = 0; i < num_balls; i++) {
        printf("\n Ball %d \n", i + 1);
        int choice;
        printf("Choose input type:\n1) Speed and Angle (GCSE style)\n2) Velocity Components vx, vy (Vector form)\nEnter choice: ");
        scanf("%d", &choice);
        if(choice == 1) input_speed_angle(&balls[i]);
        else if(choice == 2) input_velocity_components(&balls[i]);
        else {
            printf("Invalid choice. Defaulting to speed/angle.\n");
            input_speed_angle(&balls[i]);
        }

        printf("Enter mass (kg): ");
        scanf("%f", &balls[i].mass);
    }
}


//======================================
//END OF BACKEND FUNCTIONS FROM IBRAHEEM
//======================================



// INITIALIZATION
void initialization() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib frontend window - Physics Simulator");
    
    SetTargetFPS(currentFps);
}


int main(void) {

    initialization();

    // Initialise ball object
    Ball balls[MAX_BALLS] = {0};
    
    // Run input collection from backend
    get_user_inputs(balls);
    
    // Restrict restitution to 0.8
    balls[0].restitution = 0.8f;
    
    
    // Initialise and get input of total time from the user
    float total_time;

    printf("Enter total simulation time (seconds): ");
    scanf("%f", &total_time);


    // Print out all the values collected for debugging purposes
    for (int i = 0; i < num_balls; i++) {
        printf("BALL %d: \n", i);
        printf("vx = %.2f\n", balls[i].velocityx);
        printf("vy = %.2f\n", balls[i].velocityy);
        printf("mass = %.2f\n", balls[i].mass);
    }
    
    // Set initial positions of ball
    // balls[0].x = 0;
    // balls[0].y = (float)screenHeight/3.0f;
    balls[0].x = 30;
    balls[0].y = 120;
    
    // save ball information into another object
    balls[1] = balls[0];

// GetFrameTime() returns the time it took to draw the last frame, in seconds (usually called delta time)
// Gets raylibs current frame time as a measurement instead of doing it ourself
// This fixes the bug which makes the ball change in velocity when framerate is changed.
    // Main Simulation loop
    //
    // TODO end the loop if running time reaches the total_time DONE

    // Set elapsed time
    float onGoingFrameTime = 0.0f;
    
    // Main loop to do calculations and draw the ball
    while (!WindowShouldClose() && onGoingFrameTime < total_time) // Detect window close button or ESC key AND check if time is up or not.
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // CODE BEFORE IMPLEMENTING PHYSICS SIMULATOR

        // Multiply by 6.0 (an arbitrary value) in order to make the speed
        // visually closer to the other circle (at 60 fps), for comparison
        //
        //
        // deltaCircle.x += GetFrameTime()*6.0f*speedx;
        // deltaCircle.y += GetFrameTime()*-6.0f*speedy;
        //
        //
        
        // dt means change in time and were using the raylib frame time instead of a arbitrary value.
        
        

        // Reset the ball when the key "R" is pressed
        if (IsKeyPressed(82)) {
            balls[0] = balls[1];
        }


        // BUG huge spike when app starts results in ball skyrocketing
        float dt = GetFrameTime();
        // FIX
        if (dt > 0.05f) dt = 0.05f;

        // printf("dt = %f\n", dt);


        // Does all the position maths with raylib frame times and uses raylib geometry
        simulation_step(balls, num_balls, dt);
        
        // if circle hits any of the borders they reset
        // Left wall
        if (balls[0].x - 20 < 0)
        {
            balls[0].x = 20;
            balls[0].velocityx *= -balls[0].restitution;
        }

        // right wall
        if (balls[0].x +20 > screenWidth)
        {
            balls[0].x = screenWidth - 20;
            balls[0].velocityx *= -balls[0].restitution;

        }

        // Bottom wall
        if (balls[0].y + 20 > screenHeight)
        {
            balls[0].y = screenHeight - 20;
            balls[0].velocityy *= -balls[0].restitution;

            if (balls[0].y < 0.1f)
                balls[0].velocityy = 0;
        }

        // Top wall
        if (balls[0].y - 20 < 0)
        {
            balls[0].y = 20;
            balls[0].velocityy *= -balls[0].restitution;
        }

        // Draw
        // ------------------------------------------------ //
        BeginDrawing();
            // Set background colour
            ClearBackground(GRAY);
            // Draw circle with position x(gets it from ball object), y and set a radius of 20(used 20 because the backend's is too small for raylib). and the colour of RED
            DrawCircle(balls[0].x, balls[0].y, 20, RED);
            


            // frame circle bad because it is relatively affected by the framerate
            // DrawCircleV(frameCircle, circleRadius, BLUE);

            // Draw the help texts/fps text/misc
            // Determine what help text to show depending on the current FPS target
            const char *fpsText = 0;
            if (currentFps <= 0) fpsText = TextFormat("FPS: unlimited (%i)", GetFPS());
            else fpsText = TextFormat("FPS: %i (target: %i)", GetFPS(), currentFps);
            DrawText(fpsText, 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime()), 10, 30, 20, DARKGRAY);
            DrawText(TextFormat("Time elapsed: %02.02f s", onGoingFrameTime), 10, 50, 20, DARKGRAY);
            // Draw the text above the circle
            DrawText("FUNC: Using GetFrameTime() as metric for each frame.", 10, 90, 20, RED);
            DrawRectangleLines(0, 0, screenWidth, screenHeight, BLACK);
            
            // Misc
            DrawText(TextFormat("X: %02.02f, Y: %02.02f", balls[0].x, balls[0].y), 10, 70, 20, DARKGRAY);

        

            // Calculate time
            // ----------------------------- //
            float timeNow = GetFrameTime();
            onGoingFrameTime += timeNow;

        EndDrawing();
        // ------------------------------------------------ //
    }
    // De-initilization
    // --------------------------------------------- //
    CloseWindow();
    // --------------------------------------------- //
    

    return 0;
}
