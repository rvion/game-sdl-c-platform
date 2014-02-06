CC = gcc

CFLAGS = -Wall -pedantic -O2

OBJS = basic_maths.o camera.o geometry.o ini.o ini_load.o physics.o polygons.o sdl_affichage.o game_affichage.o main.o

LIB =  -lSDL -lm

PROG = Libellule

all : $(OBJS) 
	$(CC) -o $(PROG) $(OBJS) $(LIB) 

main.o : main.c
	$(CC) $(CFLAGS)  -c main.c
	
basic_maths.o : basic_maths.c
	$(CC) $(CFLAGS)  -c basic_maths.c
	
camera.o : camera.c
	$(CC) $(CFLAGS)  -c camera.c
	
geometry.o : geometry.c
	$(CC) $(CFLAGS)  -c geometry.c
	
ini.o : ini.c
	$(CC) $(CFLAGS)  -c ini.c
	
ini_load.o : ini_load.c
	$(CC) $(CFLAGS)  -c ini_load.c
	
physics.o : physics.c
	$(CC) $(CFLAGS)  -c physics.c
	
polygons.o : polygons.c
	$(CC) $(CFLAGS)  -c polygons.c
	
sdl_affichage.o : sdl_affichage.c
	$(CC) $(CFLAGS)  -c sdl_affichage.c

game_affichage.o : game_affichage.c
	$(CC) $(CFLAGS)  -c game_affichage.c
	
	
clean :
	-rm *.o
