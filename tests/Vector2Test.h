#include "jiggle.h"
#include "test.h"

#include <math.h>

test(Vector2Math)
{
     jgVector2 foo, bar, baz;
     foo = bar = jgVector2New(1.0, 1.0);
     baz = jgVector2New(2.0, 2.0);

     assert(jgVector2Equal(jgVector2Add(foo, bar), baz));
     assert(jgVector2Equal(jgVector2Subtract(baz, bar), foo));
     assert(jgVector2Equal(jgVector2Multiply(foo, 2), baz));
     assert(jgVector2Dot(foo, baz) == 4.0);
}

test(Vector2Transformations)
{
     jgVector2 foo, bar, baz;
     foo = jgVector2New(1.0, 1.0);
     bar = jgVector2New(-1.0, 1.0);

     assert(jgVector2Equal(jgVector2Perpendicular(foo), bar));

     baz = jgVector2Rotate(foo, 3.1415926535 * 2);
     assert_close(foo.x, baz.x, 0.001);
     assert_close(foo.y, baz.y, 0.001);

     foo = jgVector2New(1.0, 0.0);
     baz = jgVector2Rotate(foo, 0.785398163);
     assert_close(1.0/sqrt(2.0), baz.x, 0.001);
     assert_close(1.0/sqrt(2.0), baz.y, 0.001);
}

test(Vector2Intersections)
{
     jgVector2 a1, a2, b1, b2;
     a1 = jgVector2New(1, 1);
     a2 = jgVector2New(2, 2);
     b1 = jgVector2New(1, 3);
     b2 = jgVector2New(2, 0);
     assert(jgVector2Intersect(a1, a2, b1, b2));

     a1 = jgVector2New(1, 1);
     a2 = jgVector2New(2, 2);
     b1 = jgVector2New(0, 0);
     b2 = jgVector2New(-5, -2);
     assert(!jgVector2Intersect(a1, a2, b1, b2));
}

/* test(Vector2Angle) */
/* { */
/*      jgVector2 a = {-1, 0}; */
/*      jgVector2 b = {1, 0}; */
/*      printf("%f\n", jgVector2AngleBetween(a, b)); */
/* } */

test(Vector2Misc)
{
     jgVector2 foo, bar;
     foo = jgVector2New(2.0, 2.0);
     assert_close(jgVector2Length(foo), 2.0*sqrt(2.0), 0.001);

     foo = jgVector2New(2.0, 2.0);
     bar = jgVector2New(2.0, 1.0);
     assert(jgVector2CW(foo, bar));
}
