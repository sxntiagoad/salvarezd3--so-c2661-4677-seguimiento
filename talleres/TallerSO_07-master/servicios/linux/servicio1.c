#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>

#define FIFO_ENTRADA "/tmp/eco_entrada"
#define FIFO_SALIDA  "/tmp/eco_salida"

static int fd_entrada = -1;
static int fd_salida  = -1;

static void finalizar(int sig) {
    if (fd_entrada != -1) close(fd_entrada);
    if (fd_salida  != -1) close(fd_salida);

    unlink(FIFO_ENTRADA);
    unlink(FIFO_SALIDA);

    syslog(LOG_INFO, "Servicio detenido");
    closelog();
    exit(0);
}

static void invertir_caso(char *s, int n) {
    for (int i = 0; i < n; i++) {
        if (islower((unsigned char)s[i]))      s[i] = toupper((unsigned char)s[i]);
        else if (isupper((unsigned char)s[i])) s[i] = tolower((unsigned char)s[i]);
    }
}

int main(void) {
    signal(SIGTERM, finalizar);
    signal(SIGINT,  finalizar);

    openlog("servicio1", LOG_PID | LOG_CONS, LOG_USER);

    umask(0);
    mkfifo(FIFO_ENTRADA, 0666);
    mkfifo(FIFO_SALIDA,  0666);
    chmod(FIFO_ENTRADA, 0666);
    chmod(FIFO_SALIDA,  0666);

    syslog(LOG_INFO, "Servicio iniciado");

    char buffer[1024];

    while (1) {
        fd_entrada = open(FIFO_ENTRADA, O_RDONLY);
        if (fd_entrada == -1) continue;

        int n = read(fd_entrada, buffer, sizeof(buffer) - 1);
        close(fd_entrada);
        fd_entrada = -1;

        if (n <= 0) continue;

        buffer[n] = '\0';
        /* quita el salto de línea final si viene */
        if (n > 0 && buffer[n-1] == '\n') { buffer[n-1] = '\0'; n--; }

        syslog(LOG_INFO, "Mensaje recibido: %s", buffer);

        invertir_caso(buffer, n);

        syslog(LOG_INFO, "Mensaje enviado: %s", buffer);

        fd_salida = open(FIFO_SALIDA, O_WRONLY);
        if (fd_salida == -1) continue;
        write(fd_salida, buffer, strlen(buffer));
        close(fd_salida);
        fd_salida = -1;
    }

    return 0;
}
