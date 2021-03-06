#ifndef __JG_LIST_H__
#define __JG_LIST_H__

#include <stdbool.h>

typedef struct jgList
{
     void **arr;
     int    length;    // Number of elements
     int    alloc;     // Allocated size
} jgList;

extern jgList  *jgListNew();
extern jgList  *jgListNewFromArray(void **array, int length);
extern jgList  *jgListDuplicate(jgList *list);
extern void     jgListClear(jgList *list);
extern void     jgListFree(jgList *list);

static inline void *jgListGet(jgList *list, int position) { return list->arr[position]; }
extern void     jgListSet(jgList *list, int position, void* data);

extern void     jgListInsert(jgList *list, int position, void *data);
extern void     jgListDelete(jgList *list, int position);

extern bool     jgListContains(jgList *list, void *data);

extern void     jgListAdd(jgList *list, void *data);
extern void     jgListRemove(jgList *list, void *data);

extern void     jgListAppend(jgList *list, jgList *other);

// EWW! These are mega-gross.

#define JG_LIST_FOREACH(list, var)              \
     var = jgListGet(list, 0);                  \
     for(int _i_ = 0; _i_ < list->length; _i_++, var = jgListGet(list, _i_))

// OMGWTFBBQ
#define JG_LIST_FOREACH2(list, var)             \
     var = jgListGet(list, 0);                  \
     for(int _i2_ = 0; _i2_ < list->length; _i2_++, var = jgListGet(list, _i2_))

#define JG_LIST_FOREACH_PAIR(list, var, var2)                           \
     var = jgListGet(list, 0);                                          \
     var2 = jgListGet(list, 1);                                         \
     for(int _i_ = 0; _i_ < list->length; _i_++,                        \
              var = jgListGet(list, _i_),                               \
              var2 = (_i_ == list->length - 1) ? jgListGet(list, 0) : jgListGet(list, _i_ + 1))

#define JG_LIST_FOREACH_TRIPLET(list, prev, current, next)              \
     prev = jgListGet(list, list->length - 1);                          \
     current = jgListGet(list, 0);                                      \
     next = jgListGet(list, 1);                                         \
     for(int _i_ = 0; _i_ < list->length; _i_++,                        \
              prev = (_i_ == 0) ? jgListGet(list, list->length - 1) : jgListGet(list, _i_ - 1), \
              current = jgListGet(list, _i_),                           \
              next = (_i_ == list->length - 1) ? jgListGet(list, 0) : jgListGet(list, _i_ + 1))
          
// Is this even used any more?

#define JG_LIST_FOREACH_COMBO(list, b)                                  \
     b = NULL;                                                          \
     if(_i_ != list->length - 1) b = jgListGet(list, _i_ + 1);          \
     for(int _i2_ = _i_ + 1; _i2_ < list->length; _i2_++, b = (_i2_ == list->length) ? NULL : jgListGet(list, _i2_))

// Can pair with themselves.

/* #define JG_LIST_FOREACH_COMBO(b)                                         \ */

#endif
