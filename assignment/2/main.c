#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

// 입력받은 문자열에 연산자가 있는지 확인하는 함수
int isExpression(const char* str) {
    // 문자열에 '+' 또는 '-' 연산자가 있는지 확인
    return strchr(str, '+') != NULL || strchr(str, '-') != NULL;
}

int isValidInput(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!(isdigit(str[i]) || str[i] == '+' || str[i] == '-' || str[i] == '\n')) {
            return 0;
        }
    }
    return 1;
}

void calculateExpression(const char* expression) {
    int a, b;

        // 입력된 문자열에서 '+' 또는 '-' 기호를 찾아서 분리
    char *plusPtr = strchr(expression, '+');
    char *minusPtr = strchr(expression, '-');

    if (plusPtr != NULL) {
                    // '+' 기호를 찾은 경우
        sscanf(expression, "%d+%d", &a, &b); // '+' 기호를 기준으로 정수 a와 b를 읽음
        int result = syscall(452, a, b);
        printf("Output: %d", result);

    } else if (minusPtr != NULL) {
        // '-' 기호를 찾은 경우
        sscanf(expression, "%d-%d", &a, &b); // '-' 기호를 기준으로 정수 a와 b를 읽음
        syscall(451, a, b);
        int result = syscall(452, a, b);
        printf("Output: %d", result);

    }

}


int main()
{
        char inputStr[1000];
        while(1) {
                printf("Input: ");
                fgets(inputStr, sizeof(inputStr), stdin);
                if (inputStr[0] == '\n') break;

                if (isValidInput(inputStr)) {
                    if (isExpression(inputStr)) {
                        calculateExpression(inputStr);
                    } else {
                            syscall(450, inputStr);
                    }
                } else {
                        printf("Wrong Input!\n");
                }

        }
        return 0;
}

