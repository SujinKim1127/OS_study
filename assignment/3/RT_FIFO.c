#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_PROCESSES 21

// 행렬 초기화 함수
void initialize_matrices(int A[100][100], int B[100][100]) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            A[i][j] = 10000000;
            B[i][j] = 20000000;
        }
    }
}

// 메인 계산 함수
void multiply_matrices() {
    int count = 0;
    int A[100][100], B[100][100];
    long result[100][100] = {0};

    initialize_matrices(A, B);

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
    double exec_times[NUM_PROCESSES];
    int fd[NUM_PROCESSES][2]; // 파이프 파일 디스크립터

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset); // CPU 0에 바인딩

    for (int i = 0; i < NUM_PROCESSES; i++) {
        // 파이프 생성
        if (pipe(fd[i]) == -1) {
            perror("pipe failed");
            exit(1);
        }
        
        gettimeofday(&start, NULL); // 시작 시간 측정

        pids[i] = fork();

        if (pids[i] == 0) { // 자식 프로세스
            // 프로세스를 CPU 0에 바인딩
            if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) == -1) {
                perror("sched_setaffinity failed");
                exit(1);
            }

            struct sched_param param;
            param.sched_priority = sched_get_priority_max(SCHED_FIFO);

            // 스케줄링 정책을 실시간 FIFO로 설정
            if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
                perror("sched_setscheduler failed");
                exit(1);
            }

            // 메인 계산 함수 실행
            multiply_matrices();

            // 실행 종료 시간 측정
            gettimeofday(&end, NULL);

            char start_str[100];
            struct tm *start_tm;
            start_tm = localtime(&start.tv_sec);
            strftime(start_str, sizeof(start_str), "%H:%M:%S", start_tm);
            long start_millis = start.tv_usec / 1000; // 밀리초 단위 변환

            // 종료 시간 포매팅
            char end_str[100];
            struct tm *end_tm;
            end_tm = localtime(&end.tv_sec);
            strftime(end_str, sizeof(end_str), "%H:%M:%S", end_tm);
            long end_millis = end.tv_usec / 1000; // 밀리초 단위 변환

            // 실행 시간 계산
            double exec_time = (end.tv_sec - start.tv_sec) * 1000.0;
            exec_time += (end.tv_usec - start.tv_usec) / 1000.0;

            printf("PID: %d | Start Time: %s.%03ld | End Time: %s.%03ld | Elapsed Time: %.2f ms\n",
                getpid(), start_str, start_millis, end_str, end_millis, exec_time);

            // 파이프에 실행 시간 쓰기
            close(fd[i][0]); // 읽기 끝 닫기
            write(fd[i][1], &exec_time, sizeof(exec_time));
            close(fd[i][1]); // 쓰기 끝 닫기

            exit(0);
        } else if (pids[i] < 0) {
            perror("fork failed");
            exit(1);
        }
    }

    double total_exec_time = 0;

    // 모든 자식 프로세스가 종료되기를 기다리는 부모 프로세스
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int status;
        pid_t pid = waitpid(pids[i], &status, 0);
        double exec_time;

        if (WIFEXITED(status)) {
            // 파이프에서 실행 시간 읽기
            close(fd[i][1]); // 쓰기 끝 닫기
            read(fd[i][0], &exec_time, sizeof(exec_time));
            close(fd[i][0]); // 읽기 끝 닫기

            total_exec_time += exec_time; // 총 실행 시간 업데이트
        }
    }

    // 평균 실행 시간 계산
    double average_exec_time = total_exec_time / NUM_PROCESSES;
    printf("Scheduling Policy: RT_FIFO  |   Average Execution Time: %.2f ms\n", average_exec_time);

    return 0;
}
