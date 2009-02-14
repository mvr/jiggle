#include "jiggle.h"

int main()
{
     // Jiggle setup

     jgWorld *world = jgWorldNew(jgv(-20, -20),
                                 jgv(20, 20));

     world->penetrationThreshold = 1;
     world->gravity = jgv(0, 10);

     jgVector2 groundShape[] = {jgv(-10, -2),
                                jgv(-10,  2),
                                jgv( 10,  2),
                                jgv( 10, -2)};
     jgBody *ground = jgStaticBodyNew(groundShape, 4, jgv(0, 10), 0, jgVector2One());
     jgWorldAddBody(world, ground);


     jgVector2 shape[] = {jgv(-1, -1),
                          jgv(-1,  1),
                          jgv( 1,  1),
                          jgv( 1, -1)};
     jgBody *box = jgSpringBodyNew(shape, 4, 1, jgv(0, 0), 0, jgVector2One(), false);
     box->isShapeMatching = true;
     box->shapeSpringK = 150;
     jgSpringBodyAddSpring(box, 0, 2, 400, 12);
     jgSpringBodyAddSpring(box, 1, 3, 400, 12);
     jgWorldAddBody(world, box);

     for(int j = 0; j < 10000; j++)
          for(int i = 0; i < 10; i++)
               jgWorldUpdate(world, 1 / 600.0f);

     jgWorldFreeBodies(world);
     jgWorldFree(world);

     return 0;
}
