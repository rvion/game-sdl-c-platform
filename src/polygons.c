#include <stdio.h>
#include <stdlib.h>

#include "polygons.h"
#include "geometry.h"
#include "constantes.h"
#include "structures.h"
#include "basic_maths.h"

void triangulate(Polygon *poly)
{
    /*-----------------------------------------------------------
    |                                                           |
    |  Le polygone est donné counter clockwise                  |
    |  Le premier point est considéré comme le dernier aussi    |
    |  Retourne n-2 triangles pour n points                     |
    |                                                           |
    -----------------------------------------------------------*/

    int i=0;
    int j=0;
    int v=0;

    int verif=0;
	double* temp_point_x = NULL;
	double* temp_point_y = NULL;

    int polygonSize = poly->nb_points;
    int i1, i2, i3;
    int n, k;

    int triangles_founded=0;

    double x0, x1, x2, x3, x4;
    double y0, y1, y2, y3, y4;

    int good = FALSE;
   	
	poly->nb_triangles = (poly->nb_points-2);
	poly->triangle_x0 = malloc( sizeof(double) * poly->nb_triangles);
	poly->triangle_x1 = malloc( sizeof(double) * poly->nb_triangles);
	poly->triangle_x2 = malloc( sizeof(double) * poly->nb_triangles);
	poly->triangle_y0 = malloc( sizeof(double) * poly->nb_triangles);
	poly->triangle_y1 = malloc( sizeof(double) * poly->nb_triangles);
	poly->triangle_y2 = malloc( sizeof(double) * poly->nb_triangles);

    temp_point_x = malloc( poly->nb_points * sizeof(double));
    temp_point_y = malloc( poly->nb_points * sizeof(double));

    for (n=0; n< poly->nb_points; n++)
    {
        temp_point_x[n] = poly->point_x[n];
        temp_point_y[n] = poly->point_y[n];
    }


    for (n = polygonSize; n > 3; n--)
    {
        for (k=0; k< n; k++)
        {
            i1 = k;
            i2 = (k+1)%n;
            i3 = (k+2)%n;

            x0 = temp_point_x[i1];
            x1 = temp_point_x[i2];
            x2 = temp_point_x[i3];

            y0 = temp_point_y[i1];
            y1 = temp_point_y[i2];
            y2 = temp_point_y[i3];

            if (is_clockwise(x0, y0, x1, y1, x2, y2) != TRUE)
            {
                good = TRUE;

                for (i = 0; i < n; i += 1)
                {
                    if ((i != i1) &&  (i != i2) &&  (i != i3))
                    {
                        x3 = temp_point_x[i];
                        y3 = temp_point_y[i];

                        /*  ...and if triangle has no vertices within it... */
                        if (point_in_triangle(x0, y0, x1, y1, x2, y2, x3, y3) == TRUE)
                            { good = FALSE; i=n;}
                        else
                        {
                            /* and if the new edge has no other edges crossing it... */
                            j = (i+1)%n;
                            if ((j != i1) &&  (j != i2) &&  (j != i3))
                            {
                                x4 = temp_point_x[j];
                                y4 = temp_point_y[j];
                                if (intersect_lines2( x0, y0, x2, y2
                                                    , x3, y3, x4, y4) == TRUE)
                                {
                                    good = FALSE;
                                    i=n;
                                }
                            }
                        }
                    }
                }
                /*  e.  ...then add the triangle to list and remove the shared outside edge point */
                if (good == TRUE)
                {
                    poly->triangle_x0[triangles_founded] = x0;
                    poly->triangle_y0[triangles_founded] = y0;
                    poly->triangle_x1[triangles_founded] = x1;
                    poly->triangle_y1[triangles_founded] = y1;
                    poly->triangle_x2[triangles_founded] = x2;
                    poly->triangle_y2[triangles_founded] = y2;
                    verif ++;

                    for (v=i2; v<(polygonSize-1); v++)
                    {
                        temp_point_x[v] = temp_point_x[v+1];
                        temp_point_y[v] = temp_point_y[v+1];
                    }

                    triangles_founded++;
                    k = n;
                }
            }
        }
    }
    /*-----------------------------------------------
    |  les 3 derniers points forment un dernier     |
    |  triangle que l'on ajoute à la liste          |
    -----------------------------------------------*/
    poly->triangle_x0[triangles_founded] = temp_point_x[0];
    poly->triangle_y0[triangles_founded] = temp_point_y[0];
    poly->triangle_x1[triangles_founded] = temp_point_x[1];
    poly->triangle_y1[triangles_founded] = temp_point_y[1];
    poly->triangle_x2[triangles_founded] = temp_point_x[2];
    poly->triangle_y2[triangles_founded] = temp_point_y[2];
    verif ++;

    if (poly->nb_triangles == verif)
    {
        poly->precalculated_triangles = TRUE;
        printf("triangulation done: %d triangles\n",poly->nb_triangles);
    }
    else
    {
        printf("error while triangualte : verif = %d, poly->nb_triangles =%d\n",verif,poly->nb_triangles);
    }

    free(temp_point_x);
    free(temp_point_y);
}

