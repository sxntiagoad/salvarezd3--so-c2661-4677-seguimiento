#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <archivo> <transformacion> <modulo>\n", argv[0]);
        exit(1);
    }

    int fd1[2], fd2[2];

    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) {
        dup2(fd1[1], STDOUT_FILENO);
        close(fd1[0]);
        close(fd1[1]);
        execlp("cat", "cat", argv[1], NULL);
        exit(1);
    }

    if (fork() == 0) {
        char *arg1 = strtok(argv[2], " ");
        char *arg2 = strtok(NULL, " ");

        dup2(fd1[0], STDIN_FILENO);
        dup2(fd2[1], STDOUT_FILENO);

        close(fd1[1]);
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);

        execlp("tr", "tr", arg1, arg2, NULL);
        exit(1);
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[1]);

    FILE *fp = fdopen(fd2[0], "r");
    char linea[256];
    int count = 1;
    int mod = atoi(argv[3]);

    while (fgets(linea, sizeof(linea), fp)) {
        if (count % mod != 0) {
            printf("%s", linea);
        }
        count++;
    }

    fclose(fp);

    return 0;
}
