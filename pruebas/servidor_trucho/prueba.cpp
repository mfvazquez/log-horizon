#include <iostream>
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/thread/thread.h"
#include "../../libs/TDA/socket/socket_prefijo.h"
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "../../libs/json/include/json/json.h"

#define CREAR 0
#define UNIRSE 1
#define SALIRSE -1
#define RETROCEDER 5
#define OK 6
#define ERROR 7


class EnviarNivel : public Thread{
  public:
    void asignar_sockets(SocketPrefijo &emisor_){
      emisor = emisor_;
    }
    
    void enviar_mensaje(char mensaje_){
      mutex.bloquear();
      mensaje = mensaje_;
      std::cout << "mensaje ahora es " << (int) mensaje << std::endl;
      mutex.desbloquear();
    }
    
  void esta_creada(bool creada_){
    mutex.bloquear();
    creada = creada_;
    mutex.desbloquear();
  }
  protected:
    void funcion_a_correr(){
      
      Json::Value envio;
      if (creada) envio["tipo"] = UNIRSE;
      else envio["tipo"] = CREAR;
      
      envio["nombre"] = "Partida Loca";
      envio["puntaje"] = 2123;
      envio["jugadores"] = 0;
      envio["max jugadores"] = 5;
      envio["credor"] = "Mingo";
      
      Json::StyledWriter escritor;
      std::string a_enviar = escritor.write(envio);
      
      std::cout << "a_enviar = " << a_enviar << std::endl;
      
      int contador = 0;
      while (true){
        
        mutex.bloquear();
        if (mensaje == ERROR) return;
        mutex.desbloquear();
        emisor.enviar_con_prefijo(a_enviar.c_str(), a_enviar.size());
        sleep(5);
        contador++;
        envio["jugadores"] = contador;
        a_enviar = escritor.write(envio);
      }
    }      
  
  private:
    SocketPrefijo emisor;
    Mutex mutex;
    bool creada;
    char mensaje;
};

int main(void){
  SocketPrefijo asignador;
  asignador.asignar_direccion(8000, "127.0.0.1");
  asignador.reusar();
  asignador.asociar();
  asignador.escuchar();
  SocketPrefijo asignador_emisor;
  asignador.aceptar(asignador_emisor);
  
  SocketPrefijo receptor_aux, emisor_aux;
  receptor_aux.asignar_direccion(8010, "127.0.0.1");
  receptor_aux.reusar();
  receptor_aux.asociar();
  receptor_aux.escuchar();
  emisor_aux.asignar_direccion(8011, "127.0.0.1");
  emisor_aux.reusar();
  emisor_aux.asociar();
  emisor_aux.escuchar();
  
  Json::Value valor;
  Json::StyledWriter escritor; 
  
  int puerto_emisor = 8011;
  puerto_emisor = htonl(puerto_emisor);
  int puerto_receptor = 8010;
  puerto_receptor = htonl(puerto_receptor);
  
  valor["recibir"] = puerto_emisor;
  valor["enviar"] = puerto_receptor;
  
  std::string puertos = escritor.write(valor);
  
  std::cout << "se envia = " << puertos << std::endl;
  
  asignador_emisor.enviar_con_prefijo(puertos.c_str(), puertos.size());
  
  SocketPrefijo receptor, emisor;
  emisor_aux.aceptar(emisor);
  receptor_aux.aceptar(receptor);
  
  char respuesta = 1; 
  std::string usuario_valido = "Deuteronomio";
  std::string clave_valida = "123qwe";
  while (respuesta == 1){
    
    uint32_t largo;
    receptor.recibir_largo(largo);
    char *mensaje = new char[largo + 1];
    mensaje[largo] = '\0';
    receptor.recibir(mensaje, largo);
    std::string temp = std::string(mensaje);
    std::istringstream ss(temp);
    Json::Reader reader;
    Json::Value aux;
    reader.parse(ss, aux, false);
    
    std::cout << aux << std::endl;
    
    std::string usuario = aux["usuario"].asString();
    std::string clave  = aux["clave"].asString();
    
    if (usuario == usuario_valido && clave == clave_valida)
      respuesta = 0;
    
    delete[] mensaje;

    emisor.enviar(&respuesta, sizeof(char));
  }
  
  EnviarNivel envio;
  envio.asignar_sockets(emisor);
  
  char eleccion;
  receptor.recibir(&eleccion, sizeof(char));
  if (eleccion == CREAR){
    envio.esta_creada(false);
  }else if(eleccion == UNIRSE){
    envio.esta_creada(true);
  }else if(eleccion == SALIRSE){
    char c = ERROR;
    emisor.enviar(&c,sizeof(char) );
    return 0;
  }
  envio.correr();
  
  bool lala = true;
  while (lala){
    int caca = receptor.recibir(&eleccion, sizeof(char));
    if (caca == 0){
      lala = false;
      envio.enviar_mensaje(ERROR);
      continue;
    }
    std::cout << "caca 2 = " << caca << std::endl;
    std::cout << "se recibio " << (int) eleccion << std::endl;
    if (eleccion == CREAR){
      envio.esta_creada(false);
    }else if(eleccion == UNIRSE){
      envio.esta_creada(true);
    }else if(eleccion == SALIRSE){
      envio.enviar_mensaje(ERROR);
      lala = false;
    }
  }
  
  std::cout << "antes del join" << std::endl;
  emisor.cerrar_enviar_recibir();
  receptor.cerrar_enviar_recibir();
  emisor_aux.cerrar_enviar_recibir();
  receptor_aux.cerrar_enviar_recibir();
  std::cout << "cerradas todas las conexiones " << std::endl;
  envio.join();
  
  return 0;
}
