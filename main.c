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
    GArray *intArray = GA_New(16, IntToString, NULL);
    GA_PrintlnInfo(intArray);
    for (int i = 1; i <= 40; i++) {
        Int *newInt = NewInt(i);
        printf("Append: NewInt.val = %d\n", newInt->val);
        GA_Append(intArray, newInt);
        GA_PrintlnInfo(intArray);
    }
    GA_FreeAll(intArray);
}

void TestStringArrayDailyUse() {
    GArray *strings = GA_New(4, StringToStr, FreeStr);
    GA_PrintlnInfo(strings);
    GA_Append(strings, NewStr("Arch"));
    GA_Append(strings, NewStr("Linux"));
    GA_Append(strings, NewStr("is"));
    GA_Append(strings, NewStr("the"));
    GA_Append(strings, NewStr("best"));
    GA_PrintlnInfo(strings);
    String *bestStr = GA_Get(strings, 4);
    printf("Got: \"%s\" @ %d\n", bestStr->string, GA_FindIndex(strings, bestStr));
    GA_Remove(strings, bestStr);
    FreeStr(bestStr);
    GA_PrintlnInfo(strings);
    GA_Append(strings, NewStr("worst"));
    GA_PrintlnInfo(strings);
    GA_Kill(strings, GA_Get(strings, 0));
    GA_PrintlnInfo(strings);
    String *someStr = GA_RemoveAt(strings, 2);
    GA_Println(strings);
    FreeStr(someStr);
    String *mystr = GA_Get(strings, 2);
    String *newString = NewStr("beeeest");
    GA_Replace(strings, mystr, newString);
    GA_Println(strings);
    FreeStr(mystr);
    GA_SwapAt(strings, 0, 1);
    GA_Println(strings);
    GA_Swap(strings, GA_Get(strings, 0), GA_Get(strings, 1));
    GA_Println(strings);
    GA_AddAt(strings, 0, NewStr("Gentoo"));
    GA_AddAt(strings, 3, NewStr("the"));
    GA_Println(strings);

    puts("");
    GA_FreeAll(strings);
}

int main() {
    //TestIntArrayIncrement();
    TestStringArrayDailyUse();
    return 0;
}
