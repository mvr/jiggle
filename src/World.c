#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "World.h"
#include "Vector2.h"
#include "List.h"
#include "Particle.h"
#include "AABB.h"

jgWorld *jgWorldAlloc()
{
     jgWorld *world = malloc(sizeof(jgWorld));
     world->particles = jgListNew();
     world->areas = jgListNew();
     world->springs = jgListNew();
     world->collisions = jgListNew();
     return world;
}

jgWorld *jgWorldInit(jgWorld *world, jgVector2 min, jgVector2 max, float ticksPerSecond, float currentTime)
{
     jgWorldSetSize(world, min, max);

     world->penetrationThreshold = 0.3;

     world->damping = 0.999;
     world->gravity = jgVector2Zero();

     world->ticksPerSecond = ticksPerSecond;
     world->currentTime = currentTime;
     world->timeAccumulator = 0.0;
     
     return world;
}

jgWorld *jgWorldNew(jgVector2 min, jgVector2 max, float ticksPerSecond, float currentTime)
{
     return jgWorldInit(jgWorldAlloc(), min, max, ticksPerSecond, currentTime);
}

void jgWorldFree(jgWorld *world)
{
     jgListNew(world->particles);
     jgListNew(world->areas);
     jgListNew(world->springs);
     jgListNew(world->collisions);
     free(world);
}

void jgWorldFreeChildren(jgWorld *world)
{
     jgParticle *currentParticle;
     JG_LIST_FOREACH(world->particles, currentParticle)
     {
          jgParticleFree(currentParticle);
     }

     jgSpring *currentSpring;
     JG_LIST_FOREACH(world->springs, currentSpring)
     {
          jgSpringFree(currentSpring);
     }

     jgArea *currentArea;
     JG_LIST_FOREACH(world->areas, currentArea)
     {
          jgAreaFree(currentArea);
     }
}

void jgWorldSetSize(jgWorld *world, jgVector2 min, jgVector2 max)
{
     world->limits = jgAABBNewFromVector2(min, max);
     world->size = jgVector2Subtract(max, min);
     world->gridstep = jgVector2Divide(world->size, 32);
}

// SO UNDRY!
void jgWorldAddArea(jgWorld *world, jgArea *area)
{
     if(!jgListContains(world->areas, area))
     {
          jgListAdd(world->areas, area);
     }
}

void jgWorldRemoveArea(jgWorld *world, jgArea *area)
{
     if(jgListContains(world->areas, area))
     {
          jgListRemove(world->areas, area);
     }
}

void jgWorldAddParticle(jgWorld *world, jgParticle *particle)
{
     if(!jgListContains(world->particles, particle))
     {
          jgListAdd(world->particles, particle);
     }
}

void jgWorldRemoveParticle(jgWorld *world, jgParticle *particle)
{
     if(jgListContains(world->particles, particle))
     {
          jgListRemove(world->particles, particle);
     }
}

void jgWorldAddSpring(jgWorld *world, jgSpring *spring)
{
     if(!jgListContains(world->springs, spring))
     {
          jgListAdd(world->springs, spring);
     }
}

void jgWorldRemoveSpring(jgWorld *world, jgSpring *spring)
{
     if(jgListContains(world->springs, spring))
     {
          jgListRemove(world->springs, spring);
     }
}

void jgWorldAreaCollide(jgWorld *world, jgArea *a, jgArea *b)
{
     jgParticle *prev, *particle, *next;
     JG_LIST_FOREACH_TRIPLET(a->particles, prev, particle, next)
     {
          // Early-outs
          if(!jgAABBContains(b->aabb, particle->position))
               continue;

          if(!jgAreaContains(b, particle->position))
               continue;

          jgCollisionInfo *info = malloc(sizeof(jgCollisionInfo));

          jgVector2 norm = jgVector2ThreePointNormal(prev->position, 
                                                     particle->position, 
                                                     next->position);

          info->particle = particle;
          info->area = b;
          info->hitPt = jgAreaClosestOnEdge(b, 
                                            particle->position, 
                                            norm, 
                                            &info->areaParticleA, 
                                            &info->areaParticleB, 
                                            &info->edgeD, 
                                            &info->normal);
          info->penetration = jgVector2DistanceBetween(particle->position, info->hitPt);

          jgListAdd(world->collisions, info);
     }
}

void jgWorldHandleCollisions(jgWorld *world)
{
     // I'm just copying, I don't understand any of this... yet.

     jgCollisionInfo *info;
     JG_LIST_FOREACH(world->collisions, info)
     {
          jgParticle *A  = info->particle;
          jgParticle *B1 = info->areaParticleA;
          jgParticle *B2 = info->areaParticleB;

          jgVector2 bVel = jgVector2Multiply(jgVector2Add(B1->velocity, B2->velocity), 0.5);
          jgVector2 relVel = jgVector2Subtract(A->velocity, bVel);

          float relDot = jgVector2Dot(relVel, info->normal);

          if(info->penetration > world->penetrationThreshold)
          {
               world->penetrationCount++;
               continue;
          }

          float b1inf = 1.0 - info->edgeD;
          float b2inf = info->edgeD;

          float b2MassSum = B1->mass + B2->mass;
          float massSum = A->mass + b2MassSum;

          float Amove;
          float Bmove;
	  if(A->mass == INFINITY)
          {
               Amove = 0;
               Bmove = info->penetration + 0.001;
          }
          else if(b2MassSum == INFINITY)
          {
               Amove = info->penetration + 0.001;
               Bmove = 0;
          }
          else
          {
               Amove = info->penetration * (b2MassSum / massSum);
               Bmove = info->penetration * (A->mass / massSum);
          }

          float B1move = Bmove * b1inf;
          float B2move = Bmove * b2inf;

          float AinvMass = 1.0 / A->mass;
          float BinvMass = 1.0 / b2MassSum;

          float jDenom = AinvMass + BinvMass;
          float elas = info->particle->elasticity * info->area->elasticity + 1;
          jgVector2 numV = jgVector2Multiply(relVel, elas);

          float jNumerator = jgVector2Dot(numV, info->normal);
          jNumerator = -jNumerator;

          float j = jNumerator / jDenom;

          if(A->mass != INFINITY)
	  {
               A->position = jgVector2Add(A->position, jgVector2Multiply(info->normal, Amove));
	  }

          if(b2MassSum != INFINITY)
          {
               B1->position = jgVector2Subtract(B1->position, jgVector2Multiply(info->normal, B1move));
               B2->position = jgVector2Subtract(B2->position, jgVector2Multiply(info->normal, B2move));
          }

          jgVector2 tangent = jgVector2Perpendicular(info->normal);
          float friction = info->particle->friction * info->area->friction;

          float fNumerator = jgVector2Dot(relVel, tangent);
          fNumerator *= friction;
          float f = fNumerator / jDenom;

          if (relDot < 0)
          {
	       if(A->mass != INFINITY)
               {
                    A->velocity.x += (info->normal.x * (j / A->mass)) - (tangent.x * (f / A->mass));
                    A->velocity.y += (info->normal.y * (j / A->mass)) - (tangent.y * (f / A->mass));
               }
	       if(b2MassSum != INFINITY)
               {
                    B1->velocity.x -= (info->normal.x * (j / b2MassSum) * b1inf) - (tangent.x * (f / b2MassSum) * b1inf);
                    B1->velocity.y -= (info->normal.y * (j / b2MassSum) * b1inf) - (tangent.y * (f / b2MassSum) * b1inf);

                    B2->velocity.x -= (info->normal.x * (j / b2MassSum) * b2inf) - (tangent.x * (f / b2MassSum) * b2inf);
                    B2->velocity.y -= (info->normal.y * (j / b2MassSum) * b2inf) - (tangent.y * (f / b2MassSum) * b2inf);
               }
          }

          free(info);
     }
     jgListClear(world->collisions);
}

void jgWorldUpdate(jgWorld *world, float newTime)
{
     float timeStep = 1.0 / world->ticksPerSecond;
     float deltaTime = newTime - world->currentTime;
     world->currentTime = newTime;
     world->timeAccumulator += deltaTime;
     
     while(world->timeAccumulator >= timeStep)
     {
          jgWorldStep(world, timeStep);
          world->timeAccumulator -= timeStep;
     }
}

void jgWorldStep(jgWorld *world, float timeStep)
{
     world->penetrationCount = 0;

     jgParticle *currentParticle;
     JG_LIST_FOREACH(world->particles, currentParticle)
     {
          jgParticleDampenVelocity(currentParticle, world->damping);
          jgParticleAddMasslessForce(currentParticle, world->gravity);
     }

     jgSpring *currentSpring;
     JG_LIST_FOREACH(world->springs, currentSpring)
     {
          jgSpringExert(currentSpring);
     }

     JG_LIST_FOREACH(world->particles, currentParticle)
     {
          jgParticleIntegrate(currentParticle, timeStep);
     }

     jgArea *currentArea;
     JG_LIST_FOREACH(world->areas, currentArea)
     {
          currentArea->isValid = jgAreaIsInsideOut(currentArea);

          jgAreaUpdateAABB(currentArea, timeStep);
          jgAreaUpdateBitmask(currentArea, world);
     }

     jgArea *a;
     jgArea *b;
     JG_LIST_FOREACH(world->areas, a)
     {
          JG_LIST_FOREACH_COMBO(world->areas, b)
          {

               if((!a->bitmaskX & b->bitmaskX) && (!a->bitmaskY & b->bitmaskY))
                    continue;

               if(!a->isValid || !b->isValid)
                    continue;

               if(!jgAABBIntersects(a->aabb, b->aabb))
                    continue;

               jgWorldAreaCollide(world, a, b);
               jgWorldAreaCollide(world, b, a);
          }
     }

     jgWorldHandleCollisions(world);
}
