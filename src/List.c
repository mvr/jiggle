#include <stdlib.h>

#include "List.h"

static jgListNode *jgListNthNode(jgList *list, int position)
{
     jgListNode *current = list->head;
     while(position-- != 0) {
          if(current->next)
               current = current->next;
          else
               return NULL;
     }
     return current;
}

jgList *jgListNew()
{
     jgList *ret = malloc(sizeof(jgList));
     ret->head = NULL;
     ret->length = 0;
     return ret;
}

jgList *jgListNewFromArray(void **array, int length)
{
     jgList *list = jgListNew();
     list->length = length;

     for(int i = length - 1; i >= 0; i--)
     {
          jgListAdd(list, array[i]);
     }
     return list;
}

void jgListClear(jgList *list)
{
     jgListNode *current = list->head;
     jgListNode *tmp;
     while(current)
     {
          tmp = current->next;
          free(current);
          current = tmp;
     }
     list->head = NULL;
     list->length = 0;
}

void jgListFree(jgList *list)
{
     jgListClear(list);
     free(list);
}

void *jgListGet(jgList *list, int position)
{
     jgListNode *node = jgListNthNode(list, position);
     if(node)
          return node->data;
     else
          return NULL;
}

void jgListSet(jgList *list, int position, void* data)
{
     jgListNode *node = jgListNthNode(list, position);
     if(node)
          node->data = data;
}

void jgListInsert(jgList *list, int position, void *data)
{
     jgListNode *newnode = malloc(sizeof(jgListNode));
     newnode->data = data;

     if(position == 0) {
          jgListNode *oldhead = list->head;
          list->head = newnode;
          list->head->next = oldhead;
     } else {
          jgListNode *previous = jgListNthNode(list, position - 1);
          newnode->next = previous->next;
          previous->next = newnode;
     }
     list->length++;
}

void jgListDelete(jgList *list, int position)
{
     if(position == 0) {
          jgListNode *newhead = list->head->next;
          free(list->head);
          list->head = newhead;
     } else {
          jgListNode *previous = jgListNthNode(list, position - 1);
          jgListNode *current = previous->next;
          jgListNode *next = current->next;

          free(current);
          previous->next = next;
     }

     list->length--;
}

bool jgListContains(jgList *list, void *data)
{
     jgListNode *current = list->head;
     while(current)
     {
          if(current->data == data)
               return true;
          current = current->next;
     }
     return false;
}

bool jgListHasLoop(jgList *list)
{
     // Taken from http://ostermiller.org/find_loop_singly_linked_list.html

     jgListNode *slowNode;
     jgListNode *fastNode1;
     jgListNode *fastNode2;
     slowNode = fastNode1 = fastNode2 = list->head;

     while (slowNode && (fastNode1 = fastNode2->next) && (fastNode2 = fastNode1->next)){
          if (slowNode == fastNode1 || slowNode == fastNode2) return true;
          slowNode = slowNode->next;
     }
     return false;
}

void jgListAdd(jgList *list, void *data)
{
     jgListInsert(list, 0, data);
}

void jgListRemove(jgList *list, void *data)
{
     jgListNode *current = list->head;
     int position = 0;
     while(current)
     {
          if(current->data == data)
          {
               jgListDelete(list, position);
               return;
          }
          position++;
          current = current->next;
     }
}
