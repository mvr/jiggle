/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_AREA_H__
#define __JG_AREA_H__

#include "AABB.h"
#include "List.h"
#include "Particle.h"
#include "Vector2.h"

typedef struct jgArea
{
     jgList      *particles;

     float        elasticity;
     float        friction;

     jgAABB       aabb;

     jgBitmask    bitmaskX;
     jgBitmask    bitmaskY;
} jgArea;

extern jgArea   *jgAreaNew(jgParticle **particles, int numOfParticles);
extern void      jgAreaFree(jgArea *area);

extern void      jgAreaUpdateAABB(jgArea *area, float elapsed);
extern void      jgAreaUpdateBitmask(jgArea *area, jgWorld *world);

extern bool      jgAreaContains(jgArea *area, jgVector2 point);
extern jgVector2 jgAreaClosestOnEdge(jgArea *area, jgVector2 pt, jgVector2 normal,
                                     jgParticle **Aout, jgParticle **Bout, 
                                     float *Dout, jgVector2 *normout);

extern float     jgAreaArea(jgArea *area); // Heh.
extern bool      jgAreaIsInsideOut(jgArea *area);

#endif
