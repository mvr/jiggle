#include <stdlib.h>

#include "jiggle.h"

jgArea *jgAreaAlloc()
{
     return malloc(sizeof(jgArea));
}

jgArea *jgAreaInit(jgArea *area, jgParticle **particles, int numOfParticles)
{
     area->particles = jgListNewFromArray((void **)particles, numOfParticles);

     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgListAdd(currentParticle->ownerAreas, area);
     }

     area->friction  = 0.8;
     area->elasticity = 0.3;

     jgAreaUpdateAABB(area, 0.0);
     jgAreaUpdateCenterOfMass(area);

     return area;
}

jgArea *jgAreaNew(jgParticle **particles, int numOfParticles)
{
     return jgAreaInit(jgAreaAlloc(), particles, numOfParticles);
}

void jgAreaFree(jgArea *area)
{
     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgListRemove(currentParticle->ownerAreas, area);
     }

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

//          p = jgVector2Add(p, jgVector2Multiply(currentParticle->velocity, elapsed));
//          area->aabb = jgAABBExpandToInclude(area->aabb, p);
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

jgCollision *jgAreaFindCollision(jgArea *area, jgParticle *particle)
{
     float       sameDist,     awayDist;
     jgParticle *sameA,       *awayA;
     jgParticle *sameB,       *awayB;
     jgVector2   sameClosest,  awayClosest;

     jgVector2 pt = particle->position;
     jgVector2 normal = jgParticleAreaNormal(particle);

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

     jgCollision *collision = jgCollisionAlloc();

     collision->area = area;
     collision->particle = particle;

     if(awayFound)
     {
          collision->areaParticleA = awayA;
          collision->areaParticleB = awayB;

          collision->hitPt = awayClosest;
     }
     else
     {
          collision->areaParticleA = sameA;
          collision->areaParticleB = sameB;

          collision->hitPt = sameClosest;
     }

     collision->edgeD = jgVector2PositionAlong(collision->areaParticleA->position,
                                               collision->areaParticleB->position,
                                               awayClosest);

     collision->normal = jgVector2Normal(collision->areaParticleA->position,
                                         collision->areaParticleB->position);

     collision->penetration = jgVector2DistanceBetween(particle->position, collision->hitPt);

     return collision;
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

void jgAreaUpdateCenterOfMass(jgArea *area)
{
     float totalMass = 0;
     jgVector2 center = jgv(0, 0);

     jgParticle *particle;
     JG_LIST_FOREACH(area->particles, particle)
     {
          totalMass += particle->mass;
          center = jgVector2Add(center, jgVector2Multiply(particle->position, particle->mass));
     }
     
     center = jgVector2Divide(center, totalMass);

     area->centerOfMass = center;
}
