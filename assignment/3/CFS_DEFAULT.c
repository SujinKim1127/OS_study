#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>

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

    while(count < 100){
        for(int k = 0; k < 100; k++){
            for(int i = 0; i < 100; i++){ 
                for(int j = 0; j < 100; j++){
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

    gettimeofday(&startTime, NULL);

    for (int processNumber = 1; processNumber <= 21; processNumber++) {
        pid_t pid = fork();
        if (pid == 0) {
            // 자식 프로세스에서 작업 수행
            setpriority(PRIO_PROCESS, 0, processNumber); // 우선순위 설정
            performMatrixMultiplication(processNumber);
            exit(0);
        } else if (pid > 0) {
            // 부모 프로세스에서 자식 프로세스 정보 기록
            gettimeofday(&endTime, NULL);
            formatTime(&startTime, startStr);
            formatTime(&endTime, endStr);

            printf("PID: %d | Start Time: %s | End Time: %s | ", processNumber, startStr, endStr);
            printf("Elapsed Time: %ld milliseconds\n", (endTime.tv_sec - startTime.tv_sec) * 1000 +
                   (endTime.tv_usec - startTime.tv_usec) / 1000);
            
            int status;
            wait(&status);
        } else {
            perror("Fork failed");
            return 1;
        }
    }

    return 0;
}
