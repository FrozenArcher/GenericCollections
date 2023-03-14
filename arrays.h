#ifndef GC_ARRAYS_H
#define GC_ARRAYS_H

/**
 * @brief 执行状态，初始、OK或者错误。在第一次调用之前一直是初始状态。
 */
typedef enum GStatus {
    GSTAT_INIT,
    GSTAT_OK,
    GSTAT_ERROR,
} GStatus;

/**
 * @brief 执行错误。若状态为OK，则错误为“无错误”状态，否则为错误名。
 */
typedef enum GError {
    GERROR_NO_ERROR,
    GERROR_INDEX_OUT_OF_RANGE,
    GERROR_ITEM_NOT_FOUND,
    GERROR_ITEM_TO_STRING_NOT_FOUND,
    GERROR_UNKNOWN_ERROR,
} GError;

/**
 * @brief 动态泛型数组。
 */
typedef struct GArray {
    void **buffer;
    int cap, len;
    void (*to_string)(void *, char *);
    void (*item_free)(void *);
} GArray;

/**
 * @brief 获取数组的容量。
 * 
 * @param array 目标数组
 * @return array的容量
 */
inline const int GA_Cap(GArray *array) { return array->cap; }

/**
 * @brief 获取数组的长度。
 * 
 * @param array 目标数组
 * @return array的长度
 */
inline const int GA_Len(GArray *array) { return array->len; }

/**
 * @brief 获取运行状态。
 * 
 * @return 运行状态
 */
const GStatus GA_GetStatus();

/**
 * @brief 获取运行错误。
 * 
 * @return 运行错误，若无错误，则为 `GERROR_NO_ERROR`
 */
const GError GA_GetError();

/**
 * @brief 在堆内存创建新数组。
 * 
 * @param n 初始大小，最好是2的n次方
 * @return 创建的数组
 */
GArray *GA_New_Raw(int n, void (*to_string)(void *, char *), void (*item_free)(void *));

/**
 * @brief 按顺序释放全部元素内存，并释放数组内存。
 * 
 * @param array 目标数组
 */
void GA_FreeAll(GArray *array);

/**
 * @brief 按顺序释放全部元素内存，并释放数组内存，使用自定义释放函数。
 * 
 * @param array 目标数组
 * @param mem_free 元素释放函数
 */
void GA_FreeAllWith_Raw(GArray *array, void (*mem_free)(void *));

/**
 * @brief 将新元素item添加到数组末尾。
 * 
 * @param array 目标数组
 * @param item 新元素
 */
void GA_Append(GArray *array, void *item);

/**
 * @brief 将元素item从数组中移除并释放。
 * 
 * @param array 目标数组
 * @param item 待移除元素
 */
void GA_Kill(GArray *array, void *item);

/**
 * @brief 将元素item从数组中移除并释放，使用自定义释放函数。
 * 
 * @param array 目标数组
 * @param item 待移除元素
 * @param mem_free 元素释放函数
 */
void GA_KillWith_Raw(GArray *array, void *item, void (*mem_free)(void *));

/**
 * @brief 获取下标为index的元素。
 * 
 * @param array 目标函数
 * @param index 下标
 * @return 找到的元素。若下标超出范围，则返回空指针。
 */
void *GA_Get(GArray *array, int index);

/**
 * @brief 输出整个数组。
 * 
 * @param array 目标数组
 * @param str 将元素转换为字符串的函数
 * @param endline 是否换行
 * @param info 是否显示容量和长度信息
 */
void GA_Print_Raw(GArray *array, int endline, int info);

/**
 * @brief 查找元素item的下标。
 * 
 * @param array 目标数组
 * @param item 目标元素
 * @return 找到的下标。如果没找到，返回-1。
 */
int GA_FindIndex(GArray *array, void *item);

/**
 * @brief 移除下标为index的元素，但不释放。
 * 
 * @param array 目标数组
 * @param index 元素下标
 * @return 原下标为index，已经被移除的元素
 */
void *GA_RemoveAt(GArray *array, int index);

/**
 * @brief 移除元素item，但不释放。
 * 
 * @param array 目标数组
 * @param item 待移除元素
 */
void GA_Remove(GArray *array, void *item);

/**
 * @brief 使用new_item替换old_item。
 * 
 * @param array 目标数组
 * @param old_item 被替换的元素
 * @param new_item 新元素
 */
void GA_Replace(GArray *array, void *old_item, void *new_item);

/**
 * @brief 使用new_item替换下标为index的旧元素。
 * 
 * @param array 目标数组
 * @param index 被替换的元素的下标
 * @param new_item 新元素
 * @return 原下标为index的已被替换的元素
 */
void *GA_ReplaceAt(GArray *array, int index, void *new_item);

/**
 * @brief 在下标为index处添加元素index，原下标为index及以后的元素全体后移一位。
 * 
 * @param array 目标数组
 * @param index 目标位置下标
 * @param item 新添加的元素
 */
void GA_AddAt(GArray *array, int index, void *item);

/**
 * @brief 交换两个元素的位置。
 * 
 * @param array 目标数组
 * @param item_1 第一个元素
 * @param item_2 第二个元素
 */
void GA_Swap(GArray *array, void *item_1, void *item_2);

/**
 * @brief 交换下标为index_1和index_2的两个元素的位置。
 * 
 * @param array 目标数组
 * @param index_1 第一个元素的下标
 * @param index_2 第二个元素的下标
 */
void GA_SwapAt(GArray *array, int index_1, int index_2);

#define GA_New(n, to_string, item_free)\
    GA_New_Raw(n, (void(*)(void*, char*))to_string, (void(*)(void *))item_free)
#define GA_PrintInfo(array) GA_Print_Raw((array), 0, 1)
#define GA_PrintlnInfo(array) GA_Print_Raw((array), 1, 1)
#define GA_Print(array) GA_Print_Raw((array), 0, 0)
#define GA_Println(array) GA_Print_Raw((array), 1, 0)
#define GA_KillWith(array, item, mem_free) GA_KillWith_Raw((array), (item), (void (*)(void *))(mem_free))
#define GA_FreeAllWith(array, mem_free) GA_FreeAllWith_Raw((array), (void (*)(void *))(mem_free))

#endif