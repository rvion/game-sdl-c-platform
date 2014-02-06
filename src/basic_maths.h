#ifndef BASIC_MATHS_H_INCLUDED
#define BASIC_MATHS_H_INCLUDED

double is_positive  (double a);
double is_negative  (double a);
double val_absolue  (double a);
double sign_of      (double a);

double r_min        (double a, double b);
double r_max        (double a, double b);

double pente_x      ( double x1, double y1
                    , double x2, double y2);

double pente_y      ( double x1, double y1
                    , double x2, double y2);

int my_random       (int x_min, int x_max);
int random_from_0   (int x);

int collision_squares( double x1, double y1
                     , double x2, double y2
                     , double x3, double y3
                     , double x4, double y4);


int point_in_square ( double xt, double yt
                    , double x1, double y1
                    , double x2, double y2);

#endif
