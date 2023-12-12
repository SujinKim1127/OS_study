#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void calculateExpression(const char* expression) {
    int a, b;

    char *minusPtr = strchr(expression, '-');
    sscanf(expression, "%d-%d", &a, &b); 
    
    int result = a + b;
    printf("Output: %d\n", result);
}

int main() {
    char inputStr[1000];

    printf("Input: ");
    fgets(inputStr, sizeof(inputStr), stdin); 

    calculateExpression(inputStr);

    return 0;

}

#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(minus_to_plus, int, a,int, b)
{
        int result = a + b;
        printk("Output: %d\n", result);

        return result;
}

