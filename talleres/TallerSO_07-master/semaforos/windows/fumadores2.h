#pragma once
#include <windows.h>

#define NOM_SEM_AGENTE  "agente"
#define NOM_SEM_CERILLA "cerilla"
#define NOM_SEM_PAPEL   "papel"
#define NOM_SEM_TABACO  "tabaco"

#define PRE_GLOBAL "Global\\"

#define MAX_TIEMPO_PREPARANDO 4000
#define MAX_TIEMPO_FUMANDO    5000

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };
typedef enum SEM_ID SEM_ID;

#define NUM_SEMS (AGENTE + 1)

struct EstadoAgente {
    HANDLE hSems[NUM_SEMS];
    char  *nom_real_sem[NUM_SEMS];
};
typedef struct EstadoAgente EstadoAgente;
typedef struct EstadoAgente *PEstadoAgente;

struct EstadoControl {
    int num_procesos;
    PROCESS_INFORMATION *pi;
};
typedef struct EstadoControl EstadoControl;
typedef struct EstadoControl *PEstadoControl;

char* obtener_nombre_semaforo(char const *prefijo, char const *nom_sem);

PEstadoAgente  crear_estado_agente(char const *nom_sems[]);
void           destruir_estado_agente(PEstadoAgente estado);

PEstadoControl crear_estado_control(int num_procesos);
void           destruir_estado_control(PEstadoControl estado);
