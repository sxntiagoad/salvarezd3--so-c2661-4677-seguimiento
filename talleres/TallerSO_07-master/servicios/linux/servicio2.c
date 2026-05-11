#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>

#define LIMITE_SENALES 5

static volatile sig_atomic_t contador = 0;

static void manejador(int signo) {
    contador++;

    const char *nombre;
    switch (signo) {
        case SIGTERM: nombre = "SIGTERM"; break;
        case SIGINT:  nombre = "SIGINT";  break;
        case SIGTSTP: nombre = "SIGTSTP"; break;
        case SIGHUP:  nombre = "SIGHUP";  break;
        default:      nombre = "DESCONOCIDA"; break;
    }

    syslog(LOG_INFO, "Senal %s recibida (%d/%d)", nombre, (int)contador, LIMITE_SENALES);

    if (contador >= LIMITE_SENALES) {
        syslog(LOG_INFO, "Limite de senales alcanzado: terminando servicio2");
        closelog();
        exit(0);
    }
}

int main(void) {
    openlog("servicio2", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Servicio2 iniciado (PID=%d)", getpid());

    struct sigaction sa;
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    /* SIGSTOP no puede capturarse: usamos SIGTSTP (Ctrl-Z) como equivalente.
       Tambien escuchamos SIGHUP que envia systemctl reload. */
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGHUP,  &sa, NULL);

    while (1) {
        pause();   /* duerme hasta que llegue una senal */
    }

    return 0;
}
