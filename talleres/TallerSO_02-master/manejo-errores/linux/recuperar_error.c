#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void
main(void) {
  int estado = open("../data/no-existe.txt", O_RDONLY);

  if (estado < 0) {
    fprintf(stderr, "Error abriendo fichero debido a %d\n", errno);
  }

  errno = 0;

  estado = open("../data/existe.txt", O_RDONLY);

  if (estado < 0) {
    fprintf(stderr, "Error abriendo fichero debido a %d\n", errno);
  }

  // Todo recurso debe ser cerrado
  close(estado);
  
  exit(0);
}
