#include "raylib.h"
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



const float BALLRADIUS = 15.0; //All balls with have the same radius

float BallCountSliderValue;
float GravitySliderValue = -9.8;

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

// int ArrayLength(Ball Array[]){
    
//     int size1 = sizeof(Array);
//     unsigned int newsize = size1/sizeof(Array[0]);
//     printf("The size of the array is %d", newsize);
//     return newsize;
// };

// void DrawBalls(Ball Balls[]){
//     int i ;
//     for (i = 0; i <= ArrayLength(Balls)+1; i++){
//         DrawCircle(Balls[i].x, Balls[i].y, BALLRADIUS, RED);
//         printf("Drew ball");
//     };
// };

void drawCustomWindow(Texture GridTexture , Font FontToUse){

    //Drawing the UI for the simulator

    DrawRectangle(0.0, 0.0, 1280.0, 720.0, GRAY); //Draw the grey background
    DrawRectangle(0.01, 0.01, 1280.0,720.0, WHITE); //White main area

    DrawTextureRec(GridTexture, (Rectangle){0.0,0.0,874.0,716.0}, (Vector2){0.0, 0.0}, WHITE); // Render the grid

    DrawLineEx((Vector2){874.0,716.0},(Vector2){874.0,0.0},5.0,BLACK); //Drawing border between the grid and the buttons

    DrawTextPro(FontToUse, "Options", (Vector2){900.0, 0.0}, (Vector2){0.0, 0.0}, 0.0, 24.0, 1.0, BLACK); //Draw the text "Options"

    DrawLineEx((Vector2){889.0, 20.0}, (Vector2){990.0, 20.0}, 2.0, BLACK ); //Draw line under the "options" text

    GuiSlider((Rectangle){900.0, 60.0, 100.0, 10.0},"1", "5",&BallCountSliderValue, 1.0, 5.0);

    DrawTextPro(FontToUse, TextFormat("Ball Amount: %d",(int)BallCountSliderValue), (Vector2){900.0, 40.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 1.0, BLACK); //Draw the text for the current ball count

    GuiSliderPro((Rectangle){900.0, 140.0, 100.0, 10.0},"-100.0", "100.0",&GravitySliderValue, -100.0, 100.0, 10);

    DrawTextPro(FontToUse, TextFormat("Gravity: %.2f",GravitySliderValue), (Vector2){900.0, 100.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 1.0, BLACK);

}

int main (){

    //Create window
    InitWindow(1280.0, 716.0, "PHYSICS SIMULATOR");

    //Set frames per second of the window
    SetTargetFPS(60);


    Texture GridTexture = LoadTexture("GridBG.png"); // Define grid texture

    Font FontToUse = LoadFont(""); //Not loading a font but want to use the default style


    while (!WindowShouldClose()){

        //Open the drawing buffer
        BeginDrawing();

        //Set the background to white to avoid tearing and other issues
        ClearBackground(RAYWHITE);
        
        //Draw the custom UI and button window
        drawCustomWindow(GridTexture, FontToUse);

        //Stop the drawing, remove from buffer
        EndDrawing();
    }

    UnloadTexture(GridTexture); //Unload for performance
    UnloadFont(FontToUse); //Unload for performance

    CloseWindow();
    
    return 0;
}
