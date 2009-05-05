/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_AABB_H__
#define __JG_AABB_H__

#include <stdbool.h>

#include "jiggle.h"

typedef struct jgAABB
{
     jgVector2 min, max;
     bool isValid;
} jgAABB;

extern jgAABB   jgAABBNewFromVector2(jgVector2 min, jgVector2 max);
extern jgAABB   jgAABBNewFromFloat(float x1, float y1, float x2, float y2);
extern jgAABB   jgAABBNull();
extern jgAABB   jgAABBExpandToInclude(jgAABB a, jgVector2 pt);

extern bool     jgAABBContains(jgAABB a, jgVector2 pt);
extern bool     jgAABBIntersects(jgAABB a, jgAABB b);

extern jgVector2 jgAABBCenter(jgAABB a);

#endif
