#include <stdio.h>

int main(void) {
#if defined(_WIN32) || defined(_WIN64)
    const char *so = "Windows";
#elif defined(__linux__)
    const char *so = "Linux";
#elif defined(__APPLE__)
    const char *so = "macOS";
#else
    const char *so = "Desconocido";
#endif

#ifdef DEBUG
    printf("[DEBUG] Compilado con simbolos de depuracion\n");
#endif

    printf("Sistema operativo detectado: %s\n", so);

#ifdef VERSION
    printf("Version: %s\n", VERSION);
#else
    printf("Version: (no definida)\n");
#endif

    return 0;
}
