#include "main.h"

SDL_Surface *ecran;
SDL_Surface *tile;
SDL_Surface *tile2;

int main(int argc, char *argv[])
{

    SDL_Surface *bg;

    int color = 0;

    bg = IMG_Load("backg.jpg");
    tile = IMG_Load("ground1.png");
    tile2 = IMG_Load("bloc.png");
    SDL_Rect bgpos;
    SDL_Rect tmppos;
    bgpos.x = 0;
    bgpos.y = 0;
    bgpos.h = bg->h;
    bgpos.w = bg->w;
    int continuer = 1;
    int frameLimit = 0;
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(LARGEUR_FEN, HAUTEUR_FEN, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    int i;
    initPlayer();
    loadMap();
    while (continuer)
    {
        getInput();

        updatePlayer();

        scrollMap();
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 200, color/2 + 128, 256-color));
        drawMap();
        col();
        SDL_BlitSurface(trolli.image, NULL, ecran, &trolli.onMapPos);
        delay(frameLimit);
        frameLimit = SDL_GetTicks() + 16;
        SDL_Flip(ecran);
        color = trolli.pos.y*2/5;
        //SDL_FreeSurface(trolli.image);
    }
    SDL_FreeSurface(ecran);
    SDL_Quit();
    return 0;
}

void getInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT : exit(EXIT_SUCCESS); break;
            case SDL_KEYDOWN :
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE : exit(EXIT_SUCCESS); break;
                case SDLK_LEFT : input.left = 1; break;
                case SDLK_RIGHT : input.right = 1; break;
                case SDLK_UP : input.up = 1; break;
                case SDLK_DOWN : input.down = 1; break;
                case SDLK_SPACE : input.mouse1 = 1; break;
                default: break;
            };break;
            case SDL_KEYUP :
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE : exit(EXIT_SUCCESS); break;
                case SDLK_LEFT : input.left = 0; break;
                case SDLK_RIGHT : input.right = 0; break;
                case SDLK_UP : input.up = 0; break;
                case SDLK_DOWN : input.down = 0; break;
                case SDLK_SPACE : input.mouse1 = 0; break;
                default: break;
            };break;
            default: break;
        }
    }
}

void initPlayer()
{
    trolli.pos.x = 64;
    trolli.pos.y = 0;
    trolli.image = IMG_Load("meknatrolli.png");
    trolli.onGround = 0;
    trolli.etat = DROITE;
    trolli.timerDeath = 0;
    trolli.dirX = 0;
    trolli.dirY = 0;
    map.startPos.x = 0;
    map.startPos.y = 0;
}

void updatePlayer()
{
    if (trolli.timerDeath == 0)
    {
        trolli.dirY += GRAVITY_SPEED;
        if (trolli.dirY >= MAX_FALL_SPEED)
        {
            trolli.dirY = MAX_FALL_SPEED;
        }
        if (input.left == 1)
  {
		trolli.dirX -= PLAYER_SPEED;
        if (trolli.dirX <= -MAX_PLAYER_SPEED)
        {
            trolli.dirX = -MAX_PLAYER_SPEED;
        }
        if(trolli.etat != GAUCHE)
		{
		    trolli.etat = GAUCHE;
		    trolli.image = IMG_Load("meknagauche.png");
		}
	}
    if (input.left == 0 && input.right == 0)
	{
		if (trolli.etat != INACTIF)
		{
		    trolli.image = IMG_Load("meknatrolli.png");
		}
		if (trolli.dirX >= PLAYER_SPEED)
		{
           trolli.dirX -= PLAYER_SPEED;
		}
		else if (trolli.dirX <= - PLAYER_SPEED)
		{
		    trolli.dirX += PLAYER_SPEED;
		}
		else trolli.dirX = 0;
		trolli.etat = INACTIF;
	}

	if (input.right == 1)
	{
		trolli.dirX += PLAYER_SPEED;
        if (trolli.dirX >= MAX_PLAYER_SPEED)
        {
            trolli.dirX = MAX_PLAYER_SPEED;
        }
		if(trolli.etat != DROITE)
		{
		    trolli.etat = DROITE;
		    trolli.image = IMG_Load("meknadroite.png");
		}
  }
    if (input.up == 1 && trolli.saut != 1)
    {
        trolli.dirY -= JUMP_HEIGHT;
        trolli.onGround = 0;
        trolli.saut = 1;
    }


    }
    if (trolli.pos.y > HAUTEUR_FEN)
    {
        trolli.pos.x = 0;
        trolli.pos.y = 0;
        trolli.dirY = 0;
        trolli.dirX = 0;
    }
}

void col()
{
    int x1, x2, y1, y2;

    x1 = (trolli.pos.x + trolli.dirX)/BLOC_DIM;
    x2 = (trolli.pos.x + trolli.pos.h + trolli.dirX)/BLOC_DIM;

    y1 = (trolli.pos.y + trolli.dirY)/BLOC_DIM;
    y2 = (trolli.pos.y + trolli.pos.w + trolli.dirY)/BLOC_DIM;

    if (trolli.dirY > 0)
    {
        if (map.tile[x1][y2] != EMPTY || map.tile[x2][y2] != EMPTY)
        {
            if (input.up == 0)
            {
                trolli.dirY = 0;
                trolli.pos.y = (y1) * BLOC_DIM;
            }
            else
            {
                trolli.dirY -= JUMP_HEIGHT;
            }

        }

    }
    else if (trolli.dirY < 0)
    {
        if (map.tile[x1][y1] != EMPTY || map.tile[x2][y1] != EMPTY)
        {
            trolli.dirY = 0;
            trolli.pos.y = (y1+1) * BLOC_DIM + 1;
        }
    }

    {
        if (map.tile[x1][y1] != EMPTY || map.tile[x2][y1] != EMPTY)
        {
            trolli.dirX = 0;
        }
    }

    trolli.pos.x = trolli.pos.x + trolli.dirX;
    trolli.pos.y = trolli.pos.y + trolli.dirY;
    trolli.onMapPos.y = trolli.pos.y; //décale les y de 64px, de façon imcompréhensible
    trolli.onMapPos.x = trolli.pos.x - map.startPos.x; // pareil pour les x dans l'autre direction
}


void delay(int frameLimit)
{

    /* Gestion des 60 fps */
    int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

void drawMap()
{
    int i, j;

    SDL_Rect postmp;
    for (i = 0; i < 30 ; i++)
    {
        for (j = 0; j < 30; j++)
        {
            if (map.tile[i][j] == GROUND)
            {
                postmp.x = i*BLOC_DIM - map.startPos.x;
                postmp.y = j*BLOC_DIM;
                SDL_BlitSurface(tile, NULL, ecran, &postmp);
            }
            if (map.tile[i][j] == WALL)
            {
                postmp.x = i*BLOC_DIM - map.startPos.x;
                postmp.y = j*BLOC_DIM;
                SDL_BlitSurface(tile2, NULL, ecran, &postmp);
            }
        }


    }
}

void loadMap()
{
    FILE *fichier;
    int i, j;
    fichier = fopen("save.txt", "r");
    for (i = 0; i < 30 ; i++)
    {
        for (j = 0; j < 30 ; j++)
        {
           fscanf(fichier, "%d", &map.tile[i][j]);
        }
        //fprintf(fichier, "\n");
    }
    fclose(fichier);
}

void scrollMap()
{
    if (trolli.pos.x > LARGEUR_FEN / 2)
    {
        map.startPos.x = trolli.pos.x - LARGEUR_FEN/2;
    }
    else map.startPos.x = 0;
}
