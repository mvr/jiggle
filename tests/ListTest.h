#include "test.h"
#include "jiggle.h"

test(ListNewFromArray)
{
     void *array[] = {(void *)1,
                      (void *)2,
                      (void *)3};
     jgList *foo = jgListNewFromArray(array, 3);
     assert(jgListGet(foo, 0) == (void *)1);
     assert(jgListGet(foo, 1) == (void *)2);
     assert(jgListGet(foo, 2) == (void *)3);
     jgListFree(foo);
}

test(ListInsert)
{
     jgList *foo = jgListNew();
     assert(foo->length == 0);
     jgListInsert(foo, 0, (void *)1);
     jgListInsert(foo, 1, (void *)2);
     jgListInsert(foo, 1, (void *)3);
     assert(foo->length == 3);
     assert(jgListGet(foo, 0) == (void *)1);
     assert(jgListGet(foo, 1) == (void *)3);
     assert(jgListGet(foo, 2) == (void *)2);
     jgListFree(foo);
}

test(ListSet)
{
     jgList *foo = jgListNew();
     jgListInsert(foo, 0, (void *)1);
     jgListSet(foo, 0, (void *)2);
     assert(jgListGet(foo, 0) == (void *)2);
     jgListFree(foo);
}

test(ListRemove)
{
     jgList *foo = jgListNew();
     jgListInsert(foo, 0, (void *)1);
     jgListInsert(foo, 1, (void *)2);
     jgListInsert(foo, 2, (void *)3);
     jgListRemove(foo, (void *) 2);
     assert(foo->length == 2);
     assert(jgListGet(foo, 0) == (void *)1);
     assert(jgListGet(foo, 1) == (void *)3);
     jgListFree(foo);
}

// Pretty lame, but oh well.

test(ListForeach)
{

     jgList *foo = jgListNew();
     jgListInsert(foo, 0, (void *)1);
     jgListInsert(foo, 1, (void *)2);
     jgListInsert(foo, 2, (void *)3);
     jgListInsert(foo, 3, (void *)4);

     int i = 1;
     void *j;
     void *k;
     JG_LIST_FOREACH(foo, j)
     {
          assert((int)j == i);
          i++;
     }

     i = 0;
     JG_LIST_FOREACH(foo, j)
     {
          JG_LIST_FOREACH_COMBO(foo, k)
          {
               i++;
          }
     }

     assert(i == 6);

     JG_LIST_FOREACH_PAIR(foo, j, k)
     {
          if((int)j == 4) 
               assert((int)k == 1);
          else
               assert((int)k == (int)j + 1);
     }


     jgListFree(foo);

     foo = jgListNew();
     JG_LIST_FOREACH(foo, j)
     {
          assert(false); // List is empty, shouldn't get here.
     }

     jgListFree(foo);
}
