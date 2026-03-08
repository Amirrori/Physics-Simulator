#include "raylib.h"
#include <stdio.h>


const float BALLRADIUS = 15.0; //All balls with have the same radius

// class for the ball
typedef struct {
    float x;
    float y;
    float velocityx;
    float velocityy; 
    float mass;
    float accelerationy; // acceleration in the y direction only because this will allow us to change the gravity
    float restitution; // NEW
} Ball;

int ArrayLength(Ball Array[]){
    
    int size1 = sizeof(Array);
    unsigned int newsize = size1/sizeof(Array[0]);
    printf("The size of the array is %d", newsize);
    return newsize;
};

void DrawBalls(Ball Balls[]){
    int i ;
    for (i = 0; i <= ArrayLength(Balls); i++){
        DrawCircle(Balls[i].x, Balls[i].y, BALLRADIUS, RED);
    };
};



int main (){

    const int SCREENWIDTH = 800;
    const int SCREENHEIGHT = 600;

    //Create window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Yo World");

    //Set frames per second of the window
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        //Open the drawing buffer
        BeginDrawing();

        //Set the background to white to avoid tearing and other issues
        ClearBackground(RAYWHITE);


        DrawText("WASSUP", 400, 0, 12, BLACK);

        Ball Balls[2] = {{10,10,0,0,0,0,0}, {20,20,0,0,0,0,0}};

        DrawBalls(Balls);

        EndDrawing();
    };
    return 0;
};
