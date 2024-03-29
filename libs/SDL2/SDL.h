#ifndef SDL_H
#define SDL_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "ventana/ventana.h"
#include "superficie/superficie.h"
#include "animacion/animacion.h"
#include "fps/fps.h"
#include "texto/texto.h"
#include "text_box/text_box.h"
#include "boton/boton.h"
#include "boton/checkbox.h"
#include "boton/checkbox_disjuntos.h"
#include "mensaje/mensaje.h"

class SDL {
  public:
    // Permite el uso de las librerias que utilizan SDL2
    SDL(Uint32 flags = 0){
      SDL_Init(flags);
    }
    
    // Permite el uso de True Type Fotns
    // devuelve false en caso de error
    bool habilitar_texto(){
      return TTF_Init() >= 0;
    }
    
    // Permite el uso de efectos sonoros
    void habilitar_sonido(int frecuencia, Uint16 formato, int canales, int chunksize){
      Mix_OpenAudio(frecuencia, formato, canales, chunksize);
    }
    
  virtual ~SDL(){
    SDL_Quit();
    Mix_CloseAudio();
    TTF_Quit();
  }
};

#endif // SDL_H
