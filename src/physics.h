#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "structures.h"

void update_a_modifs(double *sens_x, double *sens_y, double v_x, double v_y, double *x_modif, double *y_modif, double * range);
void update_player(Player* player, Map* map);

#endif
