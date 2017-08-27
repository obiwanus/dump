#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
  int child_pid = fork();
  if (child_pid < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  if (child_pid == 0) {
    // Child
    wait(NULL);
    printf("Hey I'm child\n");
  } else {
    // Parent
    printf("Hey I'm parent\n");
  }
  return 0;
}
