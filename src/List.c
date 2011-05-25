#include <stdlib.h>
#include <string.h> // For memcpy, wtf?

#include "jiggle.h"

jgList *jgListNew()
{
     jgList *ret = malloc(sizeof(jgList));
     ret->length = 0;
     ret->alloc = 4;
     ret->arr = calloc(ret->alloc, sizeof(void*));
     return ret;
}

jgList *jgListNewFromArray(void **array, int length)
{
     jgList *list = malloc(sizeof(jgList));
     list->length = length;
     list->alloc = length;
     list->arr = malloc(length * sizeof(void *));
     memcpy(list->arr, array, length * sizeof(void *));

     return list;
}

jgList *jgListDuplicate(jgList *list)
{
     return jgListNewFromArray(list->arr, list->length);
}

void jgListClear(jgList *list)
{
     list->length = 0;
}

void jgListFree(jgList *list)
{
     free(list->arr);
     free(list);
}

void jgListSet(jgList *list, int position, void* data)
{
     if(position < list->length)
          list->arr[position] = data;
}

static void jgListExpand(jgList *list)
{
     if(list->length == list->alloc){
          list->alloc *= 2;
          list->arr = realloc(list->arr, list->alloc * sizeof(void *));
     }
     list->length++;     
}

static void jgListExpandTo(jgList *list, int size)
{
     list->length = size;

     if(size > list->alloc){
          list->alloc = size;
          list->arr = realloc(list->arr, list->alloc * sizeof(void *));
     }
}

void jgListInsert(jgList *list, int position, void *data)
{
     if(list->length == 0)
     {
          jgListAdd(list, data);
          return;
     }
     
     jgListExpand(list);
     memcpy(list->arr + position + 1, list->arr + position, (list->length - position) * sizeof(void *));
     list->arr[position] = data;
}

void jgListDelete(jgList *list, int position)
{
     memcpy(list->arr + position, list->arr + position + 1, (list->length - position - 1) * sizeof(void *));
     list->length--;
}

bool jgListContains(jgList *list, void *data)
{
     void *currentData;
     JG_LIST_FOREACH(list, currentData)
     {
          if(currentData == data)
               return true;
     }
     return false;
}

void jgListAdd(jgList *list, void *data)
{
     jgListExpand(list);
     list->arr[list->length - 1] = data;
}

void jgListRemove(jgList *list, void *data)
{
     void *currentData;
     JG_LIST_FOREACH(list, currentData)
     {
          // Hack!
          if(currentData == data)
               jgListDelete(list, _i_);
     }
}

void jgListAppend(jgList *list, jgList *other)
{
     int originalLength = list->length;
     jgListExpandTo(list, list->length + other->length);
     memcpy(list->arr + originalLength, other->arr, other->length * sizeof(void *));    
}
