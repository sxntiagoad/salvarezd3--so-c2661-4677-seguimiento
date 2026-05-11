#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "jardin_ornamental_general.h"

static int random_siguiente(int max, int min) {
    double r = (double) rand();
    double u = (double) RAND_MAX;
    return (int)((r / u) * (max - min)) + min;
}

static void usage(char const *prog, DWORD code) {
    fprintf(stderr,
            "Uso: %s [-l <limite>] -m <maximo>\r\n"
            "     %s -m <maximo>\r\n"
            "     %s -h\r\n",
            prog, prog, prog);
    ExitProcess(code);
}

DWORD WINAPI visitante(PVOID arg) {
    PJardin_General jardin = (PJardin_General) arg;
    DWORD id = GetCurrentThreadId();

    for (int v = 0; v < MAX_VISITAS; v++) {
        fprintf(stdout, "Visitante %lu afuera\r\n", id);
        Sleep(random_siguiente(TIEMPO_AFUERA, 0));

        entrar_jardin(jardin);
        fprintf(stdout, "Visitante %lu dentro del jardin\r\n", id);

        Sleep(random_siguiente(TIEMPO_VISITA, 0));
        salir_jardin(jardin);
    }
    fprintf(stdout, "Visitante %lu termino su jornada\r\n", id);
    return 0;
}

DWORD WINAPI admin(PVOID arg) {
    PJardin_General jardin = (PJardin_General) arg;
    for (int v = 0; v < MAX_VISITAS; v++) {
        Sleep(random_siguiente(TIEMPO_AFUERA * 2, 0));
        fprintf(stdout, "Visitantes %d dentro del jardin\r\n",
                obtener_miembros_jardin(jardin));
    }
    fprintf(stdout, "Administrador termino su jornada\r\n");
    return 0;
}

int main(int argc, char *argv[]) {
    int limite = 0;
    int maximo = 0;
    int opt;

    while ((opt = getopt(argc, argv, "l:m:h")) != -1) {
        switch (opt) {
            case 'l': limite = atoi(optarg); break;
            case 'm': maximo = atoi(optarg); break;
            case 'h': usage(argv[0], EXIT_SUCCESS); break;
            default:  usage(argv[0], EXIT_FAILURE);
        }
    }

    if (maximo <= 0) {
        fprintf(stderr, "Error: -m <maximo> es obligatorio y > 0\r\n");
        usage(argv[0], EXIT_FAILURE);
    }
    if (limite < 0 || (limite > 0 && limite >= maximo)) {
        fprintf(stderr, "Error: se requiere 0 < limite < maximo\r\n");
        ExitProcess(EXIT_FAILURE);
    }

    PJardin_General jardin = obtener_jardin(limite);
    if (jardin == NULL) {
        fprintf(stderr, "Error: no se pudo crear el jardin\r\n");
        ExitProcess(EXIT_FAILURE);
    }

    fprintf(stdout, "Jardin %s (maximo=%d, limite=%d)\r\n",
            (limite > 0 ? "LIMITADO" : "SIN LIMITE"), maximo, limite);
    fprintf(stdout, "Visitantes antes de iniciar: %d\r\n",
            obtener_miembros_jardin(jardin));

    HANDLE *hilos     = malloc(sizeof(HANDLE) * (maximo + 1));
    DWORD  *hilos_id  = malloc(sizeof(DWORD)  * (maximo + 1));
    srand(time(NULL));

    for (int i = 0; i < maximo; i++) {
        hilos[i] = CreateThread(NULL, 0, visitante, (PVOID) jardin, 0, &hilos_id[i]);
        if (hilos[i] == NULL) {
            fprintf(stderr, "Error creando hilo %d: %lu\r\n", i, GetLastError());
            ExitProcess(EXIT_FAILURE);
        }
    }
    hilos[maximo] = CreateThread(NULL, 0, admin, (PVOID) jardin, 0, &hilos_id[maximo]);

    /* WaitForMultipleObjects soporta maximo MAXIMUM_WAIT_OBJECTS (64).
       Si maximo+1 > 64 esperamos por bloques. */
    int total = maximo + 1;
    int idx   = 0;
    while (idx < total) {
        int n = total - idx;
        if (n > MAXIMUM_WAIT_OBJECTS) n = MAXIMUM_WAIT_OBJECTS;
        WaitForMultipleObjects(n, &hilos[idx], TRUE, INFINITE);
        idx += n;
    }

    for (int i = 0; i < total; i++) CloseHandle(hilos[i]);

    fprintf(stdout, "Visitantes antes de cerrar: %d\r\n",
            obtener_miembros_jardin(jardin));

    cerrar_jardin(jardin);
    free(hilos);
    free(hilos_id);
    ExitProcess(EXIT_SUCCESS);
}
