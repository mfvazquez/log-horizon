#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g `sdl2-config --libs` -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread
#Nombre del programa principal
CEXE = prueba


all: $(CEXE) 


ventana.o: ../../libs/SDL2/ventana/ventana.h ../../libs/SDL2/ventana/ventana.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/ventana/ventana.cpp

superficie.o: ../../libs/SDL2/superficie/superficie.h ../../libs/SDL2/superficie/superficie.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/superficie/superficie.cpp
	
boton.o: ../../libs/SDL2/boton/boton.h ../../libs/SDL2/boton/boton.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/boton/boton.cpp

texto.o: ../../libs/SDL2/texto/texto.h ../../libs/SDL2/texto/texto.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/texto/texto.cpp
	
checkbox.o: ../../libs/SDL2/boton/checkbox.h ../../libs/SDL2/boton/checkbox.cpp
	$(CC) $(CFLAGS) -c ../../libs/SDL2/boton/checkbox.cpp


$(CEXE): checkbox.o ventana.o superficie.o  texto.o boton.o $(CEXE).cpp
	$(CC) $(CFLAGS) checkbox.o ventana.o superficie.o  texto.o boton.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o


cleanAll: clean
	-rm $(CEXE)

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind ./$(CEXE)
