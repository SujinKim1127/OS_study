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

// 유효한 입력인지 확인하는 함수
int isValidInput(const char* str) {
	int cnt = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        // 숫자, +, -, 공백, 엔터(\n) 외의 값이 들어오면
        if (!(isdigit(str[i]) || str[i] == '+' || str[i] == '-' || str[i] == ' ' || str[i] == '\n')) {
            return 0;
        }
        // 두 번 연산이 불가능하도록
        if(str[i] == '+' || str[i] == '-') { 
        	cnt++;
        }
        if(cnt > 1) return 0;
    }
    return 1;
}

// 공백이 같이 입력될 경우 공백을 없애주는 함수
char* DeleteSpace(char s[])
{
    char* str = (char*)malloc(sizeof(s));
    int i, k = 0;

    for (i = 0; i < strlen(s); i++)
        if (s[i] != ' ') str[k++] = s[i];

    str[k] = '\0';
    return str;
}

// 문자열에 연산자가 들어갈 경우
void calculateExpression(const char* expression) {
    int a, b;

    // 입력된 문자열에서 '+' 또는 '-' 기호를 찾아서 분리
    char *plusPtr = strchr(expression, '+');
    char *minusPtr = strchr(expression, '-');

    if (plusPtr != NULL) {
        // '+' 기호를 찾은 경우
        sscanf(expression, "%d+%d", &a, &b);
        int result = syscall(452, a, b);
        printf("Output: ");
        // b 값이 더 크면 값이 음수가 나오므로
        if (a < b) {
            printf("-%d\n", result);
        }
        else {
            printf("%d\n", result);
        }
    } else if (minusPtr != NULL) {
        // '-' 기호를 찾은 경우
        sscanf(expression, "%d-%d", &a, &b);
        int result = syscall(451, a, b);
        printf("Output: %d\n", result);
    }
}

// 숫자만 입력되었을 경우 자리수 count
int countDigits(int num) {
    int count = 0;
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

// 숫자 리버스하는 함수
void reverse(const char* expression) {
    int num;
    sscanf(expression, "%d", &num);
    int result = syscall(450, num);
    printf("Output: ");

    // 만약 숫자가 300이면 앞에 00을 붙여주도록
    if(countDigits(num) > countDigits(result)){
        for(int i = 0; i < countDigits(num) - countDigits(result); i++)
            printf("0");
    }
    printf("%d\n", result);

}


int main()
{
        char inputStr[1000];
        char output[1000];
        while(1) {
                printf("Input: ");
                fgets(inputStr, sizeof(inputStr), stdin);
                if (inputStr[0] == '\n') break;

                // 이상한 입력이 아니면
                if (isValidInput(inputStr)) {
                    // 입력값에 연산자가 있으면
                    if (isExpression(inputStr)) {
                        calculateExpression(DeleteSpace(inputStr));
                    } 
                    // 숫자만 들어오면
                    else {
                            reverse(inputStr);
                    }
                } 
                // 이상한 입력이 들어오면
                else {
                        printf("Wrong Input!\n");
                }
        }
        return 0;
}
