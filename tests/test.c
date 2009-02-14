#include "test.h"

#include "tests/AABBTest.h"
#include "tests/BitmaskTest.h"
#include "tests/BodyTest.h"
#include "tests/ListTest.h"
#include "tests/test.h"
#include "tests/Vector2Test.h"
int main()
{
run_test(AABB);
run_test(Bitmask);
run_test(BodyContains);
run_test(ListInsert);
run_test(ListSet);
run_test(ListRemove);
run_test(ListForeach);
run_test(Vector2Math);
run_test(Vector2Transformations);
run_test(Vector2Intersections);
run_test(Vector2Misc);
}
