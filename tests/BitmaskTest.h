#include "test.h"
#include "jiggle.h"

test(Bitmask)
{
     jgBitmask mask;
     JG_BITMASK_CLEAR(mask);
     JG_BITMASK_SET_ON(mask, 5);
     JG_BITMASK_SET_ON(mask, 3);
     assert(JG_BITMASK_GET(mask, 3));
     assert(!JG_BITMASK_GET(mask, 4));
     assert(JG_BITMASK_GET(mask, 5));
}
