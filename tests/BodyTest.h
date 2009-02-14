#include "test.h"

#include "jiggle.h"

test(BodyContains)
{
     jgVector2 pts[] = {jgVector2New(0, 2),
                        jgVector2New(2, 0),
                        jgVector2New(-2, 0)};

     jgBody *body = jgStaticBodyNew(pts, 3, jgVector2Zero(), 0, jgVector2One());
     assert( jgBodyContains(body, jgVector2New(0, 1)));
     assert(!jgBodyContains(body, jgVector2New(5, 5)));
     jgBodyFree(body);
}
