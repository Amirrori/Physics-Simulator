#include "raylib.h"
#include <stdio.h>


const float BALLRADIUS = 15.0; //All balls with have the same radius
const int SCREENWIDTH = 1280;
const int SCREENHEIGHT = 720;

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
    for (i = 0; i <= ArrayLength(Balls)+1; i++){
        DrawCircle(Balls[i].x, Balls[i].y, BALLRADIUS, RED);
        printf("Drew ball");
    };
};

void drawCustomWindow(){

    //Drawing the UI for the simulator

    char GridFileName[] = "GridBG.png";

    

    DrawRectangle(0.0, 0.0, SCREENWIDTH,SCREENHEIGHT, GRAY); //Draw the grey background
    DrawRectangle(SCREENWIDTH*0.1,SCREENHEIGHT*0.1, SCREENWIDTH,SCREENHEIGHT/1.15, WHITE); //White main area
    Texture GridTexture = LoadTexture(GridFileName); // Define grid texture
    DrawTextureEx(GridTexture, (Vector2){SCREENWIDTH*0.1, SCREENHEIGHT*0.1}, 0.0, 0.8, WHITE); // Render the grid

}

int main (){

    //Create window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Yo World");

    //Set frames per second of the window
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        //Open the drawing buffer
        BeginDrawing();

        //Set the background to white to avoid tearing and other issues
        ClearBackground(RAYWHITE);


        //DrawText("WASSUP", 400, 0, 12, BLACK);

        // Ball Balls[2] = {{500,500,0,0,0,0,0}, {200,200,0,0,0,0,0}};

        // DrawBalls(Balls);

        drawCustomWindow();

        EndDrawing();
    };
    return 0;
};
