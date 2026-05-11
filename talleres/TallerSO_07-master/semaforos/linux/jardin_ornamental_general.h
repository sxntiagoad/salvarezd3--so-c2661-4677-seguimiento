#pragma once
#include <semaphore.h>

#define MAX_VISITAS    10
#define TIEMPO_AFUERA  1
#define TIEMPO_VISITA  5

struct Jardin_General {
  sem_t sem_mutex;
  sem_t sem_limite;
  int   tiene_limite;
  int   contador;
};

typedef struct Jardin_General Jardin_General;
typedef struct Jardin_General *PJardin_General;

PJardin_General obtener_jardin(int limite);
void entrar_jardin(PJardin_General jardin);
void salir_jardin(PJardin_General jardin);
int  obtener_miembros_jardin(PJardin_General jardin);
void cerrar_jardin(PJardin_General jardin);
