#ifndef GC_ARRAYS_H
#define GC_ARRAYS_H

typedef enum GC_Status {
    GC_STAT_OK,
    GC_STAT_INDEX_OUT_OF_RANGE,
    GC_UNKNOWN_ERROR,
} GC_Status;

typedef struct GC_Array {
    void **buffer;
    int cap, len;
} GC_Array;

GC_Array *GC_NewArray(int n, GC_Status *stat);
void GC_ArrayFree(GC_Array *array, GC_Status *stat);
void GC_ArrayFreeWith(GC_Array *array, void (*mem_free)(void *), GC_Status *stat);
void GC_ArrayAppend(GC_Array *array, void *item, GC_Status *stat);
void GC_ArrayRemove(GC_Array *array, void *item, GC_Status *stat);
void GC_ArrayRemoveWith(GC_Array *array, void *item, void (*mem_free)(void *), GC_Status *stat);
void *GC_ArrayGet(GC_Array *array, int index, GC_Status *stat);
void GC_ArrayPrint_Raw(GC_Array *array, void (*str)(void*, char*), int endline, int info);
int GC_ArrayFindIndex(GC_Array *array, void *item, GC_Status *stat);

#define GC_ArrayPrintInfo(array, str) GC_ArrayPrint_Raw((array), (void (*)(void *, char *))(str), 0, 1)
#define GC_ArrayPrintlnInfo(array, str) GC_ArrayPrint_Raw((array), (void (*)(void *, char *))(str), 1, 1)
#define GC_ArrayPrint(array, str) GC_ArrayPrint_Raw((array), (void (*)(void *, char *))(str), 0, 0)
#define GC_ArrayPrintln(array, str) GC_ArrayPrint_Raw((array), (void (*)(void *, char *))(str), 1, 0)

#endif