#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void abrir_fichero(const char *ruta) {
    int fd = open(ruta, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "Error abriendo %s: %s\n", ruta, strerror(errno));
        return;
    }

    printf("Fichero %s abierto correctamente\n", ruta);
    close(fd);
}

int main(void) {
    abrir_fichero("no-existe.txt");
    abrir_fichero("../data/existe.txt");
    return 0;
}
