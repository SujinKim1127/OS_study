#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROCESSES 21
#define CPU_CORE 0

// 실행/종료 했을때의 컴퓨터 시간으로 변환하는 함수
void format_timeval(struct timeval *tv, char *buf, size_t sz) {
    struct tm *tm;
    size_t len;
    long ms;

    tm = localtime(&tv->tv_sec);
    len = strftime(buf, sz, "%H:%M:%S", tm);
    ms = tv->tv_usec / 1000;
    snprintf(buf + len, sz - len, ".%06ld", tv->tv_usec);
}

void multiply_matrices() {
    int A[100][100], B[100][100];
    long result[100][100];
    int count = 0;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            A[i][j] = 10000000;
            B[i][j] = 20000000;
            result[i][j] = 0;
        }
    }

    while (count < 100) {
        for (int k = 0; k < 100; k++) {
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < 100; j++) {
                    result[i][j] += A[i][i] * B[i][j];
                }
            }
        }
        count++;
    }
}

int main() {
    pid_t pids[NUM_PROCESSES];
    struct timeval start, end;
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(CPU_CORE, &set);

    // total elapsed time을 구하기 위해 임시파일 생성
    char temp_filename[] = "/tmp/exec_time_XXXXXX";
    int temp_file_fd = mkstemp(temp_filename);
    if (temp_file_fd < 0) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < NUM_PROCESSES; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        if (pids[i] == 0) { // 자식 프로세스
            // CPU 친화도 설정
            if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
                perror("sched_setaffinity");
                exit(EXIT_FAILURE);
            }

            // 다른 nice 값 설정 및 현재 nice 값 가져오기
            int current_nice;            
            if (i < 7) {
                nice(-20);
                current_nice = -20;
            } else if (i < 14) {
                nice(0);
                current_nice = 0;
            } else {
                nice(19);
                current_nice = 19;
            }

            // 시작 시간 측정
            gettimeofday(&start, NULL);
            char start_str[50];
            format_timeval(&start, start_str, sizeof(start_str));

            // 곱셈 프로세스 실행
            multiply_matrices();

            // 종료 시간 측정
            gettimeofday(&end, NULL);
            char end_str[50];
            format_timeval(&end, end_str, sizeof(end_str));

            // elapsed time 계산
            long seconds = end.tv_sec - start.tv_sec;
            long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            
            printf("PID: %d | Nice: %d | Start Time: %s | End Time: %s | Elapsed Time: %ld.%06ld seconds\n",
                   getpid(), current_nice, start_str, end_str, seconds, micros);
            

            // 실행 시간 임시 파일에 기록
            dprintf(temp_file_fd, "%ld\n", micros);
            close(temp_file_fd);
            exit(EXIT_SUCCESS);

        }
    }

    // 모든 자식 프로세스가 종료될 때까지 대기하고 평균 실행 시간 계산
    int status;
    int status;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pids[i], &status, 0);
    }

    // 임시 파일에서 평균 실행 시간 계산    
    FILE *temp_file = fopen(temp_filename, "r");
    if (!temp_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    long micros, total_micros = 0, count = 0;
    while (fscanf(temp_file, "%ld", &micros) > 0) {
        total_micros += micros;
        count++;
    }
    fclose(temp_file);

    // 임시 파일 삭제
    unlink(temp_filename);

    if (count > 0) {
        long average_micros = total_micros / count;
        printf("Scheduling Policy: CFS_NICE  |   Average Execution Time: %ld.%06ld seconds\n", average_micros / 1000000, average_micros % 1000000);
    }

    return 0;
}
