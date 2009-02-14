#include "test.h"
#include "jiggle.h"

test(AABB)
{
     jgAABB foo = jgAABBNewFromFloat(0.0, 0.0, 1.0, 1.0);
     assert(foo.isValid);
     foo = jgAABBExpandToInclude(foo, jgVector2New(2.0, 3.0));
     assert(foo.max.x == 2.0);
     assert(foo.max.y == 3.0);
     assert(jgAABBContains(foo, jgVector2New(2.0, 2.0)));

     jgAABB bar = jgAABBNewFromVector2(jgVector2New(-1.0, -1.0),
                                       jgVector2New(1.0, 1.0));
     assert(jgAABBIntersects(foo, bar));

     assert(!jgAABBNull().isValid);
}
