
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constantes.h"
#include "basic_maths.h"
#include "structures.h"

/*
 * update la camera:
 * toutes les informations necessesaires sont :
 *    camera.target_x
 *    camera.target_y
 */
void update_camera(Camera* camera, int w, int h)
{

    camera->x = camera->x_target - SCREEN_W/2;
    camera->y = camera->y_target - SCREEN_H/2;


    if (camera->x <=0)              { camera->x = 0; }
    if (camera->x >= w-SCREEN_W)    { camera->x = w-SCREEN_W; }

    if (camera->y <=0)              { camera->y =0; }
    if (camera->y >= h-SCREEN_H)    { camera->y = h-SCREEN_H; }

}
