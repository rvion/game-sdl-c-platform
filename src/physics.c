
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "geometry.h"
#include "constantes.h"
#include "basic_maths.h"
#include "structures.h"

#include "physics.h"

/*
	A chaque step, le Joueur doit se déplacer de vitesse unitée de déplacement.
	Unitée de déplacement après unitées de déplacement, nous déplacons le Joueur,
	et calculons la distance du Joueur à chaque segment de chaque polygon. 
	Pour ce faire, nous projetons le joueur sur le droite directrice du segment testé. 
	Si la projection se trouve sur le segment, la distance du Joueur à ce dernier est 
	par conséquent égal à sa distance de la projection. Si la projection n'est pas sur 
	le segment, nous calculons de quel coté du segment elle se trouve, et la distance 
	du player est par conséquent Sa distance au sommet du côté ou il se trouve. 
	
	Si la distance que l'on vient de calculer est inférieure à rayonPlayer, il y a collision. 
	Il faut donc repositionner le Joueur 1 unitée de déplacement avant 
	( dernière bonne position sans collision) et changer la direction et la norme du player.
		Nous distingons deux cas: 
			Cas 1 : La collision se fait sur un sommet: Nous projettons la vitesse et la 
			vitesse_restante_a_parcourir du Joueur sur l'axe perpendiculaire au segment 
			d'extrémités [le centre du joueur, le sommet] 
			
			Cas 2 : la collision se fait sur une arète: Nous projetteons la vitesse et la 
			vitesse_restante_a_parcourir  du Joueur sur le segment.
		Notons que les norme de la vitesse est évidement diminuée dans l'opération. 
	
	S'il n'y a eu aucune collision avec l'ensemble des polygones, nous considérons donc 
	la nouvelle position du joueur comme sûr, et nous diminuons de 1 unitée de déplacement 
	vitesse_restante_a_parcourir.
	
	Nous répétons donc cet algorithme tant que la vitesse_restante_a_parcourir du joueur n'est pas 0 (ou négligeable)
	Cette méthode permet de gérer plusieurs collisions qui surviennent lors du déplacement d'une seule unitée de déplacement.
*/
void update_player(Player* player, Map * map)
{
    double debugg=0;
    double x1 		= player->x; /* emplacement initial du joueur en X*/
    double y1 		= player->y; /* emplacement initial du joueur en Y */
    Polygon* poly	= NULL;
    double x2 		= player->x + player->v_x;
    double y2 		= player->y + player->v_y;
    double dist 	= 0;
    
	double a,b,c;
    
	int ttest;
	
    double x_modif, y_modif;
    double sens_x, sens_y;
    
	double range;
    double col_x; /* point de collision en X */
    double col_y; /* point de collision en Y */
    
	int col_type;
    int temp;                   /* variables temporaires pour eventuelles permutations */
    int i, j, zz, jpp, jmm;     /* Quelques indices */
    int cc;

    player->on_ground       = FALSE;
    player->on_left_wall    = FALSE;
    player->on_right_wall   = FALSE;

    for (zz=0; zz<map->nb_polygons; zz++)
    {
        poly = map->polygons[zz].polygon;
        for (j=0; j<poly->nb_points; j++)
        {
            map->polygons[zz].optimisation[j]=0;
        }
    }

    while (TRUE == TRUE)
    {
        /* On calcule les valeurs initiales des
		Variables nécessaires à la gestion de la physique. */
        temp  = 0;
        ttest = 0;

        update_a_modifs ( &sens_x
                        , &sens_y
                        , (x2-x1)
                        , (y2-y1)
                        , &x_modif
                        , &y_modif
                        , &range);

        /* Pour chaque unité de déplacement à effectuer = vitesse */
        for (i=1; i<=range; i++)
        {
            /* on teste la collision avec des cercles  */
                for(cc = 0; cc < map->nb_circles; cc++)
                {	
					/*Pour chaque cercle */
                    if (map->circles[cc].visible == TRUE)
                    {
						/* si le cercle est visible, c.a.d, proche du player,
						on calcule la distance 'dist' jusqu'au cercle	*/
                        dist = sqrt ( (map->circles[cc].origine_x - x1)
                                    * (map->circles[cc].origine_x - x1)
                                    + (map->circles[cc].origine_y - y1)
                                    * (map->circles[cc].origine_y - y1));
                        printf("dist = %f\n",dist);
                        if (dist > player->rc + map->circles[cc].radius + 2*range + 1)
                        {
							/* Si le cercle est trop loin, on le désactive */
                            map->circles[cc].visible = FALSE;
                        }
                        else if (dist < player->rc + map->circles[i].radius)
                        {
							/* si le  cercle est trop près, on le collisionne */
                            ttest =1;
                            debugg+=1;

                            col_x   = ( x1 * player->rc + map->circles[cc].radius * map->circles[cc].origine_x)
                                    / (      player->rc + map->circles[cc].radius);
                            col_y   = ( y1 * player->rc + map->circles[cc].radius * map->circles[cc].origine_y)
                                    / (      player->rc + map->circles[cc].radius);

                            if (debugg>20) {printf("%f\n",debugg);return;}
                            if ((col_y+1)>y1)
                            {
                                if      (col_x > (x1 + (player->r * 0.9)))  { player->on_right_wall = TRUE; }
                                else if (col_x < (x1 - (player->r * 0.9)))  { player->on_left_wall  = TRUE; }
                                else                                        { player->on_ground     = TRUE; }
                            }

							/* mémoire pour projeter la vitesse du joueur */
                            a = sqrt(player->v_x * player->v_x + player->v_y * player-> v_y);
                            b = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

                            projette_vecteur( x1  , y1
                                            , &x2 , &y2
                                            , x1  , y1
                                            , x1 -(col_y-y1)
                                            , y1 +(col_x-x1));

                            c = a/b;

							/* On projette la vitesse du joueur */
                            player->v_x = (x2-x1) * c;
                            player->v_y = (y2-y1) * c;

                        }
                    }
                }

            /* on teste la collision avec des polygons */
            if (ttest==0)
            for (zz=0; zz<map->nb_polygons; zz++)
            {
            	poly = map->polygons[zz].polygon;
            	
            	if (map->polygons[zz].visible == TRUE && ttest ==0)
            	for (j=0; j<poly->nb_points; j++)
				{
				    if (map->polygons[zz].optimisation[j]==0)
				    {
                        jpp = (j+1) % poly->nb_points;
                        jmm = (poly->nb_points+j-1) % poly->nb_points;

                        dist = distance_point_segment   ( x1+x_modif
                                                        , y1+y_modif
                                                        , poly->point_x[j]  , poly->point_y[j]
                                                        , poly->point_x[jpp], poly->point_y[jpp]
                                                        , &col_x
                                                        , &col_y
                                                        , &col_type);
                        if (dist > player->rc + 2*range + 1)
                        {
							/* Si la distance au segment est trop faible 
							On desactive le segment pour le reste du step*/
                            map->polygons[zz].optimisation[j]=1;
                        }
                        else if (dist < player->rc)
                        {
							/* S'il y a collision */
                            ttest =1;
                            debugg+=1;

                            if (debugg>20) {printf("%f\n",debugg);return;}
                            if ((col_y+1)>y1)
                            {
								/* si la collision se situe sous la ceinture du player*/
                                if      (col_x > (x1 + (player->r * 0.9)))  { player->on_right_wall = TRUE; }
                                else if (col_x < (x1 - (player->r * 0.9)))  { player->on_left_wall  = TRUE; }
                                else                                        { player->on_ground     = TRUE; }
                            }

                            a = sqrt(player->v_x * player->v_x + player->v_y * player-> v_y);
                            b = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
							
							/* on projette notre vitesse sur la surface collisionnée*/
								if (col_type == ARRETE)
								{
									/*Cas: la collision se fait sur une arète: Nous projetteons 
									la vitesse et la vitesse_restante_a_parcourir  du Joueur sur 
									le segment.*/
									projette_vecteur( x1                , y1
													, &x2               , &y2
													, poly->point_x[j]   , poly->point_y[j]
													, poly->point_x[jpp] , poly->point_y[jpp]);

								}
								else if (is_clockwise( poly->point_x[jmm], poly->point_y[jmm] 
													, poly->point_x[j]  , poly->point_y[j]
													, poly->point_x[jpp], poly->point_y[jpp]) != CLOCKWISE)

								{
																		/*Cas: La collision se fait sur un sommet: 
									Nous projettons la vitesse et la vitesse_restante_a_parcourir 
									du Joueur sur l'axe perpendiculaire au segment d'extrémités 
									[le centre du joueur, le sommet] */
									projette_vecteur( x1  , y1
													, &x2 , &y2
													, x1  , y1
													, x1 -(col_y-y1)
													, y1 +(col_x-x1));

								}
							/* et on décellère car de l'énergie est absorbée lors de la collision */
                            c = a/b;
							
							/* On projette la vitesse du joueur */
                            player->v_x = (x2-x1) * c;
                            player->v_y = (y2-y1) * c;

                        }

                        else if (dist < player->rc+1)
                        {
							/* on teste si le player est contre un mur */
                            if ((col_y+1)>y1)
                            {
                                if      (col_x > (x1 + (player->r * 0.9)))  { player->on_right_wall = TRUE; }
                                else if (col_x < (x1 - (player->r * 0.9)))  { player->on_left_wall  = TRUE; }
                                else                                        { player->on_ground     = TRUE; }
                            }
                        }
				    }
				}
			}
			if (ttest==0)
            {
				/* On vient de finir un déplacement d'un pixel */
                x1+= x_modif;
                y1+= y_modif;

                for(cc = 0; cc < map->nb_pieces; cc++)
                {
					/* pour chaque pièce */
                    if (sqrt( (map->pieces_x[cc] - x1)
                            * (map->pieces_x[cc] - x1)
                            + (map->pieces_y[cc] - y1)
                            * (map->pieces_y[cc] - y1)) < (player->rc + 5)
                        && map->pieces_prises[cc] == 0)
                    {
						/* si on est assez près et que
						la pièce n'a pas encore été mangée,
						On la mange. */
                        map->pieces_prises[cc] =1;
                        player->pieces ++;
                    }
                }

                if (range==i)
                {
					/* si on est arrivé à terme de notre déplacement,
					c'est finit pour ce step. */
					
                    player->x= x1;
                    player->y= y1;
                    return;
                }

            }
        }
    }
    return ;

}

/*
 * Cette fonction update la position du joueur en fonction de
 * Sa vitesse et des différents obstacles sur la map
 */
 void update_a_modifs(double *sens_x, double *sens_y, double v_x, double v_y, double *x_modif, double *y_modif, double * range)
{
    *sens_x = sign_of(v_x);
    *sens_y = sign_of(v_y);

    if ( (fabs(v_x)<0.01)             /* (x2 == x1) */
    &&   (fabs(v_y)>0.01) )           /* (y2 != y1) */
    {
        if   (fabs(v_y)>1)
        {
            *x_modif = 0;
            *y_modif = *sens_y;
            *range   = fabs(v_y);
        }
        else
        {
            *x_modif = 0;
            *y_modif = v_y;
            *range   = 1;
        }
    }

    else if ( (fabs(v_x)>0.01)        /* (x2 != x1) */
         &&   (fabs(v_y)<0.01) )      /* (y2 == y1) */
    {
        if   (fabs(v_x)>1)
        {
            *x_modif = *sens_x;
            *y_modif = 0;
            *range   = fabs(v_x);
        }
        else
        {
            *x_modif = v_x;
            *y_modif = 0;
            *range   = 1;
        }
    }
    else                                /* (x2 != x1) */
    {                                   /* (y2 != y1) */
        if (fabs(v_y) > fabs(v_x))
        {
            if   (fabs(v_y) > 1)
            {
                *x_modif = *sens_x *fabs(v_x/v_y);
                *y_modif = *sens_y;
                *range   = fabs(v_y);
            }
            else
            {
                *x_modif = v_x;
                *y_modif = v_y;
                *range   = 1;
            }
        }
        else
        {
            if   (fabs(v_x) > 1)
            {
                *x_modif = *sens_x;
                *y_modif = *sens_y * fabs(v_y/v_x);
                *range   = fabs(v_x);
            }
            else
            {
                *x_modif = v_x;
                *y_modif = v_y;
                *range   = 1;
            }
        }
    }
}
