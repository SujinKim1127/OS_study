#include <stdio.h>
#include <string.h>

// 입력받은 문자열을 역순으로 저장하는 함수
void reverseString(const char* input, char* output) {
    int length = strlen(input);
    int j = 0;

    int i = length - 2;
        while (i >= 0) {
                output[j++] = input[i];
                i--;
        }
        
    output[j] = '\0'; // 문자열의 끝을 표시
}

#include <stdio.h>

void reverseAndPrintNumber(int num) {
    int reversedNum = 0;
    int originalNum = num;
    
    // 숫자를 거꾸로 만들기
    while (num > 0) {
        int digit = num % 10;
        reversedNum = reversedNum * 10 + digit;
        num /= 10;
    }
    
    // 원래 숫자의 자릿수에 맞게 출력
    int numDigits = 0;
    while (originalNum > 0) {
        originalNum /= 10;
        numDigits++;
    }
    
    for (int i = 0; i < numDigits; i++) {
        int digit = reversedNum % 10;
        printf("%d", digit);
        reversedNum /= 10;
    }
    
    printf("\n");
}

int reverseAndPrint(int num) {
    char str[1000];
    int cnt = 0;
    int reverse=0;

    while (num > 0) {
            int digit = num % 10;
            reverse = reverse * 10 + digit;
            str[cnt] = digit + '0';
            num /= 10;
            cnt++;
        }
    str[cnt] = '\0';


    printf("Output: %s\n", str);    
    return reverse;
}

int main() {
    int num = 300; // 원하는 숫자를 여기에 넣어주세요.
    int result = reverseAndPrint(num);
    printf("asdfasd  %d", result);
    return 0;
}

// int main() {
//     char inputStr[1000];
//     char reversedStr[1000]; // 역순으로 저장할 문자열

//     printf("Input: ");
//     fgets(inputStr, sizeof(inputStr), stdin);

//     reverseString(inputStr, reversedStr);
    
//     printf("Output: %s\n", reversedStr);

//     return 0;
// }



#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(print_reverse, int, num)
{
        
        char str[1000];         // 커널에 출력하기 위한 문자열
        int cnt = 0;
        int reverse=0;          // return 해줄 리버스 값

        // 입력받은 숫자 리버스 해주는 함수
        while (num > 0) {
                int digit = num % 10;
                reverse = reverse * 10 + digit;
                str[cnt] = digit + '0';
                num /= 10;
                cnt++;
         }
        str[cnt] = '\0';

        // 커널에 문자열 출력
        printk("Output: %s\n", str);
        return reverse;
}

