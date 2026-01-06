#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/integral.h"

int main(void) {
    int32_t *array = (int32_t *)malloc(sizeof(int32_t) * 5);
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;
    array[4] = 5;
    printf("hello world: %d\n", MinimumI32(array, 5));
    return 0;
}
