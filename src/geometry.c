
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "geometry.h"
#include "constantes.h"
#include "basic_maths.h"
#include "structures.h"


void remove_vecteur     ( double  x1,  double  y1
                        , double *x2,  double *y2
                        , double  x3,  double  y3
                        , double  x4,  double  y4)
{
    double x_temp, y_temp;
    double pdp = ((*x2-x1)*(x4-x3) + (*y2-y1)*(y4-y3)) / ((x4-x3)*(x4-x3)+(y4-y3)*(y4-y3));
        
    x_temp = (x4-x3) * pdp;
    y_temp = (y4-y3) * pdp;
    printf("%f, %f\n",(*x2-x1), (*y2-y1));
    printf("%f, %f\n\n",x_temp, y_temp);
    *x2 -= x_temp;
    *y2 -= y_temp;
}


void projette_vecteur   ( double x1, double y1
                        , double *x2, double *y2
                        , double x3,  double y3
                        , double x4,  double y4)
{
    double pdp = ((*x2-x1)*(x4-x3) + (*y2-y1)*(y4-y3)) / ((x4-x3)*(x4-x3)+(y4-y3)*(y4-y3));
        
    *x2 = x1 + (x4-x3) * pdp;
    *y2 = y1 + (y4-y3) * pdp;
}

double distance_point_segment( double cx, double cy
                             , double ax, double ay
                             , double bx, double by
                             , double *colx
                             , double *coly
                             , int    *collision)
{
    /* 
     * distance entre point   C    (cx, cy) 
     * et             segment [AB] (ax, ay) -> (bx, by).
     * P (px, py)   est la   PROJECTION   de C sur AB.  
     * r indique la position de P sur AB
     */


    double distance_line;
    double distance_segment;

    double r_numerator   = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
    double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
    double r             = r_numerator / r_denomenator;

    double px = ax + r*(bx-ax);
    double py = ay + r*(by-ay);

    double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay)) / r_denomenator;

    distance_line = fabs(s) * sqrt(r_denomenator);

    *colx = px;
    *coly = py;

    if ( (r >= 0) && (r <= 1) )
    {
            distance_segment = distance_line;
            *collision = ARRETE;
    }
    else
    {
        double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
        double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
        if (dist1 < dist2)
        {
                *colx = ax;
                *coly = ay;
                *collision = COIN;
                distance_segment = sqrt(dist1);
        }
        else
        {
                *colx = bx;
                *coly = by;
                *collision = COIN;
                distance_segment = sqrt(dist2);
        }
    }
    return fabs(distance_segment);
}

int intersect_lines2( double x0, double y0
                    , double x1, double y1
                    , double x2, double y2
                    , double x3, double y3)
{
    if ((is_clockwise(x0, y0, x1, y1, x2, y2) != is_clockwise(x0, y0, x1, y1, x3, y3))
    &&  (is_clockwise(x2, y2, x3, y3, x0, y0) != is_clockwise(x2, y2, x3, y3, x1, y1)))
         { return 1; }
    else { return 0; }
}
/*
   Determine the intersection point of two line segments
   Return FALSE if the lines don't intersect
*/
int intersect_lines( double x1, double y1
                   , double x2, double y2
                   , double x3, double y3
                   , double x4, double y4
                   , double *x, double *y)
{
   double mua, mub;
   double denom, numera, numerb;

   denom  = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1);
   numera = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
   numerb = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);

   /* Are the line coincident? */
   if ( numera==0 && numerb==0 && denom==0 )
   {
      *x = (x1 + x2) / 2;
      *y = (y1 + y2) / 2;
      return(TRUE);
   }

   /* Are the line parallel */
   if (denom==0)
   {
      return(FALSE);
   }

   /* Is the intersection along the the segments */
   mua = numera / denom;
   mub = numerb / denom;
   if (mua < 0 || mua > 1 || mub < 0 || mub > 1)
   {
      return(FALSE);
   }
   *x = x1 + mua * (x2 - x1);
   *y = y1 + mua * (y2 - y1);
   return(TRUE);
}




    /*-----------------------------------------------
    |  les points sont en counter-clockwise order!  |
    |  x3 et y3 sont les coordonnées du point       |
    ------------------------------------------------*/
int point_in_triangle( double x0, double y0
                     , double x1, double y1
                     , double x2, double y2
                     , double x3, double y3)
{
    double a, b, invDenom ;
    double xv1, xv2, xv3;
    double yv1, yv2, yv3;
    double dot11, dot12, dot13, dot22, dot23;

    xv1 = x1 - x0;
    xv2 = x2 - x0;
    xv3 = x3 - x0;

    yv1 = y1 - y0;
    yv2 = y2 - y0;
    yv3 = y3 - y0;

    /* Calcule les produits scalaires */
    dot11 = xv1*xv1 + yv1*yv1;
    dot12 = xv1*xv2 + yv1*yv2;
    dot13 = xv1*xv3 + yv1*yv3;
    dot22 = xv2*xv2 + yv2*yv2;
    dot23 = xv2*xv3 + yv2*yv3;

    invDenom = 1 / (dot11 * dot22 - dot12 * dot12);
    a = (dot22 * dot13 - dot12 * dot23) * invDenom;
    b = (dot11 * dot23 - dot12 * dot13) * invDenom;

    if ((a>0) && (b>0) && (a+b<1))
         {return TRUE; }
    else {return FALSE;}
}

int point_in_triangle2( double x0, double y0
                      , double x1, double y1
                      , double x2, double y2
                      , double x3, double y3)
{

    if ( is_clockwise(x0,y0,x1,y1,x3,y3) == FALSE
    &&   is_clockwise(x1,y1,x2,y2,x3,y3) == FALSE
    &&   is_clockwise(x2,y2,x0,y0,x3,y3) == FALSE)
         {return TRUE; }
    else {return FALSE;}
}



int is_clockwise (double x0, double y0, double x1, double y1, double x2, double y2)
{
    /*---------------------------
    |   1 = clockwise           |
    |   0 = no solution         |
    |  -1 = contreclockwise     |
    ---------------------------*/
    
    /* 
     * Indique si le triangle passé en paramètre est 
     * donnée en sens clockwise, ou non
     */
    
    double m, b;

    if ((x0 != x1) || (y0 != y1))
    {
        if (fabs(x0-x1) < EPS) 
        {
            if ((x2 < x1) ^ (y0 > y1))           {return 1; }
            else                                 {return -1;}
        }
        else
        {
            m = ((y0 - y1) / (x0 - x1));
            b = y0 - m * x0;
            if ((y2 > (m * x2 + b)) ^ (x0 > x1)) { return 1; }
            else                                 { return -1;}
        }
    }
    return 0;
}

