#ifndef GAME_AFFICHAGE_H_INCLUDED
#define GAME_AFFICHAGE_H_INCLUDED

#include "structures.h"
#include "constantes.h"



void draw_pieces(SDL_Surface* surface,   Map* map, Camera* camera);			/* Affichage des pièces */
void draw_circles(SDL_Surface* surface,  Map* map, Camera* camera);			/* Affichage des cercles */
void draw_polygons(SDL_Surface* surface, Map* map, Camera* camera);			/* Affichage des polygons */
void draw_polygons_decors(SDL_Surface* surface, Map* map, Camera* camera);	/* Affichage des decors */

#endif
