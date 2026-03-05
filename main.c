#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define SCREEN_WIDTH 600 
#define SCREEN_HEIGTH 600
#define PLAYER_SIZE 20
#define INITIAL_PLAYER_SPEED 2
#define INITIAL_ANGLE 270
#define INITIAL_PLAYER_ROTATION_SPEED 1

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

void drawPlayer();
void handlePlayerInput();
bool didCollideWithWall(); 
void shoot();

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "asteroids");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        drawPlayer();
        handlePlayerInput();
    
        ClearBackground(BLACK);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

void drawPlayer() {
    DrawCircleLinesV(player.position, player.size, RAYWHITE);
    DrawLineEx(player.position, (Vector2){player.position.x + (player.size * cosf(player.angle * DEG2RAD)), player.position.y + (player.size * sinf(player.angle * DEG2RAD))}, 4, RAYWHITE);
}

void handlePlayerInput() {
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        player.angle -= player.speed_rotation;
    }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        player.angle += player.speed_rotation;
    }
    else if (IsKeyPressed(KEY_SPACE)) shoot();
    else if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        // FIXME: consertar colisao com parede
        //if (!didCollideWithWall())
            player.position = (Vector2){
                player.position.x + (cosf(player.angle * DEG2RAD) * player.speed),
                player.position.y + (sinf(player.angle * DEG2RAD) * player.speed)
            };
    }
}

bool didCollideWithWall() {
    return CheckCollisionCircleLine(player.position, player.size, (Vector2){0, 0}, (Vector2){SCREEN_WIDTH, 0}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){0, 0}, (Vector2){0, SCREEN_HEIGTH}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){0, SCREEN_WIDTH}, (Vector2){SCREEN_HEIGTH, SCREEN_WIDTH}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){SCREEN_HEIGTH, 0}, (Vector2){SCREEN_HEIGTH, SCREEN_WIDTH});
}

void shoot() {
    DrawCircleV(player.position, 5, GREEN);
}

