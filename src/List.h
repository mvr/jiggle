/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

typedef struct jgList
{
     void **arr;
     int    length;    // Number of elements
     int    alloc;     // Allocated size
} jgList;

extern jgList  *jgListNew();
extern void     jgListClear(jgList *list);
extern void     jgListFree(jgList *list);

extern void    *jgListGet(jgList *list, int position);
extern void     jgListSet(jgList *list, int position, void* data);

extern void     jgListInsert(jgList *list, int position, void *data);
extern void     jgListDelete(jgList *list, int position);

extern bool     jgListContains(jgList *list, void *data);

extern void     jgListAdd(jgList *list, void *data);
extern void     jgListRemove(jgList *list, void *data);

// These are clunky but they'll do for now.

#define JG_LIST_FOREACH(list, var)              \
     var = jgListGet(list, 0);                  \
     for(int _i_ = 0; _i_ < list->length; _i_++, var = jgListGet(list, _i_))

#define JG_LIST_FOREACH_PAIR(list, var, var2)                           \
     var = jgListGet(list, 0);                                          \
     var2 = jgListGet(list, 1);                                         \
     for(int _i_ = 0; _i_ < list->length; _i_++,                        \
              var = jgListGet(list, _i_),                               \
              var2 = (_i_ == list->length - 1) ? jgListGet(list, 0) : jgListGet(list, _i_ + 1))

#define JG_LIST_FOREACH_TRIPLET(list, prev, current, next)              \
     prev = jgListGet(list, list->length - 1);                          \
     var2 = jgListGet(list, 0);                                         \
     var = jgListGet(list, 1);                                          \
     for(int _i_ = 0; _i_ < list->length; _i_++,                        \
              prev = (_i_ == 0) ? jgListGet(list, list->length - 1) : jgListGet(list, _i_ - 1) \
              current = jgListGet(list, _i_),                           \
              next = (_i_ == list->length - 1) ? jgListGet(list, 0) : jgListGet(list, _i_ + 1))
          

#define JG_LIST_FOREACH_COMBO(list, b)                                  \
     b = NULL;                                                          \
     if(_i_ != list->length - 1) b = jgListGet(list, _i_ + 1);          \
     for(int _i2_ = _i_ + 1; _i2_ < list->length; _i2_++, b = (_i2_ == list->length - 1) ? NULL : jgListGet(list, _i2_))

// Can pair with themselves.

/* #define JG_LIST_FOREACH_COMBO(b)                                         \ */

#endif
