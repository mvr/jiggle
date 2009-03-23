/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_LIST_H__
#define __JG_LIST_H__

#include <stdbool.h>

typedef struct jgListNode
{
     void *data;
     struct jgListNode *next;
} jgListNode;

typedef struct jgList
{
     jgListNode *head;
     int length;
} jgList;

extern jgList  *jgListNew();
extern jgList  *jgListNewFromArray(void **array, int length);
extern void     jgListClear(jgList *list);
extern void     jgListFree(jgList *list);

extern void    *jgListGet(jgList *list, int position);
extern void     jgListSet(jgList *list, int position, void* data);

extern void     jgListInsert(jgList *list, int position, void *data);
extern void     jgListDelete(jgList *list, int position);

extern bool     jgListContains(jgList *list, void *data);
extern bool     jgListHasLoop(jgList *list);

extern void     jgListAdd(jgList *list, void *data);
extern void     jgListRemove(jgList *list, void *data);

// EWW! These are mega-gross.

#define JG_LIST_FOREACH(list, var)                              \
     var = NULL;                                                \
     if(list->length > 0) var = list->head->data;               \
     for(jgListNode *_c_ = list->head;                          \
         _c_ != NULL;                                           \
         _c_ = _c_->next, var = (_c_ == NULL) ? NULL : _c_->data)

#define JG_LIST_FOREACH_PAIR(list, var, var2)                           \
     var  = NULL;                                                       \
     var2 = NULL;                                                       \
     if(list->length > 1) var = list->head->data, var2 = list->head->next->data; \
     for(jgListNode *_c_ = list->head;                                  \
         _c_ != NULL;                                                   \
         _c_ = _c_->next,                                               \
              var = (_c_ == NULL) ? NULL : _c_->data,                   \
              var2 = (_c_ == NULL) ? NULL :                             \
              ((_c_->next == NULL) ? jgListGet(list, 0)  : _c_->next->data) \
          )

#define JG_LIST_FOREACH_COMBO(b)                                        \
     b = NULL;                                                          \
     if(_c_->next != NULL) b = _c_->next->data;                         \
     for(jgListNode *_c2_ = _c_->next;                                  \
         _c2_ != NULL;                                                  \
         _c2_ = _c2_->next, b = (_c2_ == NULL) ? NULL : _c2_->data)


// Can pair with themselves.

/* #define JG_LIST_FOREACH_COMBO(b)                                         \ */
/*      b = NULL;                                                          \ */
/*      if(_c_ != NULL) b = _c_->data;                                     \ */
/*      for(jgListNode *_c2_ = _c_;                                        \ */
/*          _c2_ != NULL;                                                  \ */
/*          _c2_ = _c2_->next, b = (_c2_ == NULL) ? NULL : _c2_->data) */

#endif
