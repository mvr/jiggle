#include <stdlib.h>

#include "Area.h"
#include "Particle.h"
#include "Spring.h"
#include "AABB.h"
#include "World.h"

jgArea *jgAreaAlloc()
{
     return malloc(sizeof(jgArea));
}

jgArea *jgAreaInit(jgArea *area, jgParticle **particles, int numOfParticles)
{
     area->particles = jgListNewFromArray((void **)particles, numOfParticles);

     area->friction  = 0.8;
     area->elasticity = 0.3;

     jgAreaUpdateAABB(area, 0.0);

     return area;
}

jgArea *jgAreaNew(jgParticle **particles, int numOfParticles)
{
     return jgAreaInit(jgAreaAlloc(), particles, numOfParticles);
}

void jgAreaFree(jgArea *area)
{
     jgListFree(area->particles);
     free(area);
}

void jgAreaUpdateAABB(jgArea *area, float elapsed)
{
     area->aabb = jgAABBNull();

     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgVector2 p = currentParticle->position;
          area->aabb = jgAABBExpandToInclude(area->aabb, p);

          p = jgVector2Add(p, jgVector2Multiply(currentParticle->velocity, elapsed));
          area->aabb = jgAABBExpandToInclude(area->aabb, p);
     }
}

bool jgAreaContains(jgArea *area, jgVector2 point)
{
     if(!jgAABBContains(area->aabb, point))
          return false;
     
     // Draw a line from the point to a point known to be outside the area.
     // Count the number of lines in the polygon it intersects. If that
     // number is odd, we are inside.  If it's even, we are outside.

     // out is guarenteed to be outside the shape.
     jgVector2 out = jgVector2Add(area->aabb.max, jgv(0, 1));

     int inside = false;
     jgParticle *currentParticle;
     jgParticle *nextParticle;
     JG_LIST_FOREACH_PAIR(area->particles, currentParticle, nextParticle)
     {
          jgVector2 start = currentParticle->position;
          jgVector2 end = nextParticle->position;

          if(jgVector2Intersect(point, out, start, end))
               inside = !inside;
     }

     return inside;
}

// AAH! Ptr to ptr!
jgVector2 jgAreaClosestOnEdge(jgArea *area, jgVector2 pt, jgVector2 normal,
                              jgParticle **Aout, jgParticle **Bout, float *Dout, jgVector2 *normout)
{
     float       sameDist,     awayDist;
     jgParticle *sameA,       *awayA;
     jgParticle *sameB,       *awayB;
     jgVector2   sameClosest,  awayClosest;

     sameDist = awayDist = INFINITY;
     sameA = sameB = awayA = awayB = 0;
     sameClosest = awayClosest = jgVector2Zero();

     bool awayFound = false;

     jgParticle *currentParticle;
     jgParticle *nextParticle;
     JG_LIST_FOREACH_PAIR(area->particles, currentParticle, nextParticle)
     {
          jgVector2 hitPt = jgVector2ClosestPointOnLine(currentParticle->position,
                                                        nextParticle->position,
                                                        pt);
          float dist = jgVector2DistanceBetween(pt, hitPt);

          float dot = jgVector2Dot(normal, jgVector2Normal(currentParticle->position, 
                                                           nextParticle->position));
          if(dot <= 0)
          {
               if(dist < awayDist)
               {
                    awayDist = dist;
                    awayA = currentParticle;
                    awayB = nextParticle;
                    awayClosest = hitPt;
                    awayFound = true;
               }
          }
          else
          {
               if(dist < sameDist)
               {
                    sameDist = dist;
                    sameA = currentParticle;
                    sameB = nextParticle;
                    sameClosest = hitPt;
               }
          }
     }

     if(awayFound)
     {
          *Aout = awayA;
          *Bout = awayB;

          *Dout = jgVector2PositionAlong(awayA->position,
                                         awayB->position,
                                         awayClosest);
          *normout  = jgVector2Normal(awayA->position,
                                      awayB->position);
          return awayClosest;
     }
     else
     {
          *Aout = sameA;
          *Bout = sameB;

          *Dout = jgVector2PositionAlong(sameA->position,
                                         sameB->position,
                                         sameClosest);
          *normout  = jgVector2Normal(sameA->position,
                                      sameB->position);
          return sameClosest;
     }
}

float jgAreaArea(jgArea *area)
{
     float sum = 0;
     jgParticle *currentParticle;
     jgParticle *nextParticle;
     JG_LIST_FOREACH_PAIR(area->particles, currentParticle, nextParticle)
     {
          jgVector2 current = currentParticle->position;
          jgVector2 next = nextParticle->position;

          sum += (current.x * next.y) - (next.x * current.y);
     }
     return sum / 2;
}

bool jgAreaIsInsideOut(jgArea *area)
{
     return jgAreaArea(area) < 0;
}
