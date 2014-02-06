/*-----------------------------------------------
|                                               |
|         Petit jeu de Platforme                |
|                                               |
|       réalisé par    Arnaud Manente           |
|               et     Rémi   Vion              |
|                                               |
-----------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <math.h>

#include "structures.h"
#include "constantes.h"
#include "polygons.h"
#include "basic_maths.h"
#include "sdl_affichage.h"
#include "geometry.h"
#include "physics.h"
#include "camera.h"
#include "ini_load.h"
#include "game_affichage.h"

void clean_map(Map* map);

/*-----------------
      Main
 -----------------*/

int main(int argc,char* argv[])
{
    int alive = TRUE;	/* alive = le jeu jeu est en train de tourner 	*/
	int i;
	
    /*--------------------
        Initialisation
    --------------------*/

	/* Main data structures */
    Map 	map;
    Player 	player;
    Camera 	camera;
    
	
	/* SDL STUFF */
	SDL_Surface 			*screen     = NULL;
	SDL_Event 				event;
	Uint8 					*keystates;
	const SDL_VideoInfo* 	myPointer;
	unsigned int checkTime = 0;
	const unsigned int fps = 100;
   
    map.height              = 10000;
    map.width               = 10000;
    map.grav                = 0.4;

    map.nb_unique_polygons	= 0;
	map.nb_circles			= 0;
	map.nb_squares			= 0;
	map.nb_polygons			= 0;
	map.nb_polygons_decor   = 0;
	map.nb_pieces           = 0;
	map.nb_gravs           	= 0;


    player.d               = P;
    player.r               = (P/2);
    player.rc              = (P/2)+1;

    player.a_x              = 0;
    player.a_y              = 0;
    player.v_x             = 0;
    player.v_y             = 0;
    player.max_v_x         = 12;
    player.max_v_y         = 12;

    player.x               = 300;
    player.y               = 200;
    player.jump_force      = 10;
    player.wall_jump_force = 10;
    player.angle           = 0.35*PI;
    player.pieces          = 0;

    player.on_ground        = 0;
    player.on_left_wall     = 0;
    player.on_right_wall    = 0;
    player.on_segment       = -1;

    player.input.ac1        = 0;
    player.input.ac2        = 0;
    player.input.up         = 0;
    player.input.hold_up    = 0;
    player.input.down       = 0;
    player.input.left       = 0;
    player.input.right      = 0;

    camera.x               = 0;
    camera.y               = 0;
    camera.x_target        = player.x;
    camera.y_target        = player.y;
    camera.dx_min_1        = 200;
    camera.dy_min_1        = 200;
    camera.dx_min_2        = 250;
    camera.dy_min_2        = 250;
    camera.vit_x           = 12;
    camera.vit_y           = 12;

	ini_load_map(&map, "MAP.ini");
	
    SDL_Init(SDL_INIT_VIDEO);
	keystates = SDL_GetKeyState(NULL);
   
   	checkTime = SDL_GetTicks();
    SDL_WM_SetCaption("Platform Arnaud Remi", NULL);

    /* Récupère les informations sur le matériel vidéo actuel*/
     myPointer = SDL_GetVideoInfo();

    /* Configure le mode vidéo */
    screen = SDL_SetVideoMode(myPointer->current_w, myPointer->current_h, myPointer->vfmt->BitsPerPixel, SDL_SRCALPHA|SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	
	/* autres configs possibles */
	/**screen = SDL_SetVideoMode(myPointer->current_w, myPointer->current_h, myPointer->vfmt->BitsPerPixel, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN); */
    /**screen   = SDL_SetVideoMode(SCREEN_W,SCREEN_H,32,SDL_HWSURFACE | SDL_DOUBLEBUF );*/
    
    while(alive)
    {
		if(SDL_GetTicks() > (checkTime + 1000 / fps) ) {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
            alive = 0;
            break;
        }
				
        /*-----------------
                Input
        -----------------*/
        SDL_PumpEvents();

        if(keystates[SDLK_ESCAPE])  { alive=0; }
        if(keystates[SDLK_UP])      { player.input.up    =1; } else { player.input.up    =0; player.input.hold_up=0; }
        if(keystates[SDLK_DOWN])    { player.input.down  =1; } else { player.input.down  =0; }
        if(keystates[SDLK_LEFT])    { player.input.left  =1; } else { player.input.left  =0; }
        if(keystates[SDLK_RIGHT])   { player.input.right =1; } else { player.input.right =0; }

        /*----------------------
               Acceleration
        -----------------------*/

        player.a_x=0;
        player.a_y=0;

        if (player.input.up  ==1)
        {
            if (player.on_ground ==1)
            {
                player.a_y 				-= player.jump_force;
                player.input.hold_up 	=  25;
                player.v_y			 	=  0;
            }
            else if ( player.input.hold_up == 0)
            {
                if  ( player.on_left_wall==1 && player.on_right_wall==1)
                {
                    player.a_y 				-= player.jump_force;
                    player.input.hold_up	=25;
                    player.v_y				=0;

                }
                else if  ( player.on_left_wall==1)
                {
                    player.a_x 				+= player.wall_jump_force * cos(player.angle);
                    player.a_y 				-= player.wall_jump_force * sin(player.angle) ;
                    player.input.hold_up	=25;
                    player.v_y				=0;
                }
                else if (player.on_right_wall==1)
                {
                    player.a_x 				-= player.wall_jump_force * cos(player.angle);
                    player.a_y 				-= player.wall_jump_force * sin(player.angle);
                    player.input.hold_up	=25;
                    player.v_y				=0;
                }
            }
        }

		if (player.input.hold_up >0)
		{
			player.input.hold_up	--;
			player.a_y 				+= map.grav/3;
		}
		else { player.a_y += map.grav;}
		
		if (map.nb_gravs >0)
		for (i=0; i< map.nb_gravs; i++)
		{
			if (point_in_square ( player.x
				, player.y
				, map.grav_x1[i]
				, map.grav_y1[i]
				, map.grav_x2[i]
				, map.grav_y2[i]))
			{
				player.a_x += map.grav_vx[i];
				player.a_y += map.grav_vy[i];
			}
		}
		
        if      (player.input.left  ==1) { player.a_x -= 0.4;}
        else if (player.input.right ==1) { player.a_x += 0.4;}
        else
        {
            if (player.on_ground ==1)
            {
                player.v_x *= (1-(0.01*fabs(player.v_x)));
            }
            else
            {
                player.v_x *= (1-(0.002*fabs(player.v_x)));
            }
        }

        /*----------------------
              Speed
        -----------------------*/

        player.v_x += player.a_x;
        player.v_y += player.a_y;

        if (player.v_x > player.max_v_x)  { player.v_x =  player.max_v_x; }
        if (player.v_y > player.max_v_y)  { player.v_y =  player.max_v_y; }
        if (player.v_x < -player.max_v_x) { player.v_x = -player.max_v_x; }
        if (player.v_y < -player.max_v_y) { player.v_y = -player.max_v_y; }

        /*----------------------
            Position
        -----------------------*/

        update_player(&player, &map);

        /* update de la camera */
        camera.x_target=player.x;
        camera.y_target=player.y;
        update_camera(&camera,map.height,map.width);

        /*----------------------
            affichage
        ----------------------*/
        SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format,0,0,0,0));

		draw_pieces			( screen, &map, &camera);	/* Affichage des pièces 	*/
		draw_circles		( screen, &map, &camera);	/* Affichage des cercles 	*/
		draw_polygons		( screen, &map, &camera);	/* Affichage des polygons 	*/
		draw_polygons_decors( screen, &map, &camera);	/* Affichage des decors 	*/

		/* on affiche le joueur */
		draw_cercle ( screen
			, player.x-camera.x
			, player.y-camera.y
			, player.r
			, SDL_MapRGB(screen->format,0,0,0));
	
        SDL_Flip(screen);
        /*SDL_Delay(10);*/
		checkTime = SDL_GetTicks();
		}
    }
	
	clean_map(&map);
    SDL_FreeSurface(screen);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void clean_map(Map* map)
{
   int i;
   /** Destroy ALL the unique Polygons!*/
   for(i = 0; i < map->nb_unique_polygons; i++) {
       free(map->unique_polygons[i].point_x);
       free(map->unique_polygons[i].point_y);
       free(map->unique_polygons[i].triangle_x0);
       free(map->unique_polygons[i].triangle_x1);
       free(map->unique_polygons[i].triangle_x2);
       free(map->unique_polygons[i].triangle_y0);
       free(map->unique_polygons[i].triangle_y1);
       free(map->unique_polygons[i].triangle_y2);
   }
   free(map->unique_polygons);

   /** Destroy ALL the Squares!*/
   for(i = 0; i < map->nb_squares; i++) {
       free(map->squares[i].point_x);
       free(map->squares[i].point_y);
       free(map->squares[i].triangle_x0);
       free(map->squares[i].triangle_x1);
       free(map->squares[i].triangle_x2);
       free(map->squares[i].triangle_y0);
       free(map->squares[i].triangle_y1);
       free(map->squares[i].triangle_y2);

   }
   free(map->squares);

   /** Destroy ALL the Circles!*/
   free(map->circles);


   /** Destroy ALL the Triplets!*/
   for(i = 0; i < map->nb_polygons; i++) {
       free(map->polygons[i].optimisation);
   }
   free(map->polygons);

   /** Destroy ALL the Decorations!*/
   for(i = 0; i < map->nb_polygons_decor; i++) {
       free(map->polygons_decor[i].optimisation);
   }
   free(map->polygons_decor);

   /** DESTROY EVERYTHING THAT'S LEFT!*/
   free(map->pieces_prises);
   free(map->pieces_x);
   free(map->pieces_y);

   free(map->grav_vx);
   free(map->grav_vy);
   free(map->grav_x1);
   free(map->grav_x2);
   free(map->grav_y1);
   free(map->grav_y2);
}
