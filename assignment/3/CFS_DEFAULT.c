#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_PROCESSES 21
#define TIME_BUFFER_SIZE 64

void matrix_multiply();

int main() {
    pid_t pids[NUM_PROCESSES];
    int i;
    struct timeval start, end;
    long mtime, seconds, useconds;    
    int status;
    long total_time = 0;
    // total elasped time을 구하기 위해 파이프 사용
    int pipefds[NUM_PROCESSES][2]; // 파이프 파일 디스크립터 배열

    // 부모 프로세스의 시작 시간을 기록합니다.
    gettimeofday(&start, NULL);

    for (i = 0; i < NUM_PROCESSES; i++) {
         if (pipe(pipefds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork failed");
            exit(1);
        }

        // 자식 프로세스일 때
        if (pids[i] == 0) {
            close(pipefds[i][0]); // 읽기용 파이프 닫기
            matrix_multiply(pipefds[i][1]); // 파이프 파일 디스크립터 전달
            exit(0);
        }else {
            close(pipefds[i][1]); // 쓰기용 파이프 닫기
        }
    }

    // 모든 자식 프로세스들이 끝날 때까지 대기합니다.
    for (i = 0; i < NUM_PROCESSES; i++) {
        long child_time;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            child_time = WEXITSTATUS(status);
            total_time += child_time;
        }
    }

        // 실행 시간 수집 로직
    for (i = 0; i < NUM_PROCESSES; i++) {
        long child_time;
        // 자식 프로세스가 전송한 실행 시간을 읽습니다.
        if (read(pipefds[i][0], &child_time, sizeof(child_time)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        total_time += child_time;
        close(pipefds[i][0]); // 읽기용 파이프 닫기
    }

    // 부모 프로세스의 종료 시간을 기록합니다.
    gettimeofday(&end, NULL);
    // 평균 실행 시간을 계산합니다.
    double avg_time = (double)total_time / NUM_PROCESSES;
    printf("Scheduling Policy: CFS_DEFAULT  |   Average Elapsed Time: %f ms\n", avg_time);

    return 0;
}

// 실행/종료 했을때의 컴퓨터 시간으로 변환하는 함수
void format_time(struct timeval *tv, char *buffer) {
    struct tm *tm_info;
    char time_string[TIME_BUFFER_SIZE - 8]; 
    time_t curr_time = tv->tv_sec;

    tm_info = localtime(&curr_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", tm_info);
    snprintf(buffer, TIME_BUFFER_SIZE, "%s.%06ld", time_string, tv->tv_usec);
}

void matrix_multiply(int write_fd) {
    int count = 0;
    int A[100][100], B[100][100];
    long result[100][100];
    struct timeval start_time, end_time;
    char start_str[TIME_BUFFER_SIZE];
    char end_str[TIME_BUFFER_SIZE];

    // 자식 프로세스의 시작 시간을 기록합니다.
    gettimeofday(&start_time, NULL);
    format_time(&start_time, start_str);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            A[i][j] = 10000000;
            B[i][j] = 20000000;
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

    // 자식 프로세스의 종료 시간을 기록합니다.
    gettimeofday(&end_time, NULL);
    format_time(&end_time, end_str);

    long elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000 + end_time.tv_usec - start_time.tv_usec;

    printf("PID: %d | Start Time: %s | End Time: %s  |  Elapsed Time: %ldms \n",
        getpid(),
        start_str,
        end_str,
        elapsed);
    
    if (write(write_fd, &elapsed, sizeof(elapsed)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}
