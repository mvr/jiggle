#include <allegro.h>

BITMAP *buffer;

jgVector2 world2screen(jgVector2 in)
{
     return jgVector2Add(jgVector2Multiply(in, 10), jgv(320, 240));
}

jgVector2 screen2world(jgVector2 in)
{
     return jgVector2Divide(jgVector2Subtract(in, jgv(320, 240)), 10);
}

void draw_all_bodies(jgWorld *world)
{
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

volatile int ticks;
void ticker(void)
{
     ++ticks;
} END_OF_FUNCTION(ticker)

float secondsElapsed()
{
     return ticks / 100.0;
}

void flip()
{
     blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
     clear_bitmap(buffer);
}

void setup() 
{
     allegro_init();

     install_keyboard();
     install_mouse();
     show_os_cursor(MOUSE_CURSOR_ARROW);

     set_color_depth(32);
     set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

     LOCK_FUNCTION(ticker);
     LOCK_VARIABLE(ticks);
     install_int_ex(ticker, BPS_TO_TIMER(100));

     buffer = create_bitmap(SCREEN_W, SCREEN_H);
}

