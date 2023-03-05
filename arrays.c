#include "arrays.h"

#include "stdlib.h"
#include "string.h"

GC_Array *GC_NewArray(int n, GC_Status *stat) {
    GC_Array *newArray = malloc(sizeof(GC_Array));
    void **initBuffer = malloc(sizeof(void *) * n);
    newArray->buffer = initBuffer;
    newArray->cap = n;
    newArray->len = 0;
    if (stat != NULL) *stat = GC_STAT_OK;
    return newArray;
}

void GC_ArrayFree(GC_Array *array, GC_Status *stat) {
    for (int i = 0; i < array->len; i++) {
        free(array->buffer[i]);
    }
    free(array->buffer);
    free(array);
    array = NULL;
    if (stat != NULL) *stat = GC_STAT_OK;
}

// GC_Status GC_FreeArrayWith();
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
    if (stat != NULL) *stat = GC_STAT_OK;
}

void GC_ArrayRemove(GC_Array *array, void **item, GC_Status *stat) {
    if (array->buffer <= item && item < array->buffer + array->len) {
        free(*item);
        for (void **p = item + 1; p < array->buffer + array->len; p++) {
            *(p - 1) = *p;
        }
        array->len--;
        if (stat != NULL) *stat = GC_STAT_OK;
    } else {
        if (stat != NULL) *stat = GC_STAT_INDEX_OUT_OF_RANGE;
    }
}

void *GC_ArrayGet(GC_Array *array, int index, GC_Status *stat) {
    if (0 <= index && index < array->len) {
        if (stat != NULL) *stat = GC_STAT_OK;
        return array->buffer + index;
    } else {
        if (stat != NULL) *stat = GC_STAT_INDEX_OUT_OF_RANGE;
        return NULL;
    }
}