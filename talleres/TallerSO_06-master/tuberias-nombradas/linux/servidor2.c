#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>

char peticion[100] = "/tmp/tuberia_peticion";
char solicitud[100] = "/tmp/tuberia_solicitud";

void handler(int sig){
    unlink(peticion);
    unlink(solicitud);
    exit(0);
}

int main(int argc, char *argv[]){
    int opt;
    int crear = 0;

    while((opt = getopt(argc, argv, "cp:s:")) != -1){
        if(opt == 'c') crear = 1;
        if(opt == 'p') strcpy(peticion, optarg);
        if(opt == 's') strcpy(solicitud, optarg);
    }

    if(crear){
        mkfifo(peticion, 0666);
        mkfifo(solicitud, 0666);
    }

    signal(SIGQUIT, handler);

    int fd_pet = open(peticion, O_RDONLY);
    int fd_sol = open(solicitud, O_WRONLY);

    char buffer[100];
    while(1){
        int n = read(fd_pet, buffer, sizeof(buffer));
        if(n > 0){
            write(fd_sol, buffer, n);
        }
    }

    return 0;
}
