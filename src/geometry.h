#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include "structures.h"

double distance_point_segment   ( double cx,    double cy       /* point C dont on veut connaitre la distance a[AB]     */
                                , double ax,    double ay       /* point A de [AB]                                      */
                                , double bx,    double by       /* point B de [AB]                                      */
                                , double *colx, double *coly    /* Coordonées de la projection de C sur AB ou du coin   */
                                , int *collision);              /* retourne le type de collision: COIN ou ARRETE        */


int intersect_lines2            ( double x0, double y0
                                , double x1, double y1
                                , double x2, double y2
                                , double x3, double y3);

int intersect_lines             ( double x1, double y1
                                , double x2, double y2
                                , double x3, double y3
                                , double x4, double y4
                                , double *x, double *y);

int point_in_triangle           ( double x0, double y0
                                , double x1, double y1
                                , double x2, double y2
                                , double x3, double y3);

int point_in_triangle2          ( double x0, double y0
                                , double x1, double y1
                                , double x2, double y2
                                , double x3, double y3);

int is_clockwise                ( double x0, double y0
                                , double x1, double y1
                                , double x2, double y2);
                                
void projette_vecteur           ( double  x1, double  y1
                                , double *x2, double *y2
                                , double  x3, double  y3
                                , double  x4, double  y4);
                               

void remove_vecteur             ( double  x1, double  y1
                                , double *x2, double *y2
                                , double  x3, double  y3
                                , double  x4, double  y4);
#endif
