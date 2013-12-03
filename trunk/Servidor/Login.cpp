#include "Login.h"
#include "funciones_auxiliares.h"
#include "../libs/json/include/json/json.h"
#include <sstream>

#define OK '0'
#define ERROR '1'

#define PUERTO_MAX 65535
#define PUERTO_CLIENTES 8001
#define TAG_USUARIO "usuario"
#define TAG_CLAVE "clave"

using std::string;

int Login::proximo_puerto = PUERTO_CLIENTES;

Login::Login(Socket* cliente, usuario_t* usuario, Mutex* mutex_puertos) :
    nuevo_usuario(usuario), mutex(mutex_puertos), cliente_actual(cliente) {
}

Login::~Login(){
}

int Login::asignarPuerto(Socket& sockfd){
    bool asociado = false;
    mutex->bloquear();
    while(! asociado){
        sockfd.asignar_direccion(proximo_puerto);
        if (sockfd.reusar() == -1) return 1;
        asociado = (sockfd.asociar() != -1);

        proximo_puerto++;
        if (proximo_puerto == PUERTO_MAX)
            proximo_puerto = PUERTO_CLIENTES;
    }
    mutex->desbloquear();
    if (sockfd.escuchar() == -1) return 2;
    return 0;
}

void Login::enviarPuertos(){
    nuevo_usuario->sockets->enviar = new Socket();
    nuevo_usuario->sockets->recibir = new Socket();

    Json::Value mensaje;
    Json::StyledWriter escritor;
    mensaje["recibir"] = htonl((uint32_t) asignarPuerto(*(nuevo_usuario->sockets->enviar)));
    mensaje["enviar"] = htonl((uint32_t) asignarPuerto(*(nuevo_usuario->sockets->recibir)));
    std::string envio = escritor.write(mensaje);
    std::cout << "recibir: " << nuevo_usuario->sockets->enviar->ver_puerto() << ", enviar: " << nuevo_usuario->sockets->recibir->ver_puerto() << std::endl;

    cliente_actual->enviar(envio.c_str(), envio.length());
    cliente_actual->cerrar_enviar_recibir();
}

bool Login::aceptarSubConexiones(){
    nuevo_usuario->sockets->enviar_cli = new Socket();
    nuevo_usuario->sockets->recibir_cli = new Socket();
    Socket* sockfds[] = {nuevo_usuario->sockets->enviar, nuevo_usuario->sockets->recibir};
    Socket* socks_cli[] = {nuevo_usuario->sockets->enviar_cli, nuevo_usuario->sockets->recibir_cli};

    for(int i=0; i < 2 ; i++){
        if (sockfds[i]->aceptar(*(socks_cli[i])) == -1) {
            delete nuevo_usuario->sockets->enviar_cli;
            delete nuevo_usuario->sockets->recibir_cli;
            return false;
        }
    }
//    nuevo_usuario->sockets->enviar->cerrar_enviar_recibir();
//    nuevo_usuario->sockets->recibir->cerrar_enviar_recibir();
//    delete nuevo_usuario->sockets->enviar;
//    delete nuevo_usuario->sockets->recibir;
    return true;
}

int Login::recibirUsuarioContrasenia(){
    string aux;
    if (!recibirMsjPrefijo(*(nuevo_usuario->sockets->recibir_cli), aux))
        return CONEXION_ABORTADA;
    Json::Value datos;
    Json::Reader reader;
    std::istringstream ss(aux);

    reader.parse(ss, datos, false);
    nuevo_usuario->nombre = new string(datos[TAG_USUARIO].asString());
    nuevo_usuario->contrasenia = new string(datos[TAG_CLAVE].asString());

    return 0;
}

bool Login::verificarUsuario(ArchivoDirecto& arch_usuarios){
    string aux_contrasenia;
    if (! arch_usuarios.obtener(*(nuevo_usuario->nombre), aux_contrasenia)) {
        arch_usuarios.agregar(*(nuevo_usuario->nombre), *(nuevo_usuario->contrasenia));
        return true;
    }
    char res = (aux_contrasenia == *(nuevo_usuario->contrasenia)) ? OK : ERROR;
    nuevo_usuario->sockets->enviar_cli->enviar(&res, sizeof(char));

    if (res == ERROR) delete nuevo_usuario->nombre;
    delete nuevo_usuario->contrasenia;

    return (res == OK);
}

