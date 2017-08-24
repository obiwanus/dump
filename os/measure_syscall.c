#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef uint64_t u64;

u64 LinuxGetWallClock() {
  u64 result = 0;
  struct timespec spec;

  clock_gettime(CLOCK_MONOTONIC, &spec);
  result = spec.tv_nsec;  // ns

  return result;
}

int main() {
  const int kNumSysCalls = 100000;

  u64 timestamp = LinuxGetWallClock();
  for (int i = 0; i < kNumSysCalls; ++i) {
    read(0, NULL, 0);
  }
  u64 ns_per_read = (LinuxGetWallClock() - timestamp) / kNumSysCalls;

  printf("Nanoseconds per read() call: %lu\n", ns_per_read);

  return 0;
}
