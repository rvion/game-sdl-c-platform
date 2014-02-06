
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constantes.h"

double is_positive(double a) { if (a > 0) { return  1;} else { return  0;} }
double is_negative(double a) { if (a > 0) { return  0;} else { return  1;} }
double val_absolue(double a) { if (a > 0) { return  a;} else { return -a;} }

double sign_of    (double a) { if (fabs(a) < 0.01) 	{ return 0;}
							   else if(a > 0) 		{ return  1;}
							   else			   		{ return -1;}}

double r_min      (double a, double b) { if (a > b) { return  a;} else { return b;} }
double r_max      (double a, double b) { if (a > b) { return  a;} else { return b;} }

double pente_x    (double x1, double y1, double x2, double y2) { return ((y2-y1)/(x2-x1)); }
double pente_y    (double x1, double y1, double x2, double y2) { return ((x2-x1)/(y2-y1)); }

int my_random     (int x_min, int x_max) { return (x_min + rand() % (x_max-x_min)); }
int random_from_0 (int x)                { return (rand() % x); }


int point_in_square ( double xt, double yt
                    , double x1, double y1
                    , double x2, double y2)
{
    if ((xt >= x1)
    &&  (xt <= x2)
    &&  (yt >= y1)
    &&  (yt <= y2))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


int collision_squares( double x1, double y1
                     , double x2, double y2
                     , double x3, double y3
                     , double x4, double y4)
{
	if ( x1>x4 ) 				{ return FALSE;}
	else if (y1> y4 || y2 < y3) { return FALSE;}
	else 						{ return TRUE; }

	if ( x2<x3 ) 				{ return FALSE;}
	else if (y1> y4 || y2 < y3) { return FALSE;}
	else 						{ return TRUE; }

	if ( y1>y4 ) 				{ return FALSE;}
	else if (x1> x4 || x2 < y3) { return FALSE;}
	else 						{ return TRUE; }

	if ( y2<y3 ) 				{ return FALSE;}
	else if (x1> x4 || x2 < x3) { return FALSE;}
	else 						{ return TRUE; }

}
