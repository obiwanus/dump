#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  if (rc == 0) {
    // Child
    printf("hello\n");
  } else {
    // Parent
    usleep(100);
    printf("goodbye\n");
  }
  return 0;
}
