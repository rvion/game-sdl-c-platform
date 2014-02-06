/*
 * ini_load.h
 *
 *  Created on: 29 nov. 2011
 *      Author: Arnaud
 */

#ifndef INI_LOAD_H_
#define INI_LOAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "ini.h"
#include "structures.h"
#include "constantes.h"


int ini_load_unique_polygon(Polygon* poly, char* section, FILE* iniFile);
int ini_load_polygon_pattern	(Polygon* poly, char* section, char* ini_name);
int load_square					(Polygon* poly, double origin_x, double origin_y, double side);
int load_triangle				(Polygon* poly, double origin_x, double origin_y, double side);

int is_section_present			(char** sections, char* section, int nb_sections);

int ini_load_map (Map* map, char* ini_name);



#endif /* INI_LOAD_H_ */
