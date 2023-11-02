#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>

// 공유 메모리를 위한 구조체
struct SharedData {
    long totalElapsedTime;
};

void performMatrixMultiplication(int processNumber, struct SharedData *sharedData) {
    struct timeval startTime, endTime;
    char startStr[64], endStr[64];

    gettimeofday(&startTime, NULL);

    int count = 0;
    int A[100][100], B[100][100];
    long result[100][100];

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            A[i][j] = 999999999;
            B[i][j] = 999999999;
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

    gettimeofday(&endTime, NULL);
    formatTime(&startTime, startStr);
    formatTime(&endTime, endStr);
    printf("PID: %d | Start Time: %s | End Time: %s | ", processNumber, startStr, endStr);
    long elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000;
    printf("Elapsed Time: %ld milliseconds\n", elapsedTime);

    // 공유 메모리를 통해 totalElapsedTime을 누적
    sharedData->totalElapsedTime += elapsedTime;
}

void formatTime(struct timeval *tv, char *output) {
    struct tm *tm_info;
    char buffer[26];

    tm_info = localtime(&tv->tv_sec);
    strftime(buffer, 26, "%H:%M:%S", tm_info);
    snprintf(output, 64, "%s.%06ld", buffer, tv->tv_usec);
}

int main() {
    // 공유 메모리 생성
    struct SharedData *sharedData = mmap(NULL, sizeof(struct SharedData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sharedData == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    sharedData->totalElapsedTime = 0; // 공유 메모리 초기화

    for (int processNumber = 1; processNumber <= 21; processNumber++) {
        struct timeval startTime, endTime;
        char startStr[64], endStr[64];
        
        // Real-Time Round-Robin 스케줄링 정책 설정
        struct sched_param sp;
        sp.sched_priority = 1; // 우선순위 설정
        if (sched_setscheduler(0, SCHED_RR, &sp) == -1) {
            perror("sched_setscheduler");
            return 1;
        }

        gettimeofday(&startTime, NULL);
        
        pid_t pid = fork();
        if (pid == 0) {
            // 자식 프로세스에서 작업 수행
            performMatrixMultiplication(processNumber, sharedData);
            gettimeofday(&endTime, NULL);
            formatTime(&startTime, startStr);
            formatTime(&endTime, endStr);
            exit(0);
        } else if (pid > 0) {
            // 부모 프로세스에서 자식 프로세스 정보 기록
            int status;
            wait(&status);
        } else {
            perror("Fork failed");
            return 1;
        }
    }

    double avgElapsedTime = (double)sharedData->totalElapsedTime / 21;

    FILE *file;
    char buffer[256]; // 파일 내용을 저장할 버퍼
    int timeQuantum;    // time quantum
    // 파일 열기
    file = fopen("/proc/sys/kernel/sched_rr_timeslice_ms", "r");
    if (file == NULL) {
        perror("파일 열기 실패");
        return 1;
    }

    // 파일 내용 읽기
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d", &timeQuantum);
    printf("Scheduling Policy: RT_RR |  Time Quantum: %d ms  |  Average Elapsed Time: %.2f milliseconds\n", timeQuantum, avgElapsedTime);


    // 공유 메모리 정리
    munmap(sharedData, sizeof(struct SharedData));
    // 파일 닫기
    fclose(file);

    return 0;
}
