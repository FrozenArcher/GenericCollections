#include "arrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Int {
    int val;
} Int;

typedef struct String {
    char *string;
} String;

String *NewStr(char *str) {
    String *newString = malloc(sizeof(String));
    char *newBuffer = malloc(sizeof(char) * strlen(str) + 1);
    strcpy(newBuffer, str);
    newBuffer[strlen(str)] = 0;
    newString->string = newBuffer;
    return newString;
}

void FreeStr(String *string) {
    free(string->string);
    free(string);
}

void StringToStr(String *string, char *buffer) {
    sprintf(buffer, "\"%s\"", string->string);
}

Int *NewInt(int n) {
    Int *newInt = malloc(sizeof(Int));
    (*newInt).val = n;
    return newInt;
}

void IntToString(Int *num, char *buffer) {
    sprintf(buffer, "%d", num->val);
}

int main() {
    GC_Array *intArray = GC_NewArray(16, NULL);
    GC_ArrayPrintlnInfo(intArray, IntToString);
    for (int i = 1; i <= 40; i++) {
        Int *newInt = NewInt(i);
        printf("Append: NewInt.val = %d\n", (*newInt).val);
        GC_ArrayAppend(intArray, newInt, NULL);
        GC_ArrayPrintlnInfo(intArray, IntToString);
    }
    GC_ArrayFree(intArray, NULL);

    GC_Array *strings = GC_NewArray(4, NULL);
    GC_ArrayPrintlnInfo(strings, StringToStr);
    GC_ArrayAppend(strings, NewStr("Arch"), NULL);
    GC_ArrayAppend(strings, NewStr("Linux"), NULL);
    GC_ArrayAppend(strings, NewStr("is"), NULL);
    GC_ArrayAppend(strings, NewStr("the"), NULL);
    GC_ArrayAppend(strings, NewStr("best"), NULL);
    GC_ArrayPrintlnInfo(strings, StringToStr);
    String *bestStr = GC_ArrayGet(strings, 4, NULL);
    printf("Got: \"%s\" @ %d\n", bestStr->string, GC_ArrayFindIndex(strings, bestStr, NULL));
    GC_ArrayRemoveWith(strings, bestStr, (void(*)(void*))(FreeStr), NULL);
    GC_ArrayPrintlnInfo(strings, StringToStr);
    GC_ArrayAppend(strings, NewStr("worst"), NULL);
    GC_ArrayPrintlnInfo(strings, StringToStr);
    GC_ArrayRemoveWith(strings, GC_ArrayGet(strings, 0, NULL), (void(*)(void*))(FreeStr), NULL);
    GC_ArrayPrintlnInfo(strings, StringToStr);

    GC_ArrayFreeWith(strings, (void(*)(void*))(FreeStr), NULL);
}
