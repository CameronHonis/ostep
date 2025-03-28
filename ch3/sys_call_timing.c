#define _GNU_SOURCE
#include <assert.h>
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <x86intrin.h>

#define MILLION 1000000
#define BILLION 1000 * MILLION

struct timeval dt(struct timeval *a, struct timeval *b) {
    struct timeval result;
    if (b->tv_usec < a->tv_usec) {
        result.tv_sec = b->tv_sec - a->tv_sec - 1;
        result.tv_usec = BILLION + b->tv_usec - a->tv_usec;
    } else {
        result.tv_sec = b->tv_sec - a->tv_sec;
        result.tv_usec = b->tv_usec - a->tv_usec;
    }
    return result;
}

char *timeval_to_str(struct timeval *tv) {
    char *buf;
    asprintf(&buf, "%ld.%06ld", tv->tv_sec, tv->tv_usec);
    return buf;
}

void print_dt(struct timeval *a, struct timeval *b) {
    struct timeval dt_ab = dt(a, b);
    printf("dt = %ld.%06ld\n", dt_ab.tv_sec, dt_ab.tv_usec);
}

struct Profiler {
    struct timeval start_time;
    struct timeval end_time;
    unsigned long long start_cycles;
};

struct Profiler *start_profiling() {
    struct Profiler *prof = malloc(sizeof(struct Profiler));
    gettimeofday(&prof->start_time, NULL);
    prof->start_cycles = __rdtsc();
    return prof;
}

void end_profiling(struct Profiler *prof) {
    gettimeofday(&prof->end_time, NULL);
    unsigned long long end_cycles = __rdtsc();
    unsigned long long total_cycles = end_cycles - prof->start_cycles;
    print_dt(&prof->start_time, &prof->end_time);
    printf("cycles: %llu\n", total_cycles);
}

void bind_to_core(int core_id) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core_id, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}

struct PipeSet {
    bool is_parent;
    int read_fd;
    int write_fd;
};

struct PipeSet setup_pipes() {
    // fork and pipe
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(1);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid > 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        return (struct PipeSet){true, pipe1[0], pipe2[1]};
    } else {
        close(pipe1[0]);
        close(pipe2[1]);
        return (struct PipeSet){false, pipe1[1], pipe2[0]};
    }
}

int main() {
    bind_to_core(0);

    printf("Reading 1 billion bytes from a file...\n");
    struct Profiler *prof = start_profiling();

    FILE *fp = fopen("output.txt", "r");
    char *buf[0];
    for (int i = 0; i < BILLION; i++) {
        size_t br = fread(buf, 0, 0, fp);
    }

    end_profiling(prof);

    printf("\nContext switching 1 billion times...\n");
    struct PipeSet pipes = setup_pipes();
    char *pipe_buf[1];
    prof = start_profiling();
    for (int i = 0; i < BILLION; i++) {
        if (!pipes.is_parent) {
            write(pipes.write_fd, "a", 1);
        }
        read(pipes.read_fd, pipe_buf, 1);
        write(pipes.write_fd, "b", 1);
    }
    end_profiling(prof);
}
