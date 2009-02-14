#include <stdio.h>

#include <allegro.h>

#include "jiggle.h"

BITMAP *buffer;

void flip()
{
     blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
     clear_bitmap(buffer);
}

jgVector2 world2screen(jgVector2 in)
{
     return jgVector2Add(jgVector2Multiply(in, 10), jgv(320, 240));
}

jgVector2 screen2world(jgVector2 in)
{
     return jgVector2Divide(jgVector2Subtract(in, jgv(320, 240)), 10);
}

int main()
{
     // Allegro setup
     allegro_init();

     install_keyboard();
     install_mouse();
     show_os_cursor(MOUSE_CURSOR_ARROW);

     set_color_depth(32);
     set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

     buffer = create_bitmap(SCREEN_W, SCREEN_H);

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
          vsync();

	  for(long i=0;i<1000000;i++);

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

          for(int i = 0; i < 10; i++)
               jgWorldUpdate(world, 1 / 600.0f);

          jgBody *currentBody;
          JG_LIST_FOREACH(world->bodies, currentBody)
          {
               for(int i = 0; i < currentBody->numOfPoints; i++)
               {
                    jgVector2 current = world2screen(currentBody->pointMasses[i].position);
                    jgVector2 next = world2screen(currentBody->pointMasses[i == currentBody->numOfPoints - 1 ? 0 : i + 1].position);
                    line(buffer,
                         current.x,
                         current.y,
                         next.x,
                         next.y,
                         makecol32(255, 255, 255));
               }
          }
     }
     jgWorldFreeBodies(world);
     jgWorldFree(world);

     return 0;
}
END_OF_MAIN()
