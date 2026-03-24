#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"

#define TARGET_FPS 60
#define SCREEN_WIDTH 1000 
#define SCREEN_HEIGTH 650
#define PLAYER_SIZE 60
#define INITIAL_ANGLE 270
#define SHIP_ANGLE 15

//TODO: change player speed as the game progress
#define INITIAL_PLAYER_SPEED 2
#define INITIAL_PLAYER_ROTATION_SPEED 1

// ----- PLAYER -----
typedef struct {
    Vector2 position;
    float size; // radius
    float angle; // degrees
    float speed;
    float speed_rotation;
} Player;

void drawPlayer();
void handlePlayerInput();
void shootBackward(int angle);
bool didCollideWithWall(); 
float getRandomProjectileAngle();

// ----- ASTEROID -----
typedef struct {
    Vector2 position;
    float direction;
    float size;
    float speed;
} Asteroid;

typedef struct AsteroidNode{
    Asteroid* asteroid;
    struct AsteroidNode* next;
} AsteroidNode;

Asteroid createNewAsteroid();
AsteroidNode* createNewAsteroidNode();
void drawAsteroid(Asteroid*);
void drawAllAsteroids(AsteroidNode* head);
void updateAsteroid(Asteroid*);
void updateAllAsteroids(AsteroidNode* head);
void destroyAsteroidNode(AsteroidNode* target, AsteroidNode* before);

// ----- UTIL -----
float randomFloatRange(float min, float max);
void log(char*);

Player player = {
    (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGTH/2},
    PLAYER_SIZE,
    INITIAL_ANGLE,
    INITIAL_PLAYER_SPEED,
    INITIAL_PLAYER_ROTATION_SPEED
};

AsteroidNode* asteroidListHead = NULL;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "asteroids");
    SetTargetFPS(TARGET_FPS);

    int frameCounter = 0;

    asteroidListHead = (AsteroidNode*) malloc(sizeof(AsteroidNode));
    asteroidListHead = createNewAsteroidNode();

    int projectileDuration = 0;
    float projectileAngle = getRandomProjectileAngle();

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
            projectileAngle = getRandomProjectileAngle();
        }

        updateAllAsteroids(asteroidListHead);
        drawAllAsteroids(asteroidListHead);

        ClearBackground(BLACK);
        EndDrawing();
    }
    
    //TODO: free asteroid list

    CloseWindow();

    return 0;
}

void drawPlayer() {
    int segments = 1;
    DrawCircleSectorLines(
            player.position,
            player.size, 
            //player.angle + (((5*PI)/6) * RAD2DEG), 
            //player.angle + (((7*PI)/6) * RAD2DEG), 
            (player.angle - 180) + SHIP_ANGLE, 
            (player.angle - 180) - SHIP_ANGLE,
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
}

bool didCollideWithWall() {
    return CheckCollisionCircleLine(player.position, player.size, (Vector2){0, 0}, (Vector2){SCREEN_WIDTH, 0}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){0, 0}, (Vector2){0, SCREEN_HEIGTH}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){0, SCREEN_WIDTH}, (Vector2){SCREEN_HEIGTH, SCREEN_WIDTH}) ||
            CheckCollisionCircleLine(player.position, player.size, (Vector2){SCREEN_HEIGTH, 0}, (Vector2){SCREEN_HEIGTH, SCREEN_WIDTH});
}

float randomFloatRange(float min, float max) {
    srand(time(NULL));
    return min + ((rand()/(float) RAND_MAX) * (max - min));
}

float getRandomProjectileAngle() {
    float inverseAngle = player.angle - 180;
    int angleRange = 10;
    return randomFloatRange(inverseAngle - angleRange, inverseAngle + angleRange);
}

void shootBackward(int angle) {
    int projectileRange = 200;
    int projectileThickness = 3;
    
    DrawLineEx(player.position, (Vector2){
            player.position.x + cosf(angle * DEG2RAD) * projectileRange,
            player.position.y + sinf(angle * DEG2RAD) * projectileRange},
            projectileThickness, RAYWHITE);
}

Asteroid createNewAsteroid() {
    int radius = (int) randomFloatRange(20, 50);
    float speed = randomFloatRange(0, 1);
    float minAngle = -0.5;
    float maxAngle = 0.5;
    float direction = randomFloatRange(minAngle, maxAngle);

    // TODO: generate support in a random place near walls
    return (Asteroid) {(Vector2){radius, radius}, direction, radius, speed};
}

AsteroidNode* createNewAsteroidNode() {
    Asteroid asteroid = createNewAsteroid();
    AsteroidNode node = { &asteroid, NULL };
    node.next = (AsteroidNode*) malloc(sizeof(AsteroidNode));
    return &node; 
}

void destroyAsteroidNode(AsteroidNode* target, AsteroidNode* before) {
    before->next = target->next;
    free(target);
}

void drawAsteroid(Asteroid* asteroid) {
    DrawCircleLinesV(asteroid->position, asteroid->size, GREEN);
}

void drawAllAsteroids(AsteroidNode* head) {
    drawAsteroid(head->asteroid);
    if (head->next) drawAllAsteroids(head->next);
}

void updateAsteroid(Asteroid* asteroid) {
    //TODO: check collision with wall
    //TODO: check if asteroid have been shot   
    asteroid->position = (Vector2){
        (asteroid->position.x + cosf(asteroid->direction)) * asteroid->speed,
        (asteroid->position.y + sinf(asteroid->direction)) * asteroid->speed};
}

void updateAllAsteroids(AsteroidNode* head) {
    updateAsteroid(head->asteroid);
    log("UPDATE ALL ASTEROIDS");
    if (head->next) updateAllAsteroids(head->next);
}

void log(char *msg) {
    printf("\n[INFO] %s", msg);
}

