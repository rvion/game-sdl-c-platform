gcc -c basic_maths.c 	-Wall -pedantic -ansi
gcc -c camera.c 		-Wall -pedantic -ansi
gcc -c geometry.c 		-Wall -pedantic -ansi

gcc -c ini.c 			-Wall -pedantic -ansi
gcc -c ini_load.c 		-Wall -pedantic -ansi

gcc -c game_affichage.c -Wall -pedantic -ansi
gcc -c physics.c 		-Wall -pedantic -ansi
gcc -c polygons.c 		-Wall -pedantic -ansi
gcc -c sdl_affichage.c 	-Wall -pedantic -ansi
gcc -c main.c 			-Wall -pedantic -ansi

gcc -o jeuexecutable_windows basic_maths.o camera.o geometry.o ini.o ini_load.o physics.o polygons.o sdl_affichage.o game_affichage.o main.o -lmingw32 -lSDLmain -lSDL -lm
 

rm *.o

jeuexecutable_windows
