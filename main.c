#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arrays.h"

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
    printf("Free string \"%s\"\n", string->string);
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

/* Tests */
void TestIntArrayIncrement() {
    GArray *intArray = GA_New(16);
    GA_PrintlnInfo(intArray, IntToString);
    for (int i = 1; i <= 40; i++) {
        Int *newInt = NewInt(i);
        printf("Append: NewInt.val = %d\n", newInt->val);
        GA_Append(intArray, newInt);
        GA_PrintlnInfo(intArray, IntToString);
    }
    GA_FreeAll(intArray);
}

void TestStringArrayDailyUse() {
    GArray *strings = GA_New(4);
    GA_PrintlnInfo(strings, StringToStr);
    GA_Append(strings, NewStr("Arch"));
    GA_Append(strings, NewStr("Linux"));
    GA_Append(strings, NewStr("is"));
    GA_Append(strings, NewStr("the"));
    GA_Append(strings, NewStr("best"));
    GA_PrintlnInfo(strings, StringToStr);
    String *bestStr = GA_Get(strings, 4);
    printf("Got: \"%s\" @ %d\n", bestStr->string, GA_FindIndex(strings, bestStr));
    GA_Remove(strings, bestStr);
    FreeStr(bestStr);
    GA_PrintlnInfo(strings, StringToStr);
    GA_Append(strings, NewStr("worst"));
    GA_PrintlnInfo(strings, StringToStr);
    GA_KillWith(strings, GA_Get(strings, 0), FreeStr);
    GA_PrintlnInfo(strings, StringToStr);
    String *someStr = GA_RemoveAt(strings, 2);
    GA_Println(strings, StringToStr);
    FreeStr(someStr);

    GA_FreeAllWith(strings, FreeStr);
}

int main() {
    //TestIntArrayIncrement();
    TestStringArrayDailyUse();
    return 0;
}
