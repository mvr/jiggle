#include <stdlib.h>
#include <math.h>

#include "jiggle.h"

bool jgVector2Intersect(jgVector2 a1, jgVector2 a2, jgVector2 b1, jgVector2 b2)
{
     // Taken from http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
     // Because I'm too lazy to figure it out myself.
     float denom = ((b2.y - b1.y) * (a2.x - a1.x)) - ((b2.x - b1.x) * (a2.y - a1.y));

     if(denom == 0)
          return false;

     float num1 = ((b2.x - b1.x) * (a1.y - b1.y)) - ((b2.y - b1.y) * (a1.x - b1.x));
     float ua = num1 / denom;

     if(ua <= 0 || ua >= 1)
          return false;

     float num2 = ((a2.x - a1.x) * (a1.y - b1.y)) - ((a2.y - a1.y) * (a1.x - b1.x));
     float ub = num2 / denom;

     if(ub <= 0 || ub >= 1)
          return false;

     return true;
}

jgVector2 jgVector2ClosestPointOnLine(jgVector2 a1, jgVector2 a2, jgVector2 pt)
{
     jgVector2 toPt = jgVector2Subtract(pt, a1);
     jgVector2 e = jgVector2Subtract(a2, a1);
     float length = jgVector2Length(e);
     e = jgVector2Divide(e, length);

     float x = jgVector2Dot(toPt, e);

     if(x < 0)
          return a1;

     if(x > length)
          return a2;

     return jgVector2Add(a1, jgVector2Multiply(e, x));
}

float jgVector2PositionAlong(jgVector2 a1, jgVector2 a2, jgVector2 pt)
{
     if(jgVector2Equal(pt, a1))
          return 0;

     if(jgVector2Equal(pt, a2))
          return 1;

     if(a2.x - a1.x != 0)
          return (pt.x - a1.x) / (a2.x - a1.x);
     else
          return (pt.y - a1.y) / (a2.y - a1.y);
}
