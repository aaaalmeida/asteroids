#include <stdio.h>
#include <math.h>
#include <time.h>
#include "raylib.h"

#define TARGET_FPS 60
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGTH 600
#define PLAYER_SIZE 80
#define INITIAL_PLAYER_SPEED 2
#define INITIAL_ANGLE 270
#define INITIAL_PLAYER_ROTATION_SPEED 1

void drawPlayer();
void handlePlayerInput();
void shoot();
void shootBackward(int angle);
bool didCollideWithWall(); 
int GetRandomProjectileAngle();

typedef struct Player {
    Vector2 position;
    float size; // radius
    float angle; // degrees
    float speed;
    float speed_rotation;
} Player;

Player player = {
    (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGTH/2},
    PLAYER_SIZE,
    INITIAL_ANGLE,
    INITIAL_PLAYER_SPEED,
    INITIAL_PLAYER_ROTATION_SPEED
};

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "asteroids");
    SetTargetFPS(TARGET_FPS);

    int frameCounter = 0;
    int projectileDuration = 0;
    int projectileAngle = GetRandomProjectileAngle();

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        frameCounter++;
        if (frameCounter >= TARGET_FPS) frameCounter = 0;

        drawPlayer();
        handlePlayerInput();
        
        projectileDuration++;
        if (projectileDuration < TARGET_FPS * 1.5) shootBackward(projectileAngle);
        else {
            projectileDuration = 0;
            projectileAngle = GetRandomProjectileAngle();
        }

        ClearBackground(BLACK);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

void drawPlayer() {
    // TODO: delete direction indicator
    /*
    int lineThickness = 4;
    DrawCircleLinesV(player.position, player.size, RAYWHITE);
    DrawLineEx(
            player.position, 
            (Vector2){
                player.position.x + (player.size * cosf(player.angle * DEG2RAD)), 
                player.position.y + (player.size * sinf(player.angle * DEG2RAD))}, 
            lineThickness, 
            RAYWHITE
        );
*/

    // I dont know what this is for :P
    int segments = 100;

    DrawCircleSectorLines(player.position,
            player.size, 
            player.angle + (((5*PI)/6) * RAD2DEG), 
            player.angle + (((7*PI)/6) * RAD2DEG), 
            segments, 
            RAYWHITE);
}

void handlePlayerInput() {
    // using module so player angle doesnt scape float scope
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        player.angle = (int)(player.angle - player.speed_rotation) % 360;
    }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        player.angle = (int)(player.angle + player.speed_rotation) % 360;
    }
    
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        // FIXME: consertar colisao com parede
        //if (!didCollideWithWall())
        player.position = (Vector2){
            player.position.x + (cosf(player.angle * DEG2RAD) * player.speed),
            player.position.y + (sinf(player.angle * DEG2RAD) * player.speed)};
    }

    //if (IsKeyPressed(KEY_SPACE)) shootBackward();
}

bool didCollideWithWall() {
    return CheckCollisionCircleLine(player.position, player.size, (Vector2){0, 0}, (Vector2){SCREEN_WIDTH, 0}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){0, 0}, (Vector2){0, SCREEN_HEIGTH}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){0, SCREEN_WIDTH}, (Vector2){SCREEN_HEIGTH, SCREEN_WIDTH}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){SCREEN_HEIGTH, 0}, (Vector2){SCREEN_HEIGTH, SCREEN_WIDTH});
}

void shoot() {
    //DrawLineEx(player.position, (Vector2){cosf(player.angle*DEG2RAD)}, 5, RAYWHITE);
    //DrawCircleV(player.position, 5, GREEN);
}

int GetRandomProjectileAngle() {
    float inverseAngle = player.angle - 180;
    int angleRange = 10;
    SetRandomSeed(time(NULL));
    return GetRandomValue(inverseAngle - angleRange, inverseAngle + angleRange);
}

void shootBackward(int angle) {
    int projectileRange = 200;
    int projectileThickness = 3;
    
    DrawLineEx(player.position, (Vector2){
            player.position.x + cosf(angle * DEG2RAD) * projectileRange,
            player.position.y + sinf(angle * DEG2RAD) * projectileRange},
            projectileThickness, RAYWHITE);
}

