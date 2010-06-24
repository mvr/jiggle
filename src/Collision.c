#include <stdlib.h>

#include "jiggle.h"

jgCollision *jgCollisionAlloc()
{
     return malloc(sizeof(jgCollision));
}

void jgCollisionFree(jgCollision *collision)
{
     free(collision);
}

jgCollision *jgCollisionFind(jgArea *area, jgParticle *particle)
{
     jgCollision *same = jgCollisionAlloc();
     jgCollision *away = jgCollisionAlloc();
     jgCollision *collision;
     bool awayFound = false;

     same->penetration = away->penetration = INFINITY;

     jgVector2 normal = jgParticleAreaNormal(particle);

     jgParticle *currentParticle;
     jgParticle *nextParticle;
     JG_LIST_FOREACH_PAIR(area->particles, currentParticle, nextParticle)
     {
          jgVector2 hitPt = jgVector2ClosestPointOnLine(currentParticle->position,
                                                        nextParticle->position,
                                                        particle->position);
          float dist = jgVector2DistanceBetween(particle->position, hitPt);

          float dot = jgVector2Dot(normal, jgVector2Normal(currentParticle->position, 
                                                           nextParticle->position));
          if(dot <= 0)
          {
               if(dist < away->penetration)
               {
                    away->penetration = dist;
                    away->areaParticleA = currentParticle;
                    away->areaParticleB = nextParticle;
                    away->hitPt = hitPt;
                    awayFound = true;
               }
          }
          else
          {
               if(dist < same->penetration)
               {
                    same->penetration = dist;
                    same->areaParticleA = currentParticle;
                    same->areaParticleB = nextParticle;
                    same->hitPt = hitPt;
               }
          }
     }

     if(awayFound)
     {
          jgCollisionFree(same);
          collision = away;
     }
     else
     {
          jgCollisionFree(away);
          collision = same;
     }

     collision->area = area;
     collision->particle = particle;

     collision->edgeD = jgVector2PositionAlong(collision->areaParticleA->position,
                                               collision->areaParticleB->position,
                                               collision->hitPt);

     collision->normal = jgVector2Normal(collision->areaParticleA->position,
                                         collision->areaParticleB->position);

     return collision;
}
