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

u64 get_wall_clock() {
  u64 result = 0;
  struct timespec spec;

  clock_gettime(CLOCK_MONOTONIC, &spec);
  result = spec.tv_nsec;  // ns

  return result;
}

#define MAX_NUM_ITERATIONS 100

void proc_proc(int write_pipe, int read_pipe) {
  char buf[2];
  char *string = "petya\n";

  u64 cycle_start = get_wall_clock();
  u64 cycle_end = get_wall_clock();
  u64 cycle = 0;
  u64 off_cycle = 0;

  for (int i = 0; i < MAX_NUM_ITERATIONS; ++i) {
    write(write_pipe, string, 1);
    cycle_end = get_wall_clock();
    cycle += cycle_end - cycle_start;
    read(read_pipe, buf, 1);
    cycle_start = get_wall_clock();
    off_cycle += cycle_start - cycle_end;
  }

  u64 cycle_time_avg = cycle / MAX_NUM_ITERATIONS;
  u64 off_cycle_time_avg = off_cycle / MAX_NUM_ITERATIONS;
  u64 ctx_switch_cost = (off_cycle_time_avg - cycle_time_avg) / 2;

  // printf("Avg. cycle time: %lu\n", cycle_time_avg);
  // printf("Avg. off cycle time: %lu\n", off_cycle_time_avg);
  printf("Avg. ctx switch cost: %lu\n", ctx_switch_cost);
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
    proc_proc(parent_pipe[1], child_pipe[0]);
  } else {
    close(child_pipe[0]);  // close read end
    close(parent_pipe[1]);  // close write end
    proc_proc(child_pipe[1], parent_pipe[0]);
  }

  return 0;
}
