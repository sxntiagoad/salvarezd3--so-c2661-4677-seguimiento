#pragma once
#include <semaphore.h>

#define NOM_SEM_AGENTE  "agente"
#define NOM_SEM_CERILLA "cerilla"
#define NOM_SEM_PAPEL   "papel"
#define NOM_SEM_TABACO  "tabaco"

#define PRE_GLOBAL "/"

#define MAX_TIEMPO_PREPARANDO 4
#define MAX_TIEMPO_FUMANDO    5

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };
typedef enum SEM_ID SEM_ID;

#define NUM_SEMS (AGENTE + 1)

/* Estructura que reemplaza las variables globales del agente original. */
struct EstadoAgente {
    sem_t *sems[NUM_SEMS];
    char  *nom_real_sem[NUM_SEMS];
};
typedef struct EstadoAgente EstadoAgente;
typedef struct EstadoAgente *PEstadoAgente;

/* Estructura para los procesos hijos lanzados por el controlador. */
struct EstadoControl {
    int  num_procesos;
    int *procesos;          /* pids */
};
typedef struct EstadoControl EstadoControl;
typedef struct EstadoControl *PEstadoControl;

/* Utilidad */
char* obtener_nombre_semaforo(char const *prefijo, char const *nom_sem);

/* Agente */
PEstadoAgente crear_estado_agente(char const *nom_sems[]);
void          destruir_estado_agente(PEstadoAgente estado);

/* Control */
PEstadoControl crear_estado_control(int num_procesos);
void           destruir_estado_control(PEstadoControl estado);
