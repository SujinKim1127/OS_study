#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <linux/kernel.h>
// #include <sys/syscall.h>
// #include <unistd.h>


// 입력받은 숫자(문자열)를 거꾸로 출력해주는 함수
void reverseAndPrintString(const char* str) {
    int length = strlen(str);
    printf("Output: ");
    for (int i = length - 2; i >= 0; i--) {
        printf("%c", str[i]);
    }
}

// 입력받은 문자열에 연산자가 있는지 확인하는 함수
int isExpression(const char* str) {
    // 문자열에 '+' 또는 '-' 연산자가 있는지 확인
    return strchr(str, '+') != NULL || strchr(str, '-') != NULL;
}

void calculateExpression(const char* expression) {
    int a, b;

        // 입력된 문자열에서 '+' 또는 '-' 기호를 찾아서 분리
    char *plusPtr = strchr(expression, '+');
    char *minusPtr = strchr(expression, '-');

    if (plusPtr != NULL) {
        // '+' 기호를 찾은 경우
        sscanf(expression, "%d+%d", &a, &b); // '+' 기호를 기준으로 정수 a와 b를 읽음
        int result = a - b;
        printf("Output: %d\n", result);

    } else if (minusPtr != NULL) {
        // '-' 기호를 찾은 경우
        sscanf(expression, "%d-%d", &a, &b); // '-' 기호를 기준으로 정수 a와 b를 읽음
        int result = a + b;
        printf("Output: %d\n", result);
    }

}

int main() {
    char inputStr[1000];

    while (1) {
        printf("Input: ");
        fgets(inputStr, sizeof(inputStr), stdin); // 사용자 입력 받기
        if (inputStr[0] == '\n') { // Enter 키를 눌렀을 때 종료
            break; // 루프를 종료하고 프로그램을 종료
        }

        if (isExpression(inputStr)) {
            calculateExpression(inputStr);
        } else {
            reverseAndPrintString(inputStr);
            printf("\n");
        }
    }

    return 0;
}
