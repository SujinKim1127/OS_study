#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    while(1) {
        printf("Input the Scheduling Polity to apply:\n");
        printf("1. CFS_DEFAULT\n");
        printf("2. CFS_NICE\n");
        printf("3. RT_FIFO\n");
        printf("4. RT_RR\n");
        printf("0. Exit\n");
        printf("Input: ");
        int scanNum;
        scanf("%d", &scanNum);

        if(scanNum == 1) {          // cfs default
            int status;
            char *compileCommand = "gcc -o CFS_DEFAULT CFS_DEFAULT.c -lm"; // 컴파일 명령어
            // CPU 1개만 사용
            char *executionCommand = "taskset 0x1 ./CFS_DEFAULT"; // 실행 명령어

            // 컴파일 명령어 실행
            status = system(compileCommand);
            if (status != 0) {
                perror("Compilation failed");
                return 1;
            }

            // 실행 명령어 실행
            status = system(executionCommand);
            if (status != 0) {
                perror("Execution failed");
                return 1;
            }
        } else if (scanNum == 2) {      // cfs nice
            int status;
            char *compileCommand = "gcc -o CFS_NICE CFS_NICE.c -lm"; // 컴파일 명령어
            char *executionCommand = "./CFS_NICE"; // 실행 명령어

            // 컴파일 명령어 실행
            status = system(compileCommand);
            if (status != 0) {
                perror("Compilation failed");
                return 1;
            }

            // 실행 명령어 실행
            status = system(executionCommand);
            if (status != 0) {
                perror("Execution failed");
                return 1;
            }
        } else if (scanNum == 3) {      // RT FIFO
            int status;
            char *compileCommand = "gcc -o RT_FIFO RT_FIFO.c -lm"; // 컴파일 명령어
            char *executionCommand = "./RT_FIFO"; // 실행 명령어

            // 컴파일 명령어 실행
            status = system(compileCommand);
            if (status != 0) {
                perror("Compilation failed");
                return 1;
            }

            // 실행 명령어 실행
            status = system(executionCommand);
            if (status != 0) {
                perror("Execution failed");
                return 1;
            }
        } else if (scanNum == 4) {      // RT_RR
            int status;
            char *compileCommand = "gcc -o RT_RR RT_RR.c -lm"; // 컴파일 명령어
            char *executionCommand = "./RT_RR"; // 실행 명령어

            // 컴파일 명령어 실행
            status = system(compileCommand);
            if (status != 0) {
                perror("Compilation failed");
                return 1;
            }

            // 실행 명령어 실행
            status = system(executionCommand);
            if (status != 0) {
                perror("Execution failed");
                return 1;
            }
        } else {
            break;
        }
        printf("\n");
    }
    return 0;
}
