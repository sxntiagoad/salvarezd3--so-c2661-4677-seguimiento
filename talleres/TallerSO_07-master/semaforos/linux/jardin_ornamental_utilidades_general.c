#include <stdlib.h>
#include <semaphore.h>
#include "jardin_ornamental_general.h"

static Jardin_General *pJardin = NULL;

PJardin_General obtener_jardin(int limite) {
    if (pJardin != NULL) return pJardin;

    pJardin = malloc(sizeof(Jardin_General));
    if (pJardin == NULL) return NULL;

    pJardin->contador     = 0;
    pJardin->tiene_limite = (limite > 0);

    if (sem_init(&pJardin->sem_mutex, 0, 1) == -1) {
        free(pJardin);
        pJardin = NULL;
        return NULL;
    }

    if (pJardin->tiene_limite) {
        if (sem_init(&pJardin->sem_limite, 0, limite) == -1) {
            sem_destroy(&pJardin->sem_mutex);
            free(pJardin);
            pJardin = NULL;
            return NULL;
        }
    }

    return pJardin;
}

void entrar_jardin(PJardin_General jardin) {
    if (jardin == NULL) return;
    if (jardin->tiene_limite) sem_wait(&jardin->sem_limite);
    sem_wait(&jardin->sem_mutex);
    jardin->contador++;
    sem_post(&jardin->sem_mutex);
}

void salir_jardin(PJardin_General jardin) {
    if (jardin == NULL) return;
    sem_wait(&jardin->sem_mutex);
    jardin->contador--;
    sem_post(&jardin->sem_mutex);
    if (jardin->tiene_limite) sem_post(&jardin->sem_limite);
}

int obtener_miembros_jardin(PJardin_General jardin) {
    int v = -1;
    if (jardin == NULL) return v;
    sem_wait(&jardin->sem_mutex);
    v = jardin->contador;
    sem_post(&jardin->sem_mutex);
    return v;
}

void cerrar_jardin(PJardin_General jardin) {
    if (jardin == NULL) return;
    sem_destroy(&jardin->sem_mutex);
    if (jardin->tiene_limite) sem_destroy(&jardin->sem_limite);
    free(jardin);
    pJardin = NULL;
}
