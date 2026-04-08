#include "raylib.h"
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



const float BALLRADIUS = 15.0; //All balls with have the same radius

float BallCountSliderValue;
float GravitySliderValue = 9.8;
float MassSliderValue = 1.0;
Color BallColourValue = {1.0, 0.0, 0.0};

bool Started = false;


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

void drawCustomWindow(Texture GridTexture , Font FontToUse){

    //Drawing the UI for the simulator

    char ColourValue[] = "";

    DrawRectangle(0.01, 0.01, 1280.0,720.0, WHITE); //White main area

    DrawTextureRec(GridTexture, (Rectangle){0.0,0.0,874.0,716.0}, (Vector2){0.0, 0.0}, WHITE); // Render the grid

    DrawLineEx((Vector2){874.0,716.0},(Vector2){874.0,0.0},5.0,BLACK); //Drawing border between the grid and the buttons

    DrawTextPro(FontToUse, "Options", (Vector2){900.0, 0.0}, (Vector2){0.0, 0.0}, 0.0, 24.0, 1.0, BLACK); //Draw the text "Options"

    DrawLineEx((Vector2){889.0, 20.0}, (Vector2){990.0, 20.0}, 2.0, BLACK ); //Draw line under the "options" text

    DrawTextPro(FontToUse, TextFormat("Ball Amount: %d",(int)BallCountSliderValue), (Vector2){900.0, 60.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 1.0, BLACK); //Draw the text for the current ball count

    GuiSlider((Rectangle){900.0, 110.0, 100.0, 30.0},"1", "5",&BallCountSliderValue, 1.0, 5.0); // Create slider for ball count

    DrawTextPro(FontToUse, TextFormat("Gravity (Downwards accelerations): %.1f",GravitySliderValue), (Vector2){900.0, 160.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 1.0, BLACK); // Draw text for gravity title

    GuiSlider((Rectangle){900.0, 210.0, 300.0, 30.0},"-50", "50",&GravitySliderValue, -50, 50.0);

    DrawTextPro(FontToUse, TextFormat("Color:"), (Vector2){900.0, 260.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 1.0, BLACK); // Draw text for balls colour

    GuiColorPicker((Rectangle){900.0, 310.0, 300.0, 60.0},ColourValue,&BallColourValue); // create colour picker widget


    DrawTextPro(FontToUse, TextFormat("START SIMULATION:"), (Vector2){900.0, 600.0}, (Vector2){0.0, 0.0}, 0.0, 18.0, 2.0, BLACK); // Draw text for start simulation

    DrawLineEx((Vector2){889.0, 20.0}, (Vector2){990.0, 20.0}, 2.0, BLACK ); //Draw line under the "options" text

    int ButtonResult = GuiButton((Rectangle){900.0, 650.0, 300.0, 60.0},"START") ; //create button

    if (ButtonResult == 1) {
        Started = true; // set started = true when the button is pressed, the simulation should then be started by other code
    }; 
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
