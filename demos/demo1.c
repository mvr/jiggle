#include <stdio.h>

#include <allegro.h>

#include "jiggle.h"
#include "common.h"

int main()
{
     setup();

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

     bool a_was_pressed = false;
     bool b_was_pressed = false;
     bool c_was_pressed = false;

     while (!key[KEY_ESC]) {
          flip();

          if(key[KEY_A] && !a_was_pressed)
          {
               jgVector2 boxShape[] = {jgv(-1, -1),
                                       jgv(-1,  1),
                                       jgv( 1,  1),
                                       jgv( 1, -1)};

               jgBody *box = jgSpringBodyNew(boxShape,
                                             4,
                                             1,
                                             screen2world(jgv(mouse_x, mouse_y)),
                                             0,
                                             jgVector2One(),
                                             false);
	       box->edgeSpringDamp = 0.5;

               jgSpringBodyAddSpring(box, 0, 2, 400, 12);
               jgSpringBodyAddSpring(box, 1, 3, 400, 12);

               jgWorldAddBody(world, box);
          }

          if(key[KEY_B] && !b_was_pressed)
          {
               jgVector2 iShape[] = {jgv(-1.5f, 2.0f),
                                     jgv(-0.5f, 2.0f),
                                     jgv(0.5f, 2.0f),
                                     jgv(1.5f, 2.0f),
                                     jgv(1.5f, 1.0f),
                                     jgv(0.5f, 1.0f),
                                     jgv(0.5f, -1.0f),
                                     jgv(1.5f, -1.0f),
                                     jgv(1.5f, -2.0f),
                                     jgv(0.5f, -2.0f),
                                     jgv(-0.5f, -2.0f),
                                     jgv(-1.5f, -2.0f),
                                     jgv(-1.5f, -1.0f),
                                     jgv(-0.5f, -1.0f),
                                     jgv(-0.5f, 1.0f),
                                     jgv(-1.5f, 1.0f)};

               jgBody *i = jgSpringBodyNew(iShape,
                                           16,
                                           0.5,
                                           screen2world(jgv(mouse_x, mouse_y)),
                                           (rand() % 6) / 6.0,
                                           jgVector2One(),
                                           false);

               i->isShapeMatching = true;

               jgSpringBodyAddSpring(i, 0, 14,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 1, 14,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 1, 15,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 1, 5,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 2, 14,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 2, 5,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 1, 5,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 14, 5,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 2, 4,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 3, 5,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 14, 6,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 5, 13,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 13, 6,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 12, 10, 400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 13, 11, 400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 13, 10, 400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 13, 9,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 6, 10,  400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 6, 9,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 6, 8,   400.0f, 10.0f);
               jgSpringBodyAddSpring(i, 7, 9,   400.0f, 10.0f);

               jgWorldAddBody(world, i);
          }

          if(key[KEY_C] && !c_was_pressed)
          {
               jgVector2 *ballShape = jgVector2MakeCircle(1, 20);

               jgBody *ball = jgPressureBodyNew(ballShape,
                                                20,
                                                1,
                                                screen2world(jgv(mouse_x, mouse_y)),
                                                (rand() % 6) / 6.0,
                                                jgVector2One(),
                                                false);
               jgWorldAddBody(world, ball);

               free(ballShape);
          }

          a_was_pressed = key[KEY_A];
          b_was_pressed = key[KEY_B];
          c_was_pressed = key[KEY_C];

          for(int i = 0; i < 10; i++)
               jgWorldUpdate(world, 1 / 600.0f);

          draw_all_bodies(world);
     }
     jgWorldFreeBodies(world);
     jgWorldFree(world);

     return 0;
}
END_OF_MAIN()
