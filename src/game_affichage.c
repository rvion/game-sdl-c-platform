

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






/* Affichage des pièces */
void draw_pieces(SDL_Surface* surface, Map* map, Camera* camera)
{
	int i;

	for(i = 0; i < map->nb_pieces; i++)
	{
		/* si les pièces sont dans la map */
		if (point_in_square ( map->pieces_x[i]
							, map->pieces_y[i]
							, camera->x
							, camera->y
							, camera->x + SCREEN_W
							, camera->y + SCREEN_H )
			&& map->pieces_prises[i] == 0)
		{
			draw_cercle ( surface
						, map->pieces_x[i] - camera->x
						, map->pieces_y[i] - camera->y
						, 5
						, SDL_MapRGB(surface->format,0,0,0));
		}
	}
	return;
}


/* Affichage des cercles */

void draw_circles(SDL_Surface* surface, Map* map, Camera* camera)
{
	int i;
	
	for(i = 0; i < map->nb_circles; i++)
	{
		if (point_in_square ( map->circles[i].origine_x
							, map->circles[i].origine_y
							, camera->x
							, camera->y
							, camera->x + SCREEN_W
							, camera->y + SCREEN_H ))
		{
			map->circles[i].visible=TRUE;
			draw_cercle ( surface
						, map->circles[i].origine_x - camera->x
						, map->circles[i].origine_y - camera->y
						, map->circles[i].radius
						, SDL_MapRGB(surface->format,0,0,0));
		}
		else
		{
			map->circles[i].visible=FALSE;
		}
	}
	return;
}


/*Affichage des polygons */
void draw_polygons(SDL_Surface* surface, Map* map, Camera* camera)
{
	int i;
	
	for(i = 0; i < map->nb_polygons; i++)
	{
		if (collision_squares	( camera->x
								, camera->y
								, camera->x + SCREEN_W
								, camera->y + SCREEN_H
								, map->polygons[i].x_min
								, map->polygons[i].y_min
								, map->polygons[i].x_max
								, map->polygons[i].y_max) == TRUE)
		{	
			/* Si le polygon est dans l'écran */
			map->polygons[i].visible = TRUE;
			
			if (map->polygons[i].polygon->type ==1)
			{
				/* Si le polygon est de type 1, on l'affiche plein */
				draw_polygon_2	( surface
								, map->polygons[i].polygon
								, map->polygons[i].origin_x - camera->x
								, map->polygons[i].origin_y - camera->y
								, SDL_MapRGBA(surface->format, 255, 255, 255, map->polygons[i].polygon->alpha));
			}
			else if (map->polygons[i].polygon->type ==2)
			{
				/* Si le polygon est de type 2, on l'affiche vide */
				draw_polygon_2_line	( surface
								, map->polygons[i].polygon
								, map->polygons[i].origin_x - camera->x
								, map->polygons[i].origin_y - camera->y
								, SDL_MapRGBA(surface->format, 255, 255, 255, map->polygons[i].polygon->alpha));
			}
		}
		else 
		{
			/* Si le polygon est hors de l'écran */
			map->polygons[i].visible = FALSE;
		}
	}
	return;
}

/* Affichage des decors */
void draw_polygons_decors(SDL_Surface* surface, Map* map, Camera* camera)
{
	int i;
	
	/* Affichage des decors */
	for(i = 0; i < map->nb_polygons_decor; i++)
	{
		if (collision_squares	( camera->x
								, camera->y
								, camera->x + SCREEN_W
								, camera->y+ SCREEN_H
								, map->polygons_decor[i].x_min
								, map->polygons_decor[i].y_min
								, map->polygons_decor[i].x_max
								, map->polygons_decor[i].y_max) == TRUE)
		{
			/* Si le polygon est dans l'écran */
			if (map->polygons_decor[i].polygon->type ==1)
			{
				/* Si le polygon est de type 1, on l'affiche plein */
				draw_polygon_2	( surface
								, map->polygons_decor[i].polygon
								, map->polygons_decor[i].origin_x - camera->x
								, map->polygons_decor[i].origin_y - camera->y
								, SDL_MapRGBA(surface->format, 255, 255, 255, map->polygons[i].polygon->alpha));

			}
			else if (map->polygons_decor[i].polygon->type ==2)
			{
				/* Si le polygon est de type 2, on l'affiche vide */
				draw_polygon_2_line	( surface
								, map->polygons_decor[i].polygon
								, map->polygons_decor[i].origin_x - camera->x
								, map->polygons_decor[i].origin_y - camera->y
								, SDL_MapRGBA(surface->format, 255, 255, 255, map->polygons[i].polygon->alpha));
			}
		}
	}
}        
