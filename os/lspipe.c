#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {

  // Create a pipe
  int pfd[2];
  if (pipe(pfd) == -1) {
    printf("Couldn't create pipe\n");
    exit(1);
  }
  int pipe_read = pfd[0], pipe_write = pfd[1];

  int child_pid = fork();
  if (child_pid < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  if (child_pid == 0) {
    // Child
    close(pipe_read);

    // Redirect stdout to pipe
    dup2(pipe_write, STDOUT_FILENO);

    // Execute ls
    // char *arguments = {"ls", "-l", NULL};
    int result = execl("/bin/ls", "-l", (char *)NULL);
    if (result == -1) {
      printf("Couldn't execute ls\n");
    }
  } else {
    // Parent
    close(pipe_write);
    wait(NULL);
    char buf[1000];
    int bytes_read = read(pipe_read, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, bytes_read);
  }
  return 0;
}
