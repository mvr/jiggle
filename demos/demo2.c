#include <stdio.h>

#include <allegro.h>

#include "jiggle.h"
#include "common.h"

int main()
{
     setup();

     // Jiggle setup

     jgWorld *world = jgWorldNew(jgv(-20, -20),
                                 jgv(20, 20),
                                 100,
                                 secondsElapsed());

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
     jgSpringBody *springBox = (jgSpringBody *)box;
     box->isShapeMatching = true;
     box->shapeSpringK = 150;
     jgSpringBodyAddSpring(box, 0, 2, 400, 12);
     jgSpringBodyAddSpring(box, 1, 3, 400, 12);
     jgWorldAddBody(world, box);

     bool w_was_pressed = false;
     bool p_was_pressed = false;

     while (!key[KEY_ESC]) {
          flip();

          if(key[KEY_W] && !w_was_pressed)
          {
               jgSpring *currentSpring;
               JG_LIST_FOREACH(springBox->springs, currentSpring)
               {
                    currentSpring->springD = 8;
               }
          }
          if(!key[KEY_W] && w_was_pressed)
          {
               jgSpring *currentSpring;
               JG_LIST_FOREACH(springBox->springs, currentSpring)
               {
                    currentSpring->springD = 2;
               }
          }

          if(key[KEY_A])
          {
               box->derivedPosition.x -= 1;
          }

          if(key[KEY_D])
          {
               box->derivedPosition.x += 1;
          }

          if(key[KEY_P] && !p_was_pressed)
          {
               jgBody *platform = jgStaticBodyNew(groundShape, 4, screen2world(jgv(mouse_x, mouse_y)), 0, jgv(0.3, 0.3));
               jgWorldAddBody(world, platform);
          }

          w_was_pressed = key[KEY_W];
          p_was_pressed = key[KEY_P];

          box->derivedAngle = 0;

          jgWorldUpdate(world, secondsElapsed());
          
          draw_all_bodies(world);
     }
     jgWorldFreeBodies(world);
     jgWorldFree(world);

     return 0;
}
END_OF_MAIN()
