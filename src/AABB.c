#include "jiggle.h"

jgAABB jgAABBNewFromVector2(jgVector2 min, jgVector2 max)
{
     return (jgAABB){min, max, true};
}

jgAABB jgAABBNewFromFloat(float x1, float y1, float x2, float y2)
{
     return jgAABBNewFromVector2(jgVector2New(x1, y1), jgVector2New(x2, y2));
}

jgAABB jgAABBNull()
{
     return (jgAABB){jgVector2New(0.0, 0.0), jgVector2New(0.0, 0.0), false};
}

jgAABB jgAABBExpandToInclude(jgAABB a, jgVector2 pt)
{
     if (a.isValid)
     {
          jgAABB ret = a;

          if (pt.x < a.min.x)
               ret.min.x = pt.x;
          else if (pt.x > a.max.x)
               ret.max.x = pt.x;

          if (pt.y < a.min.y)
               ret.min.y = pt.y;
          else if (pt.y > a.max.y)
               ret.max.y = pt.y;

          return ret;
     }
     else
     {
          return (jgAABB){pt, pt, true};
     }
}

bool jgAABBContains(jgAABB a, jgVector2 pt)
{
     if (!a.isValid)
          return false;

     return ((pt.x >= a.min.x) &&
             (pt.x <= a.max.x) &&
             (pt.y >= a.min.y) &&
             (pt.y <= a.max.y));
}

bool jgAABBIntersects(jgAABB a, jgAABB b)
{
     return (a.min.x <= b.max.x) && (a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y) && (a.max.y >= b.min.y);
}

jgVector2 jgAABBCenter(jgAABB a)
{
     return (jgVector2){(a.min.x + a.max.x) / 2,
                        (a.min.y + a.max.y) / 2};
}
