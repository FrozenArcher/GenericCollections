#ifndef GC_ARRAYS_H
#define GC_ARRAYS_H

typedef enum GStatus {
    GC_STAT_OK,
    GC_STAT_INDEX_OUT_OF_RANGE,
    GC_UNKNOWN_ERROR,
} GStatus;

/**
 * 动态数组
 * 托管指针的生命周期
 */
typedef struct GArray {
    void **buffer;
    int cap, len;
} GArray;

const GStatus GA_GetStatus();
GArray *GA_New(int n);
void GA_Free(GArray *array);
void GA_FreeWith_Raw(GArray *array, void (*mem_free)(void *));
void GA_Append(GArray *array, void *item);
void GA_Remove(GArray *array, void *item);
void GA_RemoveWith_Raw(GArray *array, void *item, void (*mem_free)(void *));
void *GA_Get(GArray *array, int index);
void GA_Print_Raw(GArray *array, void (*str)(void*, char*), int endline, int info);
int GA_FindIndex(GArray *array, void *item);

#define GA_PrintInfo(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 0, 1)
#define GA_PrintlnInfo(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 1, 1)
#define GA_Print(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 0, 0)
#define GA_Println(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 1, 0)
#define GA_RemoveWith(array, item, mem_free) GA_RemoveWith_Raw((array), (item), (void (*)(void *))(mem_free))
#define GA_FreeWith(array, mem_free) GA_FreeWith_Raw((array), (void (*)(void *))(mem_free))

#endif