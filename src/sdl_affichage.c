
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>

#include "sdl_affichage.h"
#include "structures.h"
#include "constantes.h"
#include "polygons.h"
#include "basic_maths.h"
#include "physics.h"


/**************************************************************************************************
	Set a pixel to a color
**************************************************************************************************/
void setPixel(SDL_Surface* surface, int x, int y, Uint32 coul)
{
    *((Uint32*)(surface->pixels) + x + y * surface->w) = coul;
}



/**************************************************************************************************
	Get color of a pixel
**************************************************************************************************/
Uint32 getPixel (SDL_Surface* surface, int x , int y)
{
    return *((Uint32*)(surface->pixels) + x + y * surface->w);
}



/**************************************************************************************************
    Draw a disque
**************************************************************************************************/
void draw_disque(SDL_Surface* surface, int x, int y , int r, Uint32 couleur)
{
    int i ;
    int j ;

    int ii = x-r;
    int jj = y-r;
    if (x-r < 0) {ii=0;}
    if (y-r < 0) {jj=0;}

    for (i=ii; i<=x+r; i++)
    {
        for (j=jj; j<=y+r; j++)
        {
            if (sqrt ((x-i)*(x-i) + (y-j)*(y-j))<=r)
            {
                setPixel(surface, i, j, SDL_MapRGBA(surface->format,255,255,255,100));
            }
        }
    }
}



/**************************************************************************************************
	Draw a circle
**************************************************************************************************/
void draw_cercle(SDL_Surface* surface, int x, int y , int r, Uint32 couleur)
 {
    int i ;
    int j ;
    int ii = x-r;
    int jj = y-r;
    if (x-r < 0) {ii=0;}
    if (y-r < 0) {jj=0;}

    for (i=ii; i<=x+r; i++)
    {
        for (j=jj; j<=y+r; j++)
        {
            if ( (sqrt ((x-i)*(x-i) + (y-j)*(y-j)) <=  r   )
            &&   (sqrt ((x-i)*(x-i) + (y-j)*(y-j)) >= (r-1)) )
            {
                setPixel(surface, i, j, SDL_MapRGBA(surface->format,255,255,255,100));
            }
        }
    }
}



/**************************************************************************************************
	Draw a line
**************************************************************************************************/
void draw_line(SDL_Surface* surface, double x1, double y1 , double x2, double y2, Uint32 couleur)
{
    int i;

    double sens_x = sign_of(x2-x1);
    double sens_y = sign_of(y2-y1);
    double x_modif;
    double y_modif;

    int xi1 = (int) x1;
    int yi1 = (int) y1;

   	int xi2 = (int) x2;
    int yi2 = (int) y2;
	
    int range;

    if          ( (xi2-xi1) == 0 && (yi2-yi1) == 0 )
    {
        return;
    }
    else if      ( (xi2-xi1) == 0 )
    {
        ligneVerticale	    (surface, (y1<y2 ? x1 : x2), (y1<y2 ? y1 : y2),abs(yi2-yi1) , couleur);
        return;
    }
    else if ((yi2-yi1) == 0 )
    {
        ligneHorizontale    (surface, (x1<x2 ? x1 : x2), (x1<x2 ? y1 : y2),abs(xi2-xi1), couleur);
        return;
    }
    else
    {
    	if (abs(yi1-yi2) > abs(xi1-xi2))
        {
            if   (fabs(y2 - y1) > 1)
            {
                x_modif = sens_x * fabs((x2-x1)/(y2-y1));
                y_modif = sens_y;
                range   = (int) fabs(y2-y1);
            }
            else { return; }
        }
        else
        {
            if   (fabs(x2-x1) > 1)
            {
                x_modif = sens_x;
                y_modif = sens_y * fabs((y2-y1)/(x2-x1));
                range   = (int) fabs(x2-x1);
            }
            else { return; }
        }
    }

	for (i=1; i<=range; i++)
    {
        if (   point_in_square( xi1 + (int) ((double) i * x_modif)
                              , yi1 + (int) ((double) i * y_modif)
                              , 0, 0, SCREEN_W, SCREEN_H) == TRUE)
            setPixel( surface
                    ,  xi1 + (int) ((double) i * x_modif)
                    ,  yi1 + (int) ((double) i * y_modif)
                    , couleur);
   	}

    return;
}



/**************************************************************************************************
    Draw a rectangle
**************************************************************************************************/
void draw_recangle(SDL_Surface* surface,int x1,int y1, int x2,int y2, Uint32 couleur)
{
    draw_line(surface, x1, y1, x2, y1, couleur);
    draw_line(surface, x2, y1, x2, y2, couleur);
    draw_line(surface, x2, y2, x1, y2, couleur);
    draw_line(surface, x1, y2, x1, y1, couleur);
}


/**************************************************************************************************
    Get a random color
**************************************************************************************************/
Uint32 couleur_aleatoire(SDL_Surface* surface)
{
    return ( SDL_MapRGB(surface->format
           , rand()%255
           , rand()%255
           , rand()%255));
}



/**************************************************************************************************
    Draw a polygon
**************************************************************************************************/
void draw_polygon_2(SDL_Surface *surface, Polygon *poly, double x1, double y1, Uint32 couleur)
{
    int i;

    for (i=0; i< (*poly).nb_triangles; i++)
    {
        draw_triangle( surface
                     , (*poly).triangle_x0[i]+x1
                     , (*poly).triangle_y0[i]+y1
                     , (*poly).triangle_x1[i]+x1
                     , (*poly).triangle_y1[i]+y1
                     , (*poly).triangle_x2[i]+x1
                     , (*poly).triangle_y2[i]+y1
                     , couleur);
    }
}



/**************************************************************************************************
    Draw a polygon outline
**************************************************************************************************/
void draw_polygon_2_line(SDL_Surface *surface, Polygon *poly, double x1, double y1, Uint32 couleur)
{
    int i;

    for (i=0; i< (*poly).nb_points-1; i++)
    {
        draw_line	( surface
                    , poly->point_x[i]+x1
                    , poly->point_y[i]+y1
                    , poly->point_x[i+1]+x1
                    , poly->point_y[i+1]+y1
                    , couleur);
    }
        draw_line	( surface
                    , poly->point_x[0]+x1
                    , poly->point_y[0]+y1
                    , poly->point_x[(*poly).nb_points-1]+x1
                    , poly->point_y[(*poly).nb_points-1]+y1
                    , couleur);
}



/**************************************************************************************************
    Draw a triangle
**************************************************************************************************/
void draw_triangle(SDL_Surface *surface, double x1, double y1, double x2, double y2, double x3, double y3, Uint32 couleur)
{
    double x_haut_1,y_haut_1;
    double x_haut_2,y_haut_2;
    /* on va dessiner un triangle a base de lignes...           */
    /* plus haut coté du triangle:                              */

    double x_milieu, y_milieu;
    double i;
    double ii;
    /* le point à droite ou gauche de la plus grande hauteur,   */

    double pente_1, pente_2, pente_3;
    double x_1, x_2, dd;
    double temp;


    /*                                         x1  y1   x2  y2   x3  y3            */
    /*  ne marche pas :  draw_triangle(screen, 200,200, 300,120, 250,250);      */
    /*  On détermine les points haut_1, haut_2                                  */
    if (y1< y2)
    {
        if (y1 < y3)
        {
                           x_haut_1 = x1; y_haut_1 = y1;    /* haut1    haut2  */
            if (y2 > y3) { x_haut_2 = x2; y_haut_2 = y2;
                           x_milieu = x3; y_milieu = y3;}   /*  1         2    */
            else         { x_haut_2 = x3; y_haut_2 = y3;
                           x_milieu = x2; y_milieu = y2;}   /*  1         3    */
        }
        else             { x_haut_1 = x3; y_haut_1 = y3;    /*  3         2    */
                           x_haut_2 = x2; y_haut_2 = y2;
                           x_milieu = x1; y_milieu = y1;}
    }
    else
   {
        if (y1 > y3)
        {
                           x_haut_2 = x1; y_haut_2 = y1;
            if (y2 < y3) { x_haut_1 = x2; y_haut_1 = y2;
                           x_milieu = x3; y_milieu = y3;}   /*  2         1    */
            else         { x_haut_1 = x3; y_haut_1 = y3;
                           x_milieu = x2; y_milieu = y2;}   /*  3         1    */
        }
        else             { x_haut_1 = x2; y_haut_1 = y2;    /*  2         3    */
                           x_haut_2 = x3; y_haut_2 = y3;
                           x_milieu = x1; y_milieu = y1;}
    }


    pente_1 = pente_y(x_haut_1, y_haut_1, x_haut_2, y_haut_2);
    pente_2 = pente_y(x_haut_1, y_haut_1, x_milieu, y_milieu);
    pente_3 = pente_y(x_milieu, y_milieu, x_haut_2, y_haut_2);


    for (i=0; i<(y_milieu-y_haut_1); i+=1)
    {
        x_1 = ( x_haut_1 + i * pente_1);
        x_2 = ( x_haut_1 + i * pente_2);

        if (x_1 > x_2) { temp = x_1;
                        x_1  = x_2;
                        x_2  = temp;}
        dd  = x_2 - x_1;
        ligneHorizontale(surface, (int) (x_1)
                                , (int) (i+y_haut_1)
                                , (int) (dd+1)
                                , couleur);
    }

    for (i=0; i<y_haut_2-y_milieu; i+=1)
    {
        ii  = i + (y_milieu-y_haut_1);
        x_1 = ( x_haut_1 + ii * pente_1);
        x_2 = ( x_milieu + i  * pente_3);

        if (x_1 > x_2) {temp=x_1;x_1=x_2;x_2=temp;}
        dd  = x_2 - x_1;
        ligneHorizontale(surface, (int) (x_1)
                                , (int) (ii+y_haut_1)
                                , (int) (dd+1)
                                , couleur);
    }
}



/**************************************************************************************************
    Draw a horizontal line
**************************************************************************************************/
void ligneHorizontale(SDL_Surface *surface, int x, int y, int w, Uint32 couleur)
{
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = 1;

    SDL_FillRect(surface, &r, couleur);
}



/**************************************************************************************************
    Draw a vertical line
**************************************************************************************************/
void ligneVerticale(SDL_Surface *surface, int x, int y, int h, Uint32 couleur)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = 1;
  r.h = h;
    SDL_FillRect(surface, &r, couleur);
}
