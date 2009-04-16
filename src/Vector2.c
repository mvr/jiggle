#include <stdlib.h>
#include <math.h>

#include "Vector2.h"

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

// Arrays of Vector2s

void jgVector2Recenter(jgVector2 points[], int numOfPoints)
{
     jgVector2 total = jgVector2Zero();

     for(int i = 0; i < numOfPoints; i++)
          total = jgVector2Add(total, points[i]);

     jgVector2 average = jgVector2Divide(total, numOfPoints);

     for(int i = 0; i < numOfPoints; i++)
          points[i] = jgVector2Subtract(points[i], average);
}

jgVector2 *jgVector2Transform(jgVector2 points[],
                              int       numOfPoints,
                              jgVector2 position,
                              float     rotation,
                              jgVector2 scale)
{
     jgVector2 *dup = calloc(numOfPoints, sizeof(jgVector2));

     jgVector2 v;
     for(int i = 0; i < numOfPoints; i++)
     {
          v = jgVector2Scale(points[i], scale);
          v = jgVector2Rotate(v, rotation);
          v = jgVector2Add(v, position);
          dup[i] = v;
     }

     return dup;
}
