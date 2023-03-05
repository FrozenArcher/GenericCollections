#ifndef GC_ARRAYS_H
#define GC_ARRAYS_H

typedef enum GC_Status {
    GC_STAT_OK,
    GC_STAT_INDEX_OUT_OF_RANGE,
} GC_Status;

typedef struct GC_Array {
    void **buffer;
    int cap, len;
} GC_Array;

GC_Array *GC_NewArray(int n, GC_Status *stat);
void GC_ArrayFree(GC_Array *array, GC_Status *stat);
//GC_Status GC_FreeArrayWith();
void GC_ArrayAppend(GC_Array *array, void *item, GC_Status *stat);
void GC_ArrayRemove(GC_Array *array, void **item, GC_Status *stat);
void *GC_ArrayGet(GC_Array *array, int index, GC_Status *stat);

#endif