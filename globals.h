#ifndef GLOBALS_H
#define GLOBALS_H

// UPDATE FROM MOHAMMAD: screen width and height changed according
// to raylib screen instead of in terminal simulation


// ============================================================================
// CONSTANTS & GLOBAL VARIABLES
// ============================================================================

#define TRAIL_LENGTH 10    // number of trail positions stored per ball
#define MAX_BALLS    5      // maximum number of balls allowed



int trail_x[MAX_BALLS][TRAIL_LENGTH]; // X-coordinates of trail positions per ball
int trail_y[MAX_BALLS][TRAIL_LENGTH]; // Y-coordinates of trail positions per ball
int trail_index[MAX_BALLS];           // current write index in the circular trail buffer

int num_balls; // total num of balls in the simulation


int screenWidth = 800;
int screenHeight = 600;
int currentFps = 60;

float GRAVITY = 200.0f;  // acceleration due to gravity (m/s^2) — negative = downward
                        // mutable so it can be adjusted at runtime if needed
                        //
                        // UPDATE FROM MOHAMMAD: In raylib gravity has to be reversed
                        // and increased because negative means upwards.
                        // Can stay mutable aswell.

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct {
    float x;           // Current X position (m)
    float y;           // Current Y position (m)
    float velocityx;   // Velocity in X direction (m/s)
    float velocityy;   // Velocity in Y direction (m/s)
    float mass;        // Mass (kg)
    float forcex;      // Accumulated force in X direction (N) — reset each frame
    float forcey;      // Accumulated force in Y direction (N) — reset each frame
    float restitution; // Coefficient of restitution (0 = no bounce, 1 = perfect bounce)
    float radius;      // Radius of the ball (m) — affects collisions and boundary clamping
} Ball;

#endif
