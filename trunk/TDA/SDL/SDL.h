#ifndef SDL_H
#define SDL_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "ventana.h"
#include "superficie.h"
#include "animacion.h"

class SDL {
  public:
    SDL(Uint32 flags = 0){
      SDL_Init(flags);
    }
    
    void habilitar_sonido(int frecuencia, Uint16 formato, int canales, int chunksize){
      Mix_OpenAudio(frecuencia, formato, canales, chunksize);
    }
    
  virtual ~SDL(){
    SDL_Quit();
    Mix_CloseAudio();
  }
};

#endif // SDL_H