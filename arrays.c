#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arrays.h"

static GC_Status g_status;

const GC_Status GC_GetStatus() {
    return g_status;
}

static inline void set_stat(GC_Status stat) {
    g_status = stat;
}

GC_Array *GC_NewArray(int n) {
    GC_Array *newArray = malloc(sizeof(GC_Array));
    void **initBuffer = malloc(sizeof(void *) * n);
    newArray->buffer = initBuffer;
    newArray->cap = n;
    newArray->len = 0;
    set_stat(GC_STAT_OK);
    return newArray;
}

void GC_ArrayFree(GC_Array *array) {
    GC_ArrayFreeWith_Raw(array, free);
}

void GC_ArrayFreeWith_Raw(GC_Array *array, void (*mem_free)(void *)) {
    for (int i = 0; i < array->len; i++) {
        mem_free(array->buffer[i]);
    }
    free(array->buffer);
    free(array);
    array = NULL;
    set_stat(GC_STAT_OK);
}

void GC_ArrayAppend(GC_Array *array, void *item) {
    if (array->len + 1 > array->cap) {
        void **newBuffer = malloc(sizeof(void *) * array->cap * 2);
        memcpy(newBuffer, array->buffer, sizeof(void *) * array->len);
        free(array->buffer);
        array->buffer = newBuffer;
        array->cap *= 2;
    }
    array->buffer[array->len] = item;
    array->len++;
    set_stat(GC_STAT_OK);
}

void GC_ArrayRemove(GC_Array *array, void *item) {
    GC_ArrayRemoveWith(array, item, free);
}

void GC_ArrayRemoveWith_Raw(GC_Array *array, void *item, void (*mem_free)(void *)) {
    int id = GC_ArrayFindIndex(array, item);

    if (GC_GetStatus() == GC_STAT_OK) {
        mem_free(item);
        for (int i = id + 1; i < array->len; i++) {
            array->buffer[i - 1] = array->buffer[i];
        }
        array->len--;
        set_stat(GC_STAT_OK);
    } else {
        printf("OUT\n");
        set_stat(GC_STAT_INDEX_OUT_OF_RANGE);
    }
}

void *GC_ArrayGet(GC_Array *array, int index) {
    if (0 <= index && index < array->len) {
        set_stat(GC_STAT_OK);
        return *(array->buffer + index);
    } else {
        set_stat(GC_STAT_INDEX_OUT_OF_RANGE);
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
            item = GC_ArrayGet(array, i);
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

int GC_ArrayFindIndex(GC_Array *array, void *item) {
    int id = -1;
    for (int i = 0; i < array->len; i++) {
        if (array->buffer[i] == item) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        set_stat(GC_STAT_INDEX_OUT_OF_RANGE);
    } else {
        set_stat(GC_STAT_OK);
    }
    return id;
}