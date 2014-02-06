#ifndef SDL_AFFICHAGE_H_INCLUDED
#define SDL_AFFICHAGE_H_INCLUDED

#include "structures.h"
void ligneHorizontale   (SDL_Surface *surface, int x, int y, int w, Uint32 couleur);    /* Dessinne une ligne horizontale de largeur w  */
void ligneVerticale     (SDL_Surface *surface, int x, int y, int h, Uint32 couleur);    /* Dessinne une ligne verticale   de largeur h  */
void draw_line(SDL_Surface* surface, double x1, double y1 , double x2, double y2, Uint32 couleur);
/*
void draw_polygon       (SDL_Surface *surface, Polygon poly);            Dessine un Polygon, Triangle par Triangle    */
void draw_polygon_2     (SDL_Surface *surface, Polygon *poly, double x1, double y1, Uint32 couleur);
void draw_polygon_2_line(SDL_Surface *surface, Polygon *poly, double x1, double y1, Uint32 couleur);

/*void draw_polygon_coord	(SDL_Surface *surface, Polygon poly);  Dessine un Polygon, Triangle par Triangle    */

void draw_triangle      (SDL_Surface *surface, double x1, double y1     /* Dessine un triangle                          */
                                             , double x2, double y2
                                             , double x3, double y3, Uint32 couleur);

void draw_cercle        (SDL_Surface* surface, int x,  int y ,  int r,  Uint32 couleur); /* Dessinne un cercle */
void draw_disque        (SDL_Surface* surface, int x,  int y ,  int r,  Uint32 couleur); /* Dessinne un disque */

void draw_recangle      (SDL_Surface* surface, int x1, int y1,  int x2, int y2, Uint32 couleur);

Uint32 couleur_aleatoire(SDL_Surface* surface);                             /* Donne une couleur aléatoire                      */
Uint32 getPixel         (SDL_Surface* surface, int x , int y);              /* Récupère la couleur d'un pixel sur une surface   */
void   setPixel         (SDL_Surface* surface, int x, int y, Uint32 coul);  /* Change   la couleur d'un pixel sur une surface   */


#endif
