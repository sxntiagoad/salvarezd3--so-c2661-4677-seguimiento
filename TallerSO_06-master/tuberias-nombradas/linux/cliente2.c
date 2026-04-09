#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char peticion[100] = "/tmp/tuberia_peticion";
char solicitud[100] = "/tmp/tuberia_solicitud";

int main(int argc, char *argv[]){
    int opt;

    while((opt = getopt(argc, argv, "p:s:")) != -1){
        if(opt == 'p') strcpy(peticion, optarg);
        if(opt == 's') strcpy(solicitud, optarg);
    }

    int fd_pet = open(peticion, O_WRONLY);
    int fd_sol = open(solicitud, O_RDONLY);

    char buffer[100];
    sprintf(buffer, "hola\n");

    write(fd_pet, buffer, strlen(buffer));

    int n = read(fd_sol, buffer, sizeof(buffer));
    write(1, buffer, n);

    return 0;
}
