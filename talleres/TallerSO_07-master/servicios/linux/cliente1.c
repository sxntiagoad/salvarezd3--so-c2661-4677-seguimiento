#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_ENTRADA "/tmp/eco_entrada"
#define FIFO_SALIDA  "/tmp/eco_salida"

int main(void) {
    char mensaje[1024];
    char respuesta[1024];

    while (1) {
        printf("Mensaje: ");
        fflush(stdout);

        if (fgets(mensaje, sizeof(mensaje), stdin) == NULL) break;
        if (mensaje[0] == '\n') continue;

        int fd_entrada = open(FIFO_ENTRADA, O_WRONLY);
        if (fd_entrada == -1) { perror("open entrada"); return 1; }
        write(fd_entrada, mensaje, strlen(mensaje));
        close(fd_entrada);

        int fd_salida = open(FIFO_SALIDA, O_RDONLY);
        if (fd_salida == -1) { perror("open salida"); return 1; }
        int n = read(fd_salida, respuesta, sizeof(respuesta) - 1);
        close(fd_salida);

        if (n > 0) {
            respuesta[n] = '\0';
            printf("Respuesta: %s\n", respuesta);
        }
    }

    return 0;
}
