#include <iostream>
#include "celdas_vacias.h"

//
CeldasVacias::CeldasVacias(){
  celdas_vacias = NULL;
  contador = 0;
}

//
CeldasVacias::~CeldasVacias(){
  if (celdas_vacias){
    for (int i = 0; i < columnas; i++){
      delete celdas_vacias[i];
    }
    delete[] celdas_vacias;
  }
}

//
void CeldasVacias::inicializar(int cantidad_columnas){
  columnas = cantidad_columnas;
  celdas_vacias = new Lista<reemplazo_t>*[columnas];
  for (int i = 0; i < columnas; i++)
    celdas_vacias[i] = new Lista<reemplazo_t>;
}

//
void CeldasVacias::agregar(coordenada_t &celda, int tipo, int color){
  reemplazo_t reemplazo;
  reemplazo.celda = celda;
  reemplazo.tipo = tipo;
  reemplazo.color = color;
  Lista<reemplazo_t> *lista = celdas_vacias[celda.x];
  if (lista->esta_vacia()){
    lista->insertar_ultimo(reemplazo);
  }else{
    Lista_iter<reemplazo_t> iterador;
    iterador.iterar_en_lista(lista);
    
    reemplazo_t actual = iterador.ver_actual();
    while (actual.celda.y < celda.y){
      iterador.avanzar();
      if (iterador.al_final()) break;
      actual = iterador.ver_actual();
    }
    iterador.insertar(reemplazo);
  }
  contador++;
}

//
bool CeldasVacias::esta_vacia(int columna){
  if (columna < 0 || columna >= columnas) return false;
  Lista<reemplazo_t> *lista = celdas_vacias[columna];
  return lista->esta_vacia();
}

//
reemplazo_t CeldasVacias::borrar_proxima(int columna)throw(ListaVacia, ColumnaInvalida){
  if (columna < 0 || columna >= columnas) throw ColumnaInvalida();
  Lista<reemplazo_t> *lista = celdas_vacias[columna];
  contador--;
  return lista->borrar_primero();
}

//
bool CeldasVacias::existentes(){
  return contador != 0;
}