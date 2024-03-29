#ifndef NIVEL_H
#define NIVEL_H

#include <arpa/inet.h>

#include "matriz.h"
#include "explosion.h"
#include "celdas_vacias.h"
#include "productos.h"
#include "receptor_resultados.h"
#include "puntaje.h"

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/lista/lista.h"
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/socket/socket_prefijo.h"


class Nivel{
  public:
    // Constructor de clase
    Nivel();
    
    // Destructor de clase
    virtual ~Nivel();
    
    // Metodo principal de la clase, inicializa todos los datos necesarios
    // para la ejecucion del nivel, y lo pone en marcha.
    bool correr(Ventana* ventana);
  
    // Inicializa los datos necesarios para la ejecucion del nivel
    void inicializar(const std::string &path, Ventana* ventana, SocketPrefijo* enviar, SocketPrefijo* recibir);
    
  private:
    Productos *productos;
    Matriz *tablero;
    Explosion *explosion;
    CeldasVacias *celdas_vacias;
    Textura *fondo;
    Textura *seleccion;
    int ancho_celda;
    int alto_celda;
    ReceptorResultados *receptor;
    SocketPrefijo *socket_enviar;
    SocketPrefijo *socket_recibir;
    Lista<reemplazo_t> *celdas_a_explotar;
    Puntaje *puntaje;
    Mensaje *mensaje;
    bool finalizado;
    
    Mix_Chunk *sonido_explosion;
    Mix_Chunk *sonido_movimiento;
    Mix_Chunk *sonido_seleccion;
    Mix_Chunk *sonido_apilar;
    
  protected:
    // Dibuja en la ventana todos los elementos del nivel
    void dibujar(Ventana *ventana);
  
    // Analiza el evento ingresado por parametro, realizando diferentes
    // acciones en base al tipo de evento.
    // Si el evento es un click izquierdo, seleccionara la celda en
    // la ubicacion del mouse, en caso de haber una celda adyacente seleccionada,
    // intercambiara contenidos con dicha celda.
    bool analizar_evento(SDL_Event &evento);
    
    // Actualiza todas las animaciones utilizadas por el nivel.
    void actualizar_animaciones();
    
    // Intercambia el contenido de las celdas cuyas coordenadas
    // son recibidas por parametro.
    void intercambiar(coordenada_t &origen, coordenada_t &destino);
    
    // Apila en la columna de la celda, una animacion del tipo y color
    // ingresado, hasta la fila indicada por la celda.
    void apilar(int tipo, int color, coordenada_t &celda);
    
    // Realiza una explosion en la coordenada ingresada, y almacena
    // el tipo y color del nuevo producto a ser apilado en la columna,
    void explotar(coordenada_t &celda, int tipo, int color);
    
    // Actualiza el nivel con los datos recibidos desde el servidor, 
    // en caso de no recibirse ningun dato, el metodo no realiza accion
    // alguna.
    void actualizar_receptor();
    
    // Envia el movimiento realizado al servidor.
    void enviar_movimiento(coordenada_t &celda, coordenada_t &celda_adyacente);
    
    // Inserta un producto del tipo y color en la celda de coordenada
    // ingresada por parametro
    void insertar(coordenada_t &celda, int tipo, int color);
    
    //
    void parsear_movimiento(uint32_t &mensaje);

    //
    void parsear_explosion(uint32_t &mensaje);

    //
    void parsear_insercion(uint32_t &mensaje);

    //
    void parsear_puntaje(uint32_t &mensaje);
};

#endif // NIVEL_H
