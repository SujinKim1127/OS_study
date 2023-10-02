#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void calculateExpression(const char* plusminus) {
    int a, b;

    char *plusPtr = strchr(plusminus, '+');

    sscanf(plusminus, "%d+%d", &a, &b); // '+' 기호를 기준으로 정수 a와 b를 읽음
    int result = a - b;
    printf("Output: %d\n", result);

}

int main() {
    char inputStr[1000];

    printf("Input: ");
    fgets(inputStr, sizeof(inputStr), stdin); 

    calculateExpression(inputStr);

    return 0;
}