/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#include <stdio.h>

#define assert(test)                                                    \
     do {                                                               \
          if (test) {                                                   \
               printf(".");                                             \
          } else {                                                      \
               printf("\nassert(%s) failed\n", #test);                  \
               printf("Line %u in %s\n", __LINE__, __FILE__);           \
          }                                                             \
     } while (0)

#define assert_close(a, b, close)                                       \
     assert(((a)-(b)) > 0 ? (((a)-(b)) < close) : (((a)-(b)) > -close))

#define test(test_name) void test_name()

#define run_test(test_name)                                             \
     do {                                                               \
          printf("Running test %s\n", #test_name);                      \
          test_name();                                                  \
          printf("\n");                                                 \
     } while (0)
