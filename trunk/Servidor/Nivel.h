#ifndef NIVEL_H
#define NIVEL_H
#include "Lista.h"
#include "Tablero.h"
#include "Hash.h"
#include "Jugador.h"
#include <string>
#include "DatosJugador.h"


class Nivel{
    public:
        Nivel(Dimension& tam, char** estructura, int max_puntos);
        virtual ~Nivel();
        void agregarJugador(std::string& nombre);
        void imprimirTablero();
        void jugar();
    protected:

    private:
        int puntaje_objetivo;
        int puntaje_max;
        Tablero* tablero;
        DatosJugador* datos_jug;
};

#endif // NIVEL_H