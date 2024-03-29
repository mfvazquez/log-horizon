#include <iostream>
#include <string>

#include "login.h"
#include "../../libs/TDA/socket/socket_prefijo.h"

#define MAX_CARACTERES 20
#define FONDO_TEXTO 175
#define VOLUMEN 30
#define ALPHA_MENSAJE 220
#define DELAY 16

//
Login::Login(){
  fondo = new Textura;
  iniciar_sesion = new Boton;
  usuario = new TextBox;
  clave = new TextBox;
  escritor = new Texto;
  textura_clave = new Textura;
  textura_usuario = new Textura;
  sonido = NULL;
  mensaje = new Mensaje;
  conectando = false;
  datos_inicializados = false;
  autentificador = new Autentificador;
}

//
Login::~Login(){
  delete fondo;
  delete iniciar_sesion;
  delete usuario;
  delete clave;
  delete escritor;
  delete textura_clave;
  delete textura_usuario;
  delete mensaje;
  delete autentificador;
  if (sonido) Mix_FreeChunk(sonido);
}

//
int Login::inicializar(const std::string &path, Ventana *ventana, 
                       SocketPrefijo* enviar, SocketPrefijo* recibir){
  autentificador->asignar_sockets(enviar, recibir);
  // FONDO
  fondo->cargar_textura(path + "imagenes/fondo_login.png", ventana);
  
  unsigned int ancho = ventana->ver_ancho();
  unsigned int alto = ventana->ver_alto();
  
  // usado como punto de referencia para los demas destinos
  SDL_Rect destino;
  destino.x = ancho / 2 - ancho / 8;
  destino.w = ancho * 2 / 8;
  destino.h = alto / 20;
  destino.y = alto / 2 - destino.h * 2;
  
  
  // USUARIO
  std::string fuente_mono = path + "fuentes/mono.ttf";
  std::string fondo_texto = path + "imagenes/fondo_texto.png";
  usuario->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  usuario->asignar_fondo(fondo_texto, ventana);
  usuario->alpha_fondo(FONDO_TEXTO);
  SDL_Rect destino_usuario = destino;
  usuario->asignar_destino(destino_usuario);
  usuario->activar();
  usuario->asignar_color(255,255,255,255);   
  
  destino_text_usuario.y = destino.y;
  destino_text_usuario.x = destino.x - destino.w / 2 - 5;
  destino_text_usuario.h = destino.h;
  destino_text_usuario.w = destino.w / 2;
  
  // CLAVE
  clave->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  clave->asignar_fondo(fondo_texto, ventana);
  clave->alpha_fondo(FONDO_TEXTO);
  SDL_Rect destino_clave = destino;
  destino_clave.y = destino.y + destino.h * 2;
  clave->asignar_destino(destino_clave);
  clave->asignar_color(255,255,255,255);
  clave->ocultar();

  destino_text_clave.y = destino_clave.y;
  destino_text_clave.x = destino.x - destino.w / 2 - 5;
  destino_text_clave.h = destino.h;
  destino_text_clave.w = destino.w / 2;

  // TEXTO
  Superficie sup;
  
  escritor->asignar_fuente(path + "fuentes/orange.ttf", 30);
  escritor->asignar_color(0,0,0,255);
  
  escritor->copiar_texto("Usuario:", &sup);
  textura_usuario->cargar_textura(&sup, ventana);
  escritor->copiar_texto("Contraseña:", &sup);
  textura_clave->cargar_textura(&sup, ventana);
  
  // BOTON  
  SDL_Rect normal, apretado, sobre;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
  SDL_Rect destino_boton = destino;
  destino_boton.y = destino.y + destino.h * 4;
  destino_boton.w = destino.w / 2;
  destino_boton.x = destino.x + destino.w / 4;
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  iniciar_sesion->asignar_texturas(path + "imagenes/boton.png", 
                                   estructura, ventana);
  
  escritor->asignar_color(220,220,0,255);
  escritor->copiar_texto("Iniciar Sesion", &sup);
  escritor->asignar_color(0,0,0,255);
  
  SDL_Rect destino_texto;
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  iniciar_sesion->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // SONIDO
  std::string direccion = path + "sonidos/login_theme.wav";
  sonido = Mix_LoadWAV(direccion.c_str());
  sonido->volume = VOLUMEN;
  
  // MENSAJE
  destino.w = ancho / 3;
  destino.h = alto / 4;
  destino.x = ancho / 2 - destino.w / 2;
  destino.y = alto / 2 - destino.h / 2;
  mensaje->asignar_fondo(path + "imagenes/sub_ventana.png", destino, ventana);
  mensaje->establecer_alpha_fondo(ALPHA_MENSAJE);
  mensaje->asignar_escritor(path + "fuentes/orange.ttf", 30);
  destino.w = destino.w - destino.w / 5;
  destino.h = destino.h / 4;
  destino.y = destino.y + destino.h * 2 - destino.h / 2;
  destino.x = destino.x + destino.w / 8;
  mensaje->asignar_mensaje("Enviando datos...", destino, ventana);
  
  datos_inicializados = true;
  return 0;
}

//
bool Login::correr(Ventana *ventana){
  if (!datos_inicializados) return false;
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = DELAY;
  Mix_PlayChannel(-1, sonido, -1);
  while (corriendo){
    // Eventos
    while (SDL_PollEvent(&evento)){
      corriendo = this->analizar_evento(evento);
    }
    // Dibujado
    ventana->limpiar();
    this->dibujar(ventana);
    
    // actualizamos los fps
    frames.obtener_delay(tiempo_actual, delay);
    
    // Presentar en ventana
    ventana->presentar(delay);
    
    if (conectando && !autentificador->en_curso()){
      if (autentificador->usuario_valido()){
        int *a = new int;
        autentificador->join((void**)&a);
        return true;
      }else{
        int *a = new int;
        autentificador->join((void**)&a);
        conectando = false;
      }
    }
  }
  return false;
}

//
int Login::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  usuario->dibujar(ventana);
  clave->dibujar(ventana);
  iniciar_sesion->dibujar(ventana);
  textura_usuario->dibujar(destino_text_usuario, ventana);
  textura_clave->dibujar(destino_text_clave, ventana);
  if (conectando) mensaje->dibujar(ventana);
  return 0;
}

//
bool Login::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT) return false;
  if (!autentificador->en_curso()){
    usuario->analizar_evento(evento);
    clave->analizar_evento(evento);
    iniciar_sesion->analizar_evento(evento);
    if (iniciar_sesion->activado() && 
        usuario->ver_contenido() != "" && 
        clave->ver_contenido() != ""){
      this->enviar_datos();
    }
  }
  return true;
}

//
void Login::enviar_datos(){
  conectando = true;
  autentificador->autentificar(usuario->ver_contenido(), 
                               clave->ver_contenido());
  autentificador->correr();
}
