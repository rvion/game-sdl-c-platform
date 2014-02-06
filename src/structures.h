#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED


typedef struct struc_Input
{
    int up, hold_up;
    int down;
    int left;
    int right;

    int ac1;
    int ac2;

} Input;

typedef struct struct_Polygon
{
    double origine_x;
    double origine_y;

    int     nb_points;
    double* point_x;
    double* point_y;

    int     precalculated_triangles;

    int     nb_triangles;

    double* triangle_x0;
    double* triangle_x1;
    double* triangle_x2;

    double* triangle_y0;
    double* triangle_y1;
    double* triangle_y2;

    int     type;
    int     solid;
    int     bounce;
    int     alpha;
    int     decor;

    int     precalculated_bornes;

    double  x_max;
    double  x_min;
    double  y_max;
    double  y_min;

}Polygon;


typedef struct struct_camera
{
    double x       , y;
    double x_target, y_target;
    double dx_min_1, dx_min_2;
    double dy_min_1, dy_min_2;
    double vit_x   , vit_y;

} Camera;

typedef struct struc_Player
{
   
    int on_segment;
    int pieces;

    double d, r, rc;
    double a_x, a_y;

    double v_x, v_y;
    double max_v_x;
    double max_v_y;

    double x,y;

    double angle;
    double jump_force;
    double wall_jump_force;


    int on_ground;
    int on_left_wall;
    int on_right_wall;

    Input input;

} Player;

typedef struct s_triplet_poly
{
	Polygon* polygon;
	int*    optimisation;

	double origin_x;
	double origin_y;

	int visible;
	double  x_max;
    double  x_min;
    double  y_max;
    double  y_min;

}TripletPoly;

typedef struct s_circle
{
	double origine_x;
	double origine_y;

	double radius;

	int     type;
	int     solid;
    int     visible;

    int     precalculated_bornes;
    double  x_max;
    double  x_min;
    double  y_max;
    double  y_min;
}Circle;

typedef struct s_map
{
	double 	grav;				/*Gravite generale*/
	int 	width;				/*Largeur de la map*/
	int 	height;				/*Hauteur de la map*/
	int 	global_count;		/*Nombre total de Polygones*/

	int nb_unique_polygons;		/*Nombre de polygones uniques*/
	int nb_circles;				/*Nombre de cercles*/
	int nb_squares;				/*Nombre de carres*/
	int nb_polygons;			/*Nombre de Polygones 'Physiques'*/
	int nb_polygons_decor;		/*Nombre de Polygones de decor*/
	int nb_gravs;				/*Nombre de Zones de Gravite Variable*/
	int nb_pieces;				/*Nombre de Pieces a manger*/

	Polygon* unique_polygons;	/*Liste de Polygones Uniques*/
	Circle* circles;			/*Liste de Cercles*/
	Polygon* squares;			/*Liste de Carres*/
	TripletPoly* polygons;		/*Liste de Polygones 'Physiques'*/
	TripletPoly* polygons_decor;	/*Liste de Polygones de decor*/

    double* pieces_x;			/*Position des Pieces*/
    double* pieces_y;
    int* pieces_prises;

	double* grav_vx;			/*Zones de gravite + Valeur de la nouvelle gravite*/
	double* grav_vy;
	double* grav_x1;
	double* grav_y1;
	double* grav_x2;
	double* grav_y2;

}Map;


#endif
