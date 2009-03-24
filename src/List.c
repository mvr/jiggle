#include <stdlib.h>

#include "List.h"

jgList *jgListNew()
{
     jgList *ret = malloc(sizeof(jgList));
     ret->length = 0;
     ret->alloc = 4;
     ret->arr = calloc(ret->alloc, sizeof(void*));
     return ret;
}

void jgListClear(jgList *list)
{
     list->length = 0;
}

void jgListFree(jgList *list)
{
     free(list);
}

void *jgListGet(jgList *list, int position)
{
     if(position < list->length)
          return list->arr[position];
     else
          return NULL;
}

void jgListSet(jgList *list, int position, void* data)
{
     if(position < list->length)
          list->arr[position] = data;
}

void jgListInsert(jgList *list, int position, void *data)
{
     // TODO
}

void jgListDelete(jgList *list, int position)
{
     // TODO: reorders array
     int last = list->length - 1;
     list->arr[position] = list->arr[last];
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
     if(list->length == list->alloc){
          list->alloc *= 2;
          list->arr = realloc(list->arr, list->alloc * sizeof(void *));
     }

     list->arr[list->length] = data;
     list->length++;
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
