#Compilador
CC = g++
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c++98 -g -pthread -L./lib ../../libs/json/libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so
#Nombre del programa principal
CEXE = prueba

all:$(CEXE) 

socket.o: ../../libs/TDA/socket/socket.h ../../libs/TDA/socket/socket.cpp
	$(CC) $(CFLAGS) -c ../../libs/TDA/socket/socket.cpp
	
thread.o: ../../libs/TDA/thread/thread.h ../../libs/TDA/thread/thread.cpp
	$(CC) $(CFLAGS) -c ../../libs/TDA/thread/thread.cpp
	
json:
	cd ../../libs/json/; make
	
$(CEXE): socket.o thread.o $(CEXE).cpp
	$(CC) $(CFLAGS) socket.o thread.o $(CEXE).cpp -o $(CEXE)

clean:
	-rm *.o

cleanAll: clean
	-rm $(CEXE)

run: all
	./prueba

test: $(CEXE)
	valgrind ./$(CEXE)
