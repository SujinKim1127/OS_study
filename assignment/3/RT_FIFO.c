#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <sched.h>

void performMatrixMultiplication(int processNumber) {
    // 배열 곱셈을 수행하는 작업을 여기에 구현
    int count = 0;
    int A[100][100], B[100][100];
    long result[100][100];

    // 모든 요소를 1000으로 초기화
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
                    result[k][j] += A[k][i] * B[i][j];
                }
            }
        }
        count++;
    }
}

void formatTime(struct timeval *tv, char *output) {
    struct tm *tm_info;
    char buffer[26];

    tm_info = localtime(&tv->tv_sec);
    strftime(buffer, 26, "%H:%M:%S", tm_info);
    snprintf(output, 64, "%s.%06ld", buffer, tv->tv_usec);
}

int main() {
    struct timeval startTime, endTime;
    char startStr[64], endStr[64];
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset); // CPU 코어 번호를 여기에 지정 (0번 코어)


    struct sched_param schedParam;

    gettimeofday(&startTime, NULL);

    long totalElapsedTime = 0; // 전체 elapsed time의 합


    for (int processNumber = 1; processNumber <= 21; processNumber++) {
        pid_t pid = fork();
        if (pid == 0) {
            // 자식 프로세스에서 작업 수행
            schedParam.sched_priority = processNumber; // 우선순위 설정
            sched_setscheduler(0, SCHED_FIFO, &schedParam); // Real-Time FIFO 스케줄링으로 변경
            performMatrixMultiplication(processNumber);
            exit(0);
        } else if (pid > 0) {
            // 부모 프로세스에서 자식 프로세스 정보 기록
            gettimeofday(&endTime, NULL);
            formatTime(&startTime, startStr);
            formatTime(&endTime, endStr);

            printf("PID: %d | Start Time: %s | End Time: %s | ", processNumber, startStr, endStr);
            long elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000 +
                (endTime.tv_usec - startTime.tv_usec) / 1000;
            printf("Elapsed Time: %ld milliseconds\n", elapsedTime);

            int status;
            wait(&status);

            totalElapsedTime += elapsedTime;
        } else {
            perror("Fork failed");
            return 1;
        }
    }

    double avgElapsedTime = (double)totalElapsedTime / 21;
    printf("Scheduling Policy: RT_FIFO  |   Average Elapsed Time: %.2f microseconds\n", avgElapsedTime);


    return 0;
}
