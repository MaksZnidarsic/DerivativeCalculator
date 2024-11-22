


//#include <time.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"


int main() {
    /*clock_t time_start = clock();*/

    const char* expression = "a + b + c + d";
    const char* pos = expression;

    while (pos < expression + strlen(expression)) {
        Token t = scan(&pos);
        printf("%u\n", t.type);
    }

    /*clock_t time_end = clock();
    printf("time %fs\n", (float)(time_end - time_start) / (CLOCKS_PER_SEC));*/
}
