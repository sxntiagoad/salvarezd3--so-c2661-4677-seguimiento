#pragma once
#include <windows.h>

#define MAX_VISITAS    10
#define TIEMPO_AFUERA  1000
#define TIEMPO_VISITA  5000

struct Jardin_General {
  HANDLE h_sem_mutex;
  HANDLE h_sem_limite;
  int    tiene_limite;
  int    contador;
};

typedef struct Jardin_General Jardin_General;
typedef struct Jardin_General *PJardin_General;

PJardin_General obtener_jardin(int limite);
void entrar_jardin(PJardin_General jardin);
void salir_jardin(PJardin_General jardin);
int  obtener_miembros_jardin(PJardin_General jardin);
void cerrar_jardin(PJardin_General jardin);
