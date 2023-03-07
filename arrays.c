#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arrays.h"

static GStatus g_status;

const GStatus GA_GetStatus() {
    return g_status;
}

static inline void set_stat(GStatus stat) {
    g_status = stat;
}

GArray *GA_New(int n) {
    GArray *newArray = malloc(sizeof(GArray));
    void **initBuffer = malloc(sizeof(void *) * n);
    newArray->buffer = initBuffer;
    newArray->cap = n;
    newArray->len = 0;
    set_stat(GC_STAT_OK);
    return newArray;
}

void GA_Free(GArray *array) {
    GA_FreeWith_Raw(array, free);
}

void GA_FreeWith_Raw(GArray *array, void (*mem_free)(void *)) {
    for (int i = 0; i < array->len; i++) {
        mem_free(array->buffer[i]);
    }
    free(array->buffer);
    free(array);
    array = NULL;
    set_stat(GC_STAT_OK);
}

void GA_Append(GArray *array, void *item) {
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

void GA_Remove(GArray *array, void *item) {
    GA_RemoveWith_Raw(array, item, free);
}

void GA_RemoveWith_Raw(GArray *array, void *item, void (*mem_free)(void *)) {
    int id = GA_FindIndex(array, item);

    if (GA_GetStatus() == GC_STAT_OK) {
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

void *GA_Get(GArray *array, int index) {
    if (0 <= index && index < array->len) {
        set_stat(GC_STAT_OK);
        return *(array->buffer + index);
    } else {
        set_stat(GC_STAT_INDEX_OUT_OF_RANGE);
        return NULL;
    }
}

void GA_Print_Raw(GArray *array, void (*str)(void*, char*), int endline, int info) {
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
            item = GA_Get(array, i);
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

int GA_FindIndex(GArray *array, void *item) {
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