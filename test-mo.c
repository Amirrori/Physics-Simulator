/**
 * Multi-Ball Bouncing Physics Simulator — Terminal Edition
 *
 * Prototype / backend demonstration of the physics engine.
 * Renders to the terminal using ASCII characters.
 *
 * Architecture Layers:
 * - Input Layer:    user interaction and data collection
 * - Physics Layer:  core simulation logic and physics calculations
 * - Visual Layer:   terminal rendering and trail visualisation
 * - Analysis Layer: energy calculations and system monitoring
 *
 * Changes from original prototype:
 * - Ball struct now has a radius field (per ball, user configurable)
 * - Boundary collisions use radius so balls don't clip into walls
 * - Ceiling now applies restitution for consistency with all other walls
 * - Ball to ball collision distance uses per ball radii (not hardcoded 1.0)
 * - Trail is recorded after all physics (including ball to ball) resolves
 * - GRAVITY is a mutable float so it can be changed at runtime
 * - potential_energy uses fabsf(GRAVITY) instead of hardcoded 9.8
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
// ============================================================================
// CONSTANTS & GLOBAL VARIABLES
// ============================================================================

                             
                             // UPDATE FROM MOHAMMAD: screen width and height changed according
                             // to raylib screen instead of in terminal simulation

#define TRAIL_LENGTH 10    // number of trail positions stored per ball
#define MAX_BALLS    5      // maximum number of balls allowed

float GRAVITY = 600.0f;  // acceleration due to gravity (m/s^2) — negative = downward
                        // mutable so it can be adjusted at runtime if needed
                        //
                        // UPDATE FROM MOHAMMAD: In raylib gravity has to be reversed
                        // and increased because negative means upwards.
                        // Can stay mutable aswell.

int trail_x[MAX_BALLS][TRAIL_LENGTH]; // X-coordinates of trail positions per ball
int trail_y[MAX_BALLS][TRAIL_LENGTH]; // Y-coordinates of trail positions per ball
int trail_index[MAX_BALLS];           // current write index in the circular trail buffer

int num_balls; // total num of balls in the simulation

// ============================================================================
// DATA STRUCTURES
// ============================================================================

/**
 * represents a single ball in the simulation.
 * positions and velocities are in metres and metres/second.
 * forces accumulate each frame and are reset after integration.
 */
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

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

// Input layer
void input_speed_angle(Ball *b);
void input_velocity_components(Ball *b);
void get_user_inputs(Ball balls[]);

// Physics layer
void apply_gravity(Ball *b);
void update(Ball *b, float dt);
void check_collision(Ball *b);
void check_ball_collisions(Ball balls[], int num_balls);
void simulation_step(Ball balls[], int num_balls, float dt);

// Visual layer
void update_trail(Ball *b, int ball_index);
void draw(Ball balls[], int num_balls);

// Analysis layer
float kinetic_energy(Ball *b);
float potential_energy(Ball *b);
float total_energy(Ball *b);

// // ============================================================================
// // INPUT LAYER
// // ============================================================================
//
// /**
//  * asks the user for launch speed and angle for a single ball.
//  * converts speed and angle into vx and vy using trig:
//  *     velocityx = speed * cos(angle)
//  *     velocityy = speed * sin(angle)
//  * lets the rest of the simulation work entirely with velocity vectors.
//  */
// void input_speed_angle(Ball *b) {
//     float speed, angle_deg;
//     printf("Enter launch speed (m/s): ");
//     scanf("%f", &speed);
//     printf("Enter launch angle (degrees): ");
//     scanf("%f", &angle_deg);
//
//     float angle_rad = angle_deg * M_PI / 180.0f;
//     b->velocityx = speed * cosf(angle_rad);
//     b->velocityy = speed * sinf(angle_rad);
//
//     printf("Calculated vx = %.2f m/s\n", b->velocityx);
//     printf("Calculated vy = %.2f m/s\n", b->velocityy);
// }
//
// /**
//  * asks user to input velocity components directly:
//  *   - velocityx: horizontal speed (m/s)
//  *   - velocityy: vertical speed (m/s)
//  * useful for directly defining a motion vector without converting from speed/angle.
//  */
// void input_velocity_components(Ball *b) {
//     printf("Enter velocity x (m/s): ");
//     scanf("%f", &b->velocityx);
//     printf("Enter velocity y (m/s): ");
//     scanf("%f", &b->velocityy);
// }
//
// /**
//  * handles users input for a multi ball simulation:
//  *   - asks for number of balls (clamped to 1 MAX_BALLS)
//  *   - for each ball: velocity (speed/angle or vx/vy), mass, restitution, radius
//  *   - sets initial position at the centre of the box with a small offset per ball
//  *   - clamps radius to prevent balls larger than a quarter of the box height
//  */
// void get_user_inputs(Ball balls[]) {
//     printf("Enter number of balls (1-%d): ", MAX_BALLS);
//     scanf("%d", &num_balls);
//
//     if (num_balls < 1)        num_balls = 1;
//     if (num_balls > MAX_BALLS) num_balls = MAX_BALLS;
//
//     for (int i = 0; i < num_balls; i++) {
//         printf("\n    Ball %d    \n", i + 1);
//
//         int choice;
//         printf("Choose input type:\n");
//         printf("  1) Speed and Angle (GCSE style)\n");
//         printf("  2) Velocity Components vx, vy (vector form)\n");
//         printf("Enter choice: ");
//         scanf("%d", &choice);
//
//         if      (choice == 1) input_speed_angle(&balls[i]);
//         else if (choice == 2) input_velocity_components(&balls[i]);
//         else {
//             printf("Invalid choice. Defaulting to speed/angle.\n");
//             input_speed_angle(&balls[i]);
//         }
//
//         printf("Enter mass (kg): ");
//         scanf("%f", &balls[i].mass);
//
//         printf("Enter coefficient of restitution (0-1): ");
//         scanf("%f", &balls[i].restitution);
//
//         printf("Enter ball radius (metres): ");
//         scanf("%f", &balls[i].radius);
//
//         // clamp radius to a "sensible" max
//         float max_radius = HEIGHT / 4.0f;
//         if (balls[i].radius > max_radius) {
//             printf("Radius too large — clamped to %.2f\n", max_radius);
//             balls[i].radius = max_radius;
//         }
//         if (balls[i].radius <= 0) {
//             printf("Invalid radius — defaulting to 0.5\n");
//             balls[i].radius = 0.5f;
//         }
//
//         // place each ball in the centre with small horizontal offset
//         balls[i].x = WIDTH  / 2.0f + i * 2.0f;
//         balls[i].y = HEIGHT / 2.0f;
//     }
// }
//
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
 * Resolves collisions between every pair of balls.
 *
 * For each overlapping pair:
 *   1. Compute the collision normal from centre to centre direction.
 *   2. Calc the relative velocity along that normal.
 *   3. apply the impulse formula derived from conservation of momentum:
 *          p = 2 * dot(v_rel, n) / (m_i + m_j)
 *      and update both velocities accordingly.
 *   4. apply a position correction to push the balls apart by the overlap
 *      amount, preventing tunnelling across multiple frames.
 *
 * min distance uses per ball radii so differently sized balls collide
 * at the right distance rather than a hardcoded value.
 */
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


    for (int i = 0; i < num_balls; i++)
        update_trail(&balls[i], i);
}

// ============================================================================
// VISUAL LAYER
// ============================================================================

/**
 * Records the balls current position into its circular trail buffer.
 * The write index wraps around once it reaches TRAIL_LENGTH,
 * overwriting the oldest position automatically.
 */
void update_trail(Ball *b, int ball_index) {
    trail_x[ball_index][trail_index[ball_index]] = (int)b->x;
    trail_y[ball_index][trail_index[ball_index]] = (int)b->y;
    trail_index[ball_index]++;
    if (trail_index[ball_index] >= TRAIL_LENGTH)
        trail_index[ball_index] = 0;
}

/**
 * Renders the current simulation state to the terminal.
 *
 * Scans every cell of the grid from top to bottom, left to right:
 *   'O' — drawn at the balls current position
 *   '.' — drawn at any cell that appears in a balls trail buffer
 *   '-' — drawn along the floor (y = 0)
 *   '|' — drawn along the left and right walls
 *   ' ' — empty space
 *
 * clears the terminal each frame using system("cls") (Windows only)
 * so the animation appears in place rather than scrolling.
 */
// ============================================================================
// ANALYSIS LAYER
// ============================================================================

/**
 * Returns the kinetic energy of a ball:
 *     KE = 0.5 * m * (vx^2 + vy^2)
 */
float kinetic_energy(Ball *b) {
    return 0.5f * b->mass * (b->velocityx*b->velocityx + b->velocityy*b->velocityy);
}

/**
 * Returns the gravitational potential energy of a ball:
 *     PE = m * |g| * y
 * Uses fabsf(GRAVITY) so PE is always positive regardless of GRAVITIES sign.
 */
float potential_energy(Ball *b) {
    return b->mass * fabsf(GRAVITY) * b->y;
}

/**
 * Returns the total mechanical energy of a ball:
 *     TE = KE + PE
 * Useful for verifying energy conservation — total should decrease
 * gradually due to restitution losses and stay stable between bounces.
 */
float total_energy(Ball *b) {
    return kinetic_energy(b) + potential_energy(b);
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

/**
 * Entry point for the terminal physics simulator.
 *
 * Responsibilities:
 *   1. Initialise ball objects to zero.
 *   2. Collect user inputs (ball count, velocities, mass, restitution, radius).
 *   3. Configure simulation timing (dt and total duration).
 *   4. Run the main simulation loop.
 *   5. Display energy values each frame.
 *
 * Simulation loop (per frame):
 *   - Advance physics for all balls (gravity, integration, collisions)
 *   - Redraw the terminal scene
 *   - Print kinetic, potential and total energy
 *   - Sleep briefly so the animation is visible at a comfortable speed
 *
 * The simulation runs until max_frames is reached, then exits cleanly.
 */
