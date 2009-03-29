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

jgWorld *jgWorldInit(jgWorld *world, jgAABB bounds, float ticksPerSecond)
{
     jgWorldSetSize(world, bounds);

     world->penetrationThreshold = 0.3;

     world->damping = 0.999;
     world->gravity = jgVector2Zero();

     world->ticksPerSecond = ticksPerSecond;
     world->currentTime = 0.0;
     world->timeAccumulator = 0.0;
     
     return world;
}

jgWorld *jgWorldNew(jgAABB bounds, float ticksPerSecond)
{
     return jgWorldInit(jgWorldAlloc(), bounds, ticksPerSecond);
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

void jgWorldSetSize(jgWorld *world, jgAABB bounds)
{
     world->limits = bounds;
     world->size = jgVector2Subtract(bounds.max, bounds.min);
     world->gridstep = jgVector2Divide(world->size, 32);
}

#define CREATE_ADDS_AND_REMOVES(class, name)                            \
     void jgWorldAdd ## class(jgWorld *world, jg ## class *name)        \
     {                                                                  \
          if(!jgListContains(world->name ## s, name))                   \
          {                                                             \
               jgListAdd(world->name ## s, name);                       \
          }                                                             \
     }                                                                  \
                                                                        \
     void jgWorldRemove ## class(jgWorld *world, jg ## class *name)     \
     {                                                                  \
          if(jgListContains(world->name ## s, name))                    \
          {                                                             \
               jgListRemove(world->name ## s, name);                    \
          }                                                             \
     }                                                                  \

CREATE_ADDS_AND_REMOVES(Area,     area)
CREATE_ADDS_AND_REMOVES(Particle, particle)
CREATE_ADDS_AND_REMOVES(Spring,   spring)

#undef CREATE_ADDS_AND_REMOVES

void jgWorldAreaCollide(jgWorld *world, jgArea *area, jgParticle *hitParticle)
{
     jgParticle *prev, *particle, *next;
     JG_LIST_FOREACH_TRIPLET(area->particles, prev, particle, next)
     {
          jgCollisionInfo *info = malloc(sizeof(jgCollisionInfo));

          // Will this work?
          jgVector2 norm = jgVector2Normalize(particle->velocity);

          info->particle = particle;
          info->area = area;
          info->hitPt = jgAreaClosestOnEdge(area, 
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
     }

     jgArea *area;
     jgParticle *particle;
     JG_LIST_FOREACH(world->areas, area)
     {
          JG_LIST_FOREACH2(world->particles, particle)
          {
               if(!jgAABBContains(area->aabb, particle->position))
                    continue;

               if(!jgAreaContains(area, particle->position))
                    continue;

               if(!area->isValid)
                    continue;

               jgWorldAreaCollide(world, area, particle);
          }
     }

     jgWorldHandleCollisions(world);
}
