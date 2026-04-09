#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>

char peticion[100] = "/tmp/tuberia_peticion";
char solicitud[100] = "/tmp/tuberia_solicitud";

void handler(int sig){
    syslog(LOG_INFO, "Servidor terminado");
    unlink(peticion);
    unlink(solicitud);
    closelog();
    exit(0);
}

void daemonizar(){
    if(fork() > 0) exit(0);
    setsid();
    chdir("/");
    close(0);
    close(1);
    close(2);
}

int main(int argc, char *argv[]){
    int opt;
    int crear = 0;
    int d = 0;

    while((opt = getopt(argc, argv, "dcp:s:")) != -1){
        if(opt == 'c') crear = 1;
        if(opt == 'd') d = 1;
        if(opt == 'p') strcpy(peticion, optarg);
        if(opt == 's') strcpy(solicitud, optarg);
    }

    openlog("servidor4", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Servidor iniciado");

    if(crear){
        mkfifo(peticion, 0666);
        mkfifo(solicitud, 0666);
    }

    signal(SIGQUIT, handler);

    if(d) daemonizar();

    int fd_pet = open(peticion, O_RDONLY);
    int fd_sol = open(solicitud, O_WRONLY);

    char buffer[100];

    while(1){
        int n = read(fd_pet, buffer, sizeof(buffer));
        if(n > 0){
            syslog(LOG_INFO, "Peticion recibida");
            write(fd_sol, buffer, n);
        }
    }

    return 0;
}
