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
    int count = 0;
    int A[100][100], B[100][100];
    long result[100][100];

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
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset); // CPU 코어 번호를 여기에 지정 (0번 코어)

    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        perror("sched_setaffinity");
    }

    gettimeofday(&startTime, NULL);
    
    int totalElapsedTimePipe = 0; // 전체 elapsed time의 합 (파이프로 통신)

    int pipefd[2]; // 파이프용 파일 디스크립터 배열
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    for (int processNumber = 1; processNumber <= 21; processNumber++) {
        pid_t pid = fork();
        if (pid == 0) {
            // 자식 프로세스에서 작업 수행
            if (processNumber <= 7) {
                setpriority(PRIO_PROCESS, 0, -20); // 첫 7개 프로세스는 NICE를 -20으로 설정
            } else if (processNumber <= 14 && processNumber > 7) {
                setpriority(PRIO_PROCESS, 0, 0); // 그 다음 7개 프로세스는 NICE를 0으로 설정
            } else {
                setpriority(PRIO_PROCESS, 0, 19); // 나머지 7개 프로세스는 NICE를 19로 설정
            }
            int niceValue = getpriority(PRIO_PROCESS, 0); // NICE 값을 얻어옴
            performMatrixMultiplication(processNumber);
            gettimeofday(&endTime, NULL);
            formatTime(&startTime, startStr);
            formatTime(&endTime, endStr);
            printf("PID: %d | NICE: %d | Start Time: %s | End Time: %s | ", processNumber, niceValue, startStr, endStr);
            long elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000;
            printf("Elapsed Time: %ld milliseconds\n", elapsedTime);

            // elapsedTime를 부모 프로세스로 전달
            if (write(pipefd[1], &elapsedTime, sizeof(elapsedTime)) == -1) {
                perror("write");
            }
            
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

    // 모든 자식 프로세스의 elapsedTime을 읽어 누적
    for (int i = 0; i < 21; i++) {
        int elapsedTime;
        if (read(pipefd[0], &elapsedTime, sizeof(elapsedTime)) == -1) {
            perror("read");
        }
        totalElapsedTimePipe += elapsedTime;
    }

    printf("Scheduling Policy: CFS_NICE  |   Average Elapsed Time: %.2f microseconds\n", (double)totalElapsedTimePipe / 21);

    return 0;
}
