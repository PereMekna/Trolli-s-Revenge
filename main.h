#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define LARGEUR_FEN 768
#define HAUTEUR_FEN 640
#define GRAVITY_SPEED 0.6
#define MAX_FALL_SPEED 12
#define JUMP_HEIGHT 15
#define MAX_PLAYER_SPEED 10
#define PLAYER_SPEED 0.7
#define BLOC_DIM 64

typedef enum
{
    DROITE, GAUCHE, SAUT, REPLI, INACTIF
} typeetat;

typedef enum
{
    EMPTY, GROUND, WALL
} typebloc;

typedef struct entity
{
    SDL_Surface *image;
    SDL_Rect pos;
    SDL_Rect onMapPos;
    typeetat etat;
    int onGround;
    int saut;
    int hp;
    int timerDeath;
    float dirX, dirY;
  int saveX, saveY;
} entity;

entity trolli;

typedef struct input
{
    int right;
    int left;
    int up;
    int down;
    int space;
    int shift;
    int mouse1;
    int mouse2;
} Input;

Input input;

typedef struct map
{
    typebloc tile[30][30];
    SDL_Rect startPos;
} Map;

Map map;

void getInput();
void initPlayer();
void updatePlayer();
void col();
void delay(int frameLimit);
