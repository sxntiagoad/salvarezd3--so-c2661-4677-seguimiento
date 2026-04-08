#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void esperar_senal(void);
void verifica_estado(int estado);
void manejador(int sig);

int
main(int argc, char* const argv[], char* const env[]) {

  pid_t pid_hijo = fork();

  if (pid_hijo == 0) {

    signal(SIGALRM, manejador);
    signal(SIGHUP, manejador);
    signal(SIGQUIT, manejador);

    esperar_senal();
    _exit(EXIT_SUCCESS);
  }

  sleep(1);

  kill(pid_hijo, SIGALRM);
  sleep(1);

  kill(pid_hijo, SIGHUP);
  sleep(1);

  kill(pid_hijo, SIGQUIT);

  int estado;
  if (waitpid(pid_hijo, &estado, 0) == pid_hijo) {
    verifica_estado(estado);
  }

  return EXIT_SUCCESS;
}

void esperar_senal(void) {
  while (1) {
    pause();
  }
}

void manejador(int sig) {
  printf("Señal recibida: %d\n", sig);

  switch (sig) {
    case SIGALRM:
      printf("→ SIGALRM recibida\n");
      break;
    case SIGHUP:
      printf("→ SIGHUP recibida\n");
      break;
    case SIGQUIT:
      printf("→ SIGQUIT recibida\n");
      break;
    default:
      printf("→ Otra señal\n");
  }
}

void verifica_estado(int estado) {
  if (WIFEXITED(estado)) {
    fprintf(stdout, "Proceso terminó con exit: %d\n",
            WEXITSTATUS(estado));
  }
  else if (WIFSIGNALED(estado)) {
    fprintf(stdout, "Proceso finalizado por señal: %d\n",
            WTERMSIG(estado));
  }
}
