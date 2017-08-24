#define _GNU_SOURCE
#include <sched.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t u64;

u64 LinuxGetWallClock() {
  u64 result = 0;
  struct timespec spec;

  clock_gettime(CLOCK_MONOTONIC, &spec);
  result = spec.tv_nsec;  // ns

  return result;
}

#define MAX_NUM_ITERATIONS 1000

void parent_proc(int write_pipe, int read_pipe) {
  char buf[2];

  char *string = "vasia\n";

  for (int i = 0; i < MAX_NUM_ITERATIONS; ++i) {
    write(write_pipe, string, 6);
  }
}

void child_proc(int write_pipe, int read_pipe) {
  char buf[2];

  for (int i = 0; i < MAX_NUM_ITERATIONS; ++i) {
    while (read(read_pipe, buf, 1) > 0) {
      write(STDOUT_FILENO, buf, 1);
    }
  }
}

int main() {
  // Set the processes to run on one CPU
  cpu_set_t cpu_set;
  CPU_SET(1, &cpu_set);  // add processor 1 to the set

  // Create pipes
  int parent_pipe[2];
  int child_pipe[2];
  if (pipe(parent_pipe) == -1 || pipe(child_pipe) == -1) {
    printf("Pipe failed\n");
    exit(1);
  }

  pid_t child_pid = fork();
  if (child_pid == -1) {
    printf("Fork failed\n");
    exit(1);
  }

  bool is_parent = (child_pid != 0);
  // set both processes to be scheduled on the same CPU
  sched_setaffinity(getpid(), sizeof(cpu_set), &cpu_set);

  if (is_parent) {
    close(parent_pipe[0]);  // close read end
    close(child_pipe[1]);  // close write end
    parent_proc(parent_pipe[1], child_pipe[0]);
  } else {
    close(child_pipe[0]);  // close read end
    close(parent_pipe[1]);  // close write end
    child_proc(child_pipe[1], parent_pipe[0]);
  }

  return 0;
}
