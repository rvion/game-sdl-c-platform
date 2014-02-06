#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

/*-----------------------
|     Constantes        |
-----------------------*/

#define PI 3.1415           /* Constante PI */

#define TRUE  1             /* Boolen TRUE  */
#define FALSE 0             /* Boolen FALSE */


#define EPS 0.001	      	/* Epsilon standard */

#define EPS1 0.1           	/* Epsilon double E-1 */
#define EPS2 0.01          	/* Epsilon double E-2 */
#define EPS3 0.001         	/* Epsilon double E-3 */
#define EPS4 0.0001       	/* Epsilon double E-4 */
#define EPS5 0.00001        /* Epsilon double E-5 */

#define COIN 0              /* Boolen FALSE */
#define ARRETE 1            /* Boolen FALSE */


#define SCREEN_W 1000       /* Largeur de l'ecran */
#define SCREEN_H 700      /* hauteur de l'ecran */

#define P 100                /* Taille d'un Bloc */

#define CLOCKWISE 			1
#define NO_SOLUTION 		0
#define COUNTER_CLOCKWISE 	-1

#define SEPARATEUR '*'
#define SEPARATOR '$'
#define NIL '/'
#define LINE_LENGTH 10000

#define BLANC  SDL_MapRGB (surface->format, 255, 255, 255)
#define BLANCT SDL_MapRGBA(surface->format, 255, 255, 255, 100)

#define DB_PATH "DB/"

#endif
