/*
 * ini_load.c
 *
 *  Created on: 29 nov. 2011
 *      Author: Arnaud
 */

#include "ini_load.h"
#include "structures.h"
#include "polygons.h"
#include "math.h"
#include "ini.h"

/**
    Charge un Polygon depuis un fichier (Map ou librairie externe)
    Calcule les données manquantes si nécessaire
    aka bornes min/max + Triangulisation
*/

int ini_load_unique_polygon(Polygon* poly, char* section, FILE* iniFile)
{

	int nb_points;
	int i;

	poly->point_x = ini_read_array(iniFile, section, "point_x", NULL, &nb_points);
	if( poly->point_x == NULL)
	{
	    printf("coord X non trouvees\n");
	    return FALSE;
	}

	poly->point_y = ini_read_array(iniFile, section, "point_y", NULL, &nb_points);
	if(poly->point_y == NULL)
	{
        printf("coord Y non trouvees\n");
        return FALSE;
	}

    poly->nb_points = nb_points;
    printf("    Nb Points : %d\n", poly->nb_points);
	poly->solid 	= (int) ini_read_double(iniFile, section, "solid"   , 0);
	poly->type 	 	= (int) ini_read_double(iniFile, section, "type"    , 0);
	poly->bounce 	= (int) ini_read_double(iniFile, section, "bounce"  , 0);
	poly->alpha     = (int) ini_read_double(iniFile, section, "alpha"   , 255);

	poly->decor     = (int) ini_read_double(iniFile, section, "decor"   , 0);

	poly->precalculated_triangles = ini_read_double(iniFile ,section    , "precalculated_triangles" , 0);

	if(poly->precalculated_triangles)
	{
		poly->nb_triangles = ini_read_double(iniFile, section, "nb_triangles", 0);

		poly->triangle_x0 = ini_read_array(iniFile, section, "triangle_x0", NULL, &nb_points);
		poly->triangle_x1 = ini_read_array(iniFile, section, "triangle_x1", NULL, &nb_points);
		poly->triangle_x2 = ini_read_array(iniFile, section, "triangle_x2", NULL, &nb_points);

		poly->triangle_y0 = ini_read_array(iniFile, section, "triangle_y0", NULL, &nb_points);
		poly->triangle_y1 = ini_read_array(iniFile, section, "triangle_y1", NULL, &nb_points);
		poly->triangle_y2 = ini_read_array(iniFile, section, "triangle_y2", NULL, &nb_points);
	}
	else
	{
	    triangulate(poly);
	}

	poly->precalculated_bornes = ini_read_double(iniFile, section, "precalculated_bornes", 0);
	if(poly->precalculated_bornes)
	{
		poly->x_max = ini_read_double(iniFile, section, "x_max", 0);
		poly->x_min = ini_read_double(iniFile, section, "x_min", 0);
		poly->y_max = ini_read_double(iniFile, section, "y_max", 0);
		poly->y_min = ini_read_double(iniFile, section, "y_min", 0);
	}
	else
	{
	  	/*
		 * on trouve les bornes min et max
		 * en X et en Y de notre polygon
		 */
		poly->x_max = poly->point_x[0];
		poly->x_min = poly->point_x[0];

		poly->y_max = poly->point_y[0];
		poly->y_min = poly->point_y[0];

		for (i=1; i< poly->nb_points; i++)
		{
			if (poly->x_max < poly->point_x[i]) { poly->x_max = poly->point_x[i]; }
			if (poly->x_min > poly->point_x[i]) { poly->x_min = poly->point_x[i]; }

			if (poly->y_max < poly->point_y[i]) { poly->y_max = poly->point_y[i]; }
			if (poly->y_min > poly->point_y[i]) { poly->y_min = poly->point_y[i]; }
		}
		printf("Bornes calculees\n");
	}
	return TRUE;
}

/**
    Génere un Polygon Carré avec des données basiques
*/
int generate_square(Polygon* square, double side) {
	square->point_x = calloc(square->nb_points, sizeof(double));
	square->point_y = calloc(square->nb_points, sizeof(double));

	square->point_x[0] = 0		; square->point_y[0]  = 0;
	square->point_x[1] = side	; square->point_y[1] = 0;
	square->point_x[2] = side	; square->point_y[2] = side;
	square->point_x[3] = 0		; square->point_y[3] = side;

	square->x_max = square->origine_x+ side ; square->x_min = square->origine_x;
	square->y_max = square->origine_y + side; square->y_min = square->origine_y;

	square->triangle_x0 = calloc(square->nb_triangles, sizeof(double));
	square->triangle_x1 = calloc(square->nb_triangles, sizeof(double));
	square->triangle_x2 = calloc(square->nb_triangles, sizeof(double));

	square->triangle_y0 = calloc(square->nb_triangles, sizeof(double));
	square->triangle_y1 = calloc(square->nb_triangles, sizeof(double));
	square->triangle_y2 = calloc(square->nb_triangles, sizeof(double));

	triangulate(square);

	return TRUE;

}

/**
    Génere un Cercle avec des données de base
*/
int generate_circle(Circle* circle) {

	circle->precalculated_bornes = TRUE;

	circle->x_max = circle->origine_x + circle->radius;
	circle->x_min = circle->origine_x - circle->radius;

	circle->y_max = circle->origine_y + circle->radius;
	circle->y_min = circle->origine_y - circle->radius;

	return TRUE;
}

/**
    Teste l'existence d'une Section
*/

int is_section_present(char** sections, char* section, int nb_sections) {
	int i;
	for(i = 0; i < nb_sections; i++)
		if(same_string(sections[i], section) == TRUE)
			return TRUE;
	return FALSE;
}

/*******************************************************************************************
    FONCTION DE CHARGEMENT DE MAP
    Analyse le fichier de Map pour trouver des informations de bases
        --> Infos Générales
        --> Liste de Carrés
        --> Liste de Cercles
        --> Liste des zones de Gravités Variables
    Analyse la suite du fichier pour trouver des Sections supplémentaires
        --> Peut lire les informations de Polygons directement en local
        --> Peut lire les informations de Polygons en externe si la key "lib" est présente
            Dans ce cas il va la chercher dans le dossier \DB\lib
        --> Peut lire plusieurs occurence du meme Polygon à != endroits
            si les keys origins_x & origins_y sont présentes

    ------> Remplit toutes les infos de la Map passée en pointeur

**/

int ini_load_map(Map* map, char* ini_name) {

	int i, j;
	int uniCount, decoCount;
	int nb_sections 	= 0;
	int a_changer	 	= 5;

	char* lib 	   = NULL;
	char* path2lib = NULL;
	FILE* libFile  = NULL;

	int count;

	double* origin_x = NULL;
	double* origin_y = NULL;
	double* side	 = NULL;
	double* radius	 = NULL;
	double* type	 = NULL;
	double* solid	 = NULL;

	double* grav_vx  = NULL;
	double* grav_vy  = NULL;
	double* grav_x1  = NULL;
	double* grav_y1  = NULL;
	double* grav_x2  = NULL;
	double* grav_y2  = NULL;

	double* pieces_x = NULL;
    double* pieces_y = NULL;

    int*    pieces_prises = NULL;
    int     nb_pieces     = 0;

	char*   name = NULL;

	Polygon* 	  squares 	      = NULL;
	TripletPoly*  polygons 		  = NULL;
	TripletPoly*  polygons_decor  = NULL;
	Circle* 	  circles		  = NULL;
	Polygon* 	  unique_polygons = NULL;
	

	FILE* iniMap = ini_open(ini_name);
	char** sections = ini_get_ALL_the_sections(iniMap, &nb_sections);
	unique_polygons = calloc(nb_sections - a_changer, sizeof(Polygon));

	printf("\n\n");

    /** SQUARES*/
	if(is_section_present(sections, "SQUARE", nb_sections))
	{
		origin_x = ini_read_array(iniMap, "SQUARE", "origin_x", NULL, &count);
		origin_y = ini_read_array(iniMap, "SQUARE", "origin_y", NULL, &count);
		side 	 = ini_read_array(iniMap, "SQUARE", "side"	  , NULL, &count);
		type 	 = ini_read_array(iniMap, "SQUARE", "type"	  , NULL, &count);
		solid 	 = ini_read_array(iniMap, "SQUARE", "solid"   , NULL, &count);

		map->nb_squares = count;
		printf("Nombre de square = %d\n\n", (int)count);
		squares = calloc(count, sizeof(Polygon));

		for(i = 0; i < count; i++)
		{
			squares[i].nb_points = 4;
			squares[i].origine_x = origin_x[i];
			squares[i].origine_y = origin_y[i];
			squares[i].solid = solid[i];
			squares[i].type = type[i];
			generate_square(&squares[i], side[i]);
		}
	}
    /** Gravity*/
	if(is_section_present(sections, "GRAVITY", nb_sections))
	{
		grav_vx  = ini_read_array(iniMap, "GRAVITY", "grav_vx", NULL, &count);
		grav_vy  = ini_read_array(iniMap, "GRAVITY", "grav_vy", NULL, &count);
		grav_x1  = ini_read_array(iniMap, "GRAVITY", "grav_x1", NULL, &count);
		grav_y1  = ini_read_array(iniMap, "GRAVITY", "grav_y1", NULL, &count);
		grav_x2  = ini_read_array(iniMap, "GRAVITY", "grav_x2", NULL, &count);
		grav_y2  = ini_read_array(iniMap, "GRAVITY", "grav_y2", NULL, &count);
		map->nb_gravs = count;
	}

	/** Pieces*/
	if(is_section_present(sections, "PIECES", nb_sections))
	{
		pieces_x      = ini_read_array(iniMap, "PIECES", "pieces_x", NULL, &nb_pieces);
		pieces_y      = ini_read_array(iniMap, "PIECES", "pieces_y", NULL, &nb_pieces);
		pieces_prises =  calloc(nb_pieces, sizeof(int));
	}

    /** CIRCLES*/
	if(is_section_present(sections, "CIRCLE", nb_sections))
	{
		origin_x = ini_read_array(iniMap, "CIRCLE", "origin_x"	, NULL, &count);
		origin_y = ini_read_array(iniMap, "CIRCLE", "origin_y"	, NULL, &count);
		radius 	 = ini_read_array(iniMap, "CIRCLE",  "radius"	, NULL, &count);
		type 	 = ini_read_array(iniMap, "CIRCLE",  "type"		, NULL, &count);
		solid 	 = ini_read_array(iniMap, "CIRCLE",  "solid"	, NULL, &count);

		circles = calloc(count, sizeof(Circle));
		map->nb_circles = count;
		printf("Nombre de circles = %d\n\n", (int)count);

		for (i = 0; i < count; i++) {
			circles[i].origine_x = origin_x[i];
			circles[i].origine_y = origin_y[i];
			circles[i].solid = solid[i];
			circles[i].type = type[i];
			circles[i].radius = radius[i];
			generate_circle(&circles[i]);
		}
	}

    /** CHARGEMENT DES POLYGONS UNIQUES*/
	uniCount    = 0;
	decoCount   = 0;
	path2lib    = calloc(LINE_LENGTH, sizeof(char));
	path2lib[0] = '\0';

	for(i = a_changer; i < nb_sections; i++)
	{
		name = ini_read_string(iniMap, sections[i], "name", NULL);
		lib  = ini_read_string(iniMap, sections[i], "lib", NULL);

        /**Cherche si les infos sont externalisées*/
        if(lib != NULL)
        {
            strcpy(path2lib, DB_PATH);
            strcat(path2lib, lib);
            printf("Chemin de la librairie : %s\n", path2lib);

            libFile = ini_open(path2lib);
            ini_load_unique_polygon(&unique_polygons[i - a_changer], name, libFile);
            ini_close(libFile);
			free(lib);
			free(name);
        }
        /**Sinon chargement depuis le fichier initial*/
        else
        {
            printf("Chargement direct depuis la map\n");
            ini_load_unique_polygon(&unique_polygons[i - a_changer], sections[i], iniMap);
        }

        origin_x = ini_read_array(iniMap, sections[i], "origin_x", NULL, &count);
        origin_y = ini_read_array(iniMap, sections[i], "origin_y", NULL, &count);

        if(origin_x == NULL ||origin_y == NULL)
            count = 1;

        if(unique_polygons[i - a_changer].decor == 0)
        {
            printf("(realloc.." );
            polygons = realloc(polygons, (uniCount + count) * sizeof(TripletPoly) );
            printf("done)\n" );

            for(j = 0; j < count; j++)
            {
                polygons[uniCount + j].polygon  = &unique_polygons[i - a_changer];

                polygons[uniCount + j].x_min  = unique_polygons[i - a_changer].x_min;
                polygons[uniCount + j].y_min  = unique_polygons[i - a_changer].y_min;
                polygons[uniCount + j].x_max  = unique_polygons[i - a_changer].x_max;
                polygons[uniCount + j].y_max  = unique_polygons[i - a_changer].y_max;

                polygons[uniCount + j].optimisation=calloc(unique_polygons[i - a_changer].nb_points, sizeof(int));

                if(ini_read_double(iniMap, sections[i], "absolute", 1) == 0)
                {
                    polygons[uniCount + j].origin_x = origin_x[j];
                    polygons[uniCount + j].origin_y = origin_y[j];
                    free(origin_x);
                    free(origin_y);
                }
                else
                {
                    polygons[uniCount + j].origin_x = 0;
                    polygons[uniCount + j].origin_y = 0;
                }

            }

            uniCount += count;
        }
        else
        {
            printf("(realloc.." );
            polygons_decor = realloc(polygons_decor, (decoCount + count) * sizeof(TripletPoly) );
            printf("done)\n" );

            for(j = 0; j < count; j++)
            {
                polygons_decor[decoCount + j].polygon  = &unique_polygons[i - a_changer];

                polygons_decor[decoCount + j].x_min  = unique_polygons[i - a_changer].x_min;
                polygons_decor[decoCount + j].y_min  = unique_polygons[i - a_changer].y_min;
                polygons_decor[decoCount + j].x_max  = unique_polygons[i - a_changer].x_max;
                polygons_decor[decoCount + j].y_max  = unique_polygons[i - a_changer].y_max;

                polygons_decor[decoCount + j].optimisation=calloc(unique_polygons[i - a_changer].nb_points, sizeof(int));

                if(ini_read_double(iniMap, sections[i], "absolute", 1) == 0)
                {
                    polygons_decor[decoCount + j].origin_x = origin_x[j];
                    polygons_decor[decoCount + j].origin_y = origin_y[j];
                    free(origin_x);
                    free(origin_y);
                }
                else
                {
                    polygons_decor[decoCount + j].origin_x = 0;
                    polygons_decor[decoCount + j].origin_y = 0;
                }
            }

            decoCount += count;
        }
		printf("\n\n");
	}

	printf("\n TRANSFERT DES INFORMATIONS CHARGEES..\n" );

	map->nb_polygons    = uniCount;
	map->nb_polygons_decor  = decoCount;
	map->grav           = ini_read_double(iniMap, "GENERAL", "grav"   , 0);
	map->height         = ini_read_double(iniMap, "GENERAL", "height" , 0);
	map->width          = ini_read_double(iniMap, "GENERAL", "width"  , 0);

	map->polygons       = polygons;
	map->polygons_decor = polygons_decor;

	map->squares  = squares;
	map->circles  = circles;

	map->pieces_x = pieces_x;
    map->pieces_y = pieces_y;
    map->pieces_prises   = pieces_prises;
    map->nb_pieces       = nb_pieces;

    map->unique_polygons = unique_polygons;

	map->grav_vx  = grav_vx;
	map->grav_vy  = grav_vy;
	map->grav_x1  = grav_x1;
	map->grav_y1  = grav_y1;
	map->grav_x2  = grav_x2;
	map->grav_y2  = grav_y2;
	printf("MAP CHARGEE !\n" );

	free(path2lib);

    for(i = 0; i < nb_sections; i++)
        free(sections[i]);

    free(sections);

	
	return TRUE;
}
