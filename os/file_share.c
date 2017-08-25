#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main() {

  // Create a file
  int fd = open("./tmpfile.txt", O_WRONLY | O_CREAT | O_TRUNC);
  char buffer[100] = "vasia petya";

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  if (rc == 0) {
    // Child
    buffer[0] = 'p';
    write(fd, buffer, 11);
  } else {
    // Parent
    write(fd, buffer, 11);
  }
  return 0;
}
