#include <windows.h>
#include <stdlib.h>
#include "jardin_ornamental_general.h"

static Jardin_General *pJardin = NULL;

PJardin_General obtener_jardin(int limite) {
    if (pJardin != NULL) return pJardin;

    pJardin = malloc(sizeof(Jardin_General));
    if (pJardin == NULL) return NULL;

    pJardin->contador     = 0;
    pJardin->tiene_limite = (limite > 0);
    pJardin->h_sem_limite = NULL;

    pJardin->h_sem_mutex = CreateSemaphore(NULL, 1, 1, NULL);
    if (pJardin->h_sem_mutex == NULL) {
        free(pJardin); pJardin = NULL; return NULL;
    }

    if (pJardin->tiene_limite) {
        pJardin->h_sem_limite = CreateSemaphore(NULL, limite, limite, NULL);
        if (pJardin->h_sem_limite == NULL) {
            CloseHandle(pJardin->h_sem_mutex);
            free(pJardin); pJardin = NULL; return NULL;
        }
    }
    return pJardin;
}

void entrar_jardin(PJardin_General jardin) {
    if (jardin == NULL) return;
    if (jardin->tiene_limite) WaitForSingleObject(jardin->h_sem_limite, INFINITE);
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador++;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
}

void salir_jardin(PJardin_General jardin) {
    if (jardin == NULL) return;
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador--;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
    if (jardin->tiene_limite) ReleaseSemaphore(jardin->h_sem_limite, 1, NULL);
}

int obtener_miembros_jardin(PJardin_General jardin) {
    int v = -1;
    if (jardin == NULL) return v;
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    v = jardin->contador;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
    return v;
}

void cerrar_jardin(PJardin_General jardin) {
    if (jardin == NULL) return;
    CloseHandle(jardin->h_sem_mutex);
    if (jardin->tiene_limite) CloseHandle(jardin->h_sem_limite);
    free(jardin);
    pJardin = NULL;
}
