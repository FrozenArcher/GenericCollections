#ifndef GC_ARRAYS_H
#define GC_ARRAYS_H

typedef enum GA_Status {
    GA_STAT_OK,
    GA_STAT_INDEX_OUT_OF_RANGE,
    GA_STAT_ITEM_NOT_FOUND,
    GA_UNKNOWN_ERROR,
} GA_Status;

typedef enum GA_Error {
    GA_ERROR_NO_ERROR,
} GA_Error;

/**
 * 动态数组
 */
typedef struct GArray {
    void **buffer;
    int cap, len;
} GArray;

const GA_Status GA_GetStatus();
GArray *GA_New(int n);
void GA_FreeAll(GArray *array);
void GA_FreeAllWith_Raw(GArray *array, void (*mem_free)(void *));
void GA_Append(GArray *array, void *item);
void GA_Kill(GArray *array, void *item);
void GA_KillWith_Raw(GArray *array, void *item, void (*mem_free)(void *));
void *GA_Get(GArray *array, int index);
void GA_Print_Raw(GArray *array, void (*str)(void*, char*), int endline, int info);
int GA_FindIndex(GArray *array, void *item);
void *GA_RemoveAt(GArray *array, int index);
void GA_Remove(GArray *array, void *item);
// void *GA_Replace()
// void *GA_ReplaceAt()
// void GA_AddAt()
// void GA_Swap()
// void GA_SwapAt()

// const void * const GA_GetBuffer() [*]

#define GA_PrintInfo(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 0, 1)
#define GA_PrintlnInfo(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 1, 1)
#define GA_Print(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 0, 0)
#define GA_Println(array, str) GA_Print_Raw((array), (void (*)(void *, char *))(str), 1, 0)
#define GA_KillWith(array, item, mem_free) GA_KillWith_Raw((array), (item), (void (*)(void *))(mem_free))
#define GA_FreeAllWith(array, mem_free) GA_FreeAllWith_Raw((array), (void (*)(void *))(mem_free))

#endif