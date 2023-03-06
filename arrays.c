#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arrays.h"

#define SET_STAT(stat, value) if (stat != NULL) *stat = (value);

GC_Array *GC_NewArray(int n, GC_Status *stat) {
    GC_Array *newArray = malloc(sizeof(GC_Array));
    void **initBuffer = malloc(sizeof(void *) * n);
    newArray->buffer = initBuffer;
    newArray->cap = n;
    newArray->len = 0;
    SET_STAT(stat, GC_STAT_OK);
    return newArray;
}

void GC_ArrayFree(GC_Array *array, GC_Status *stat) {
    GC_ArrayFreeWith(array, free, stat);
}

void GC_ArrayFreeWith(GC_Array *array, void (*mem_free)(void *), GC_Status *stat) {
    for (int i = 0; i < array->len; i++) {
        mem_free(array->buffer[i]);
    }
    free(array->buffer);
    free(array);
    array = NULL;
    SET_STAT(stat, GC_STAT_OK);
}

void GC_ArrayAppend(GC_Array *array, void *item, GC_Status *stat) {
    if (array->len + 1 > array->cap) {
        void **newBuffer = malloc(sizeof(void *) * array->cap * 2);
        memcpy(newBuffer, array->buffer, sizeof(void *) * array->len);
        free(array->buffer);
        array->buffer = newBuffer;
        array->cap *= 2;
    }
    array->buffer[array->len] = item;
    array->len++;
    SET_STAT(stat, GC_STAT_OK);
}

void GC_ArrayRemove(GC_Array *array, void *item, GC_Status *stat) {
    GC_ArrayRemoveWith(array, item, free, stat);
}

void GC_ArrayRemoveWith(GC_Array *array, void *item, void (*mem_free)(void *), GC_Status *stat) {
    GC_Status id_stat;
    int id = GC_ArrayFindIndex(array, item, &id_stat);

    if (id_stat == GC_STAT_OK) {
        mem_free(item);
        for (int i = id + 1; i < array->len; i++) {
            array->buffer[i - 1] = array->buffer[i];
        }
        array->len--;
        SET_STAT(stat, GC_STAT_OK);
    } else {
        printf("OUT\n");
        SET_STAT(stat, GC_STAT_INDEX_OUT_OF_RANGE);
    }
}

void *GC_ArrayGet(GC_Array *array, int index, GC_Status *stat) {
    if (0 <= index && index < array->len) {
        SET_STAT(stat, GC_STAT_OK);
        return *(array->buffer + index);
    } else {
        SET_STAT(stat, GC_STAT_INDEX_OUT_OF_RANGE);
        return NULL;
    }
}

void GC_ArrayPrint_Raw(GC_Array *array, void (*str)(void*, char*), int endline, int info) {
    char buffer[256];
    void *item;
    if (info) {
        printf("Array: len = %d, cap = %d ", array->len, array->cap);
    }
    printf("[");
    if (array->len == 0) {
        printf("]");
    } else {
        for (int i = 0; i < array->len; i++) {
            item = GC_ArrayGet(array, i, NULL);
            if (i == array->len - 1) {
                str(item, buffer);
                printf("%s]", buffer);
            } else {
                str(item, buffer);
                printf("%s, ", buffer);
            }
        }
    }
    if (endline) {
        printf("\n");
    }
}

int GC_ArrayFindIndex(GC_Array *array, void *item, GC_Status *stat) {
    int id = -1;
    for (int i = 0; i < array->len; i++) {
        if (array->buffer[i] == item) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        SET_STAT(stat, GC_STAT_INDEX_OUT_OF_RANGE);
    } else {
        SET_STAT(stat, GC_STAT_OK);
    }
    return id;
}