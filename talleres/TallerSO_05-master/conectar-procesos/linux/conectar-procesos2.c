#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {

  pid_t child1, child2;
  int status;
  int pipe1[2];

  pipe(pipe1);

  child1 = fork();

  if (child1 == 0) {
    dup2(pipe1[1], STDOUT_FILENO);
    close(pipe1[0]);
    close(pipe1[1]);
    execl("/bin/ls", "ls", "-l", NULL);
    exit(1);
  }

  child2 = fork();

  if (child2 == 0) {
    dup2(pipe1[0], STDIN_FILENO);
    close(pipe1[0]);
    close(pipe1[1]);
    execl("/usr/bin/wc", "wc", NULL);
    exit(1);
  }

  close(pipe1[0]);
  close(pipe1[1]);

  waitpid(child1, &status, 0);
  waitpid(child2, &status, 0);

  return 0;
}
