#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arrays.h"

static GStatus g_status = GSTAT_INIT;
static GError g_error = GERROR_NO_ERROR;

const GStatus GA_GetStatus() {
    return g_status;
}

const GError GA_GetError() {
    return g_error;
}

static inline void set_stat_ok() {
    g_status = GSTAT_OK;
    g_error = GERROR_NO_ERROR;
}

static inline void set_stat_error(GError error) {
    g_status = GSTAT_ERROR;
    g_error = error;
}

static void ga_inc_cap(GArray *array) {
    void **newBuffer = malloc(sizeof(void *) * array->cap * 2);
    memcpy(newBuffer, array->buffer, sizeof(void *) * array->len);
    free(array->buffer);
    array->buffer = newBuffer;
    array->cap *= 2;
}

GArray *GA_New(int n) {
    GArray *newArray = malloc(sizeof(GArray));
    void **initBuffer = malloc(sizeof(void *) * n);
    newArray->buffer = initBuffer;
    newArray->cap = n;
    newArray->len = 0;
    set_stat_ok();
    return newArray;
}

void GA_FreeAll(GArray *array) {
    GA_FreeAllWith_Raw(array, free);
}

void GA_FreeAllWith_Raw(GArray *array, void (*mem_free)(void *)) {
    for (int i = 0; i < array->len; i++) {
        mem_free(array->buffer[i]);
    }
    free(array->buffer);
    free(array);
    array = NULL;
    set_stat_ok();
}

void GA_Append(GArray *array, void *item) {
    if (array->len + 1 > array->cap) {
        ga_inc_cap(array);
    }
    array->buffer[array->len] = item;
    array->len++;
    set_stat_ok();
}

void GA_Kill(GArray *array, void *item) {
    GA_KillWith_Raw(array, item, free);
}

void GA_KillWith_Raw(GArray *array, void *item, void (*mem_free)(void *)) {
    int id = GA_FindIndex(array, item);

    if (GA_GetStatus() == GSTAT_OK) {
        mem_free(item);
        for (int i = id + 1; i < array->len; i++) {
            array->buffer[i - 1] = array->buffer[i];
        }
        array->len--;
        set_stat_ok();
    } else {
        //printf("OUT\n");
        set_stat_error(GERROR_INDEX_OUT_OF_RANGE);
    }
}

void *GA_Get(GArray *array, int index) {
    if (0 <= index && index < array->len) {
        set_stat_ok();
        return *(array->buffer + index);
    } else {
        set_stat_error(GERROR_INDEX_OUT_OF_RANGE);
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
        set_stat_error(GERROR_ITEM_NOT_FOUND);
    } else {
        set_stat_ok();
    }
    return id;
}

void *GA_RemoveAt(GArray *array, int index) {
    void *item = GA_Get(array, index);
    if (GA_GetStatus() == GSTAT_OK) {
        item = array->buffer[index];
        for (int i = index + 1; i < array->len; i++) {
            array->buffer[i - 1] = array->buffer[i];
        }
        array->len--;
        set_stat_ok();
    } else {
        set_stat_error(GERROR_ITEM_NOT_FOUND);
    }
    return item;
}

void GA_Remove(GArray *array, void *item) {
    int index = GA_FindIndex(array, item);
    if (GA_GetStatus() == GSTAT_OK) {
        GA_RemoveAt(array, index);
        set_stat_ok();
    } else {
        set_stat_error(GERROR_ITEM_NOT_FOUND);
    }
}

void GA_Replace(GArray *array, void *old_item, void *new_item) {
    int index = GA_FindIndex(array, old_item);
    if (GA_GetStatus() == GSTAT_OK) {
        GA_ReplaceAt(array, index, new_item);
        set_stat_ok();
        return;
    } else {
        set_stat_error(GERROR_ITEM_NOT_FOUND);
        return;
    }
}

void *GA_ReplaceAt(GArray *array, int index, void *new_item) {
    if (0 <= index && index < array->len) {
        void *old_item = array->buffer[index];
        array->buffer[index] = new_item;
        set_stat_ok();
        return old_item;
    } else {
        set_stat_error(GERROR_INDEX_OUT_OF_RANGE);
        return NULL;
    }
}

void GA_Swap(GArray *array, void *item_1, void *item_2) {
    int i1 = GA_FindIndex(array, item_1);
    if (GA_GetStatus() == GSTAT_OK) {
        int i2 = GA_FindIndex(array, item_2);
        if (GA_GetStatus() == GSTAT_OK) {
            GA_SwapAt(array, i1, i2);
        } else {
            set_stat_error(GERROR_ITEM_NOT_FOUND);
            return;
        }
    } else {
        set_stat_error(GERROR_ITEM_NOT_FOUND);
        return;
    }
}
void GA_SwapAt(GArray *array, int index_1, int index_2) {
    if ((0 <= index_1 && index_1 < array->len) && (0 <= index_2 && index_2 < array->len)) {
        void *t_item = array->buffer[index_1];
        array->buffer[index_1] = array->buffer[index_2];
        array->buffer[index_2] = t_item;
        set_stat_ok();
        return;
    } else {
        set_stat_error(GERROR_INDEX_OUT_OF_RANGE);
        return;
    }
}

void GA_AddAt(GArray *array, int index, void *item) {
    if (0 <= index && index <= array->len) {
        if (array->len + 1 > array->cap) {
            ga_inc_cap(array);
        }
        for (int i = array->len; i > index; i--) {
            array->buffer[i] = array->buffer[i - 1];
        }
        array->buffer[index] = item;
        array->len++;
        set_stat_ok();
        return;
    } else {
        set_stat_error(GERROR_INDEX_OUT_OF_RANGE);
        return;
    }
}