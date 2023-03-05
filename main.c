#include "arrays.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Int {
    int val;
} Int;

typedef struct Coord {
    int x, y;
} Coord;

Int *NewInt(int n) {
    Int *newInt = malloc(sizeof(Int));
    (*newInt).val = n;
    return newInt;
}

void DisplayIntArr(GC_Array *arr) {
    printf("Array: len: %d, cap: %d, [", arr->len, arr->cap);
    if (arr->len == 0) {
        printf("]\n");
    } else {
        for (int i = 0; i < arr->len; i++) {
            Int *item = GC_ArrayGet(arr, i, NULL);
            if (i == arr->len - 1) {
                printf("%d]\n", (*item).val);
            } else {
                printf("%d, ", (*item).val);
            }
        }
    }
}

int main() {
    GC_Array *intArray = GC_NewArray(16, NULL);
    DisplayIntArr(intArray);
    for (int i = 1; i <= 40; i++) {
        Int *newInt = NewInt(i);
        printf("Append: NewInt.val = %d\n", (*newInt).val);
        GC_ArrayAppend(intArray, newInt, NULL);
        DisplayIntArr(intArray);
    }
    GC_ArrayFree(intArray, NULL);
}
