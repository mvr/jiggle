#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "World.h"
#include "Body.h"
#include "Vector2.h"
#include "List.h"
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

//static void jgWorldBodyCollide(jgWorld *world, jgBody *a, jgBody *b)
void jgWorldBodyCollide(jgWorld *world, jgBody *a, jgBody *b)
{
     for(int i = 0; i < a->numOfPoints; i++)
     {
          jgVector2 pt = a->pointMasses[i].position;

          // Early-outs
          if(!jgAABBContains(b->aabb, pt))
               continue;

          if(!jgBodyContains(b, pt))
               continue;

          jgCollisionInfo *info = malloc(sizeof(jgCollisionInfo));

          // Why doesn't this work?
/*           jgVector2 prev = a->pointMasses[(i - 1) % a->numOfPoints].position; */
/*           jgVector2 next = a->pointMasses[(i + 1) % a->numOfPoints].position; */
          jgVector2 prev = a->pointMasses[i == 0 ? a->numOfPoints - 1 : i - 1].position;
          jgVector2 next = a->pointMasses[i == a->numOfPoints - 1 ? 0 : i + 1].position;
          jgVector2 norm = jgVector2ThreePointNormal(prev, pt, next);

          info->bodyA = a;
          info->bodyB = b;
          info->bodyApm = i;
          info->hitPt = jgBodyClosestOnEdge(b, pt, norm, &info->bodyBpmA, &info->bodyBpmB, &info->edgeD, &info->normal);
          info->penetration = jgVector2DistanceBetween(pt, info->hitPt);

          jgListAdd(world->collisions, info);
     }
}

void jgWorldHandleCollisions(jgWorld *world)
{
     // I'm just copying, I don't understand any of this... yet.

     jgCollisionInfo *info;
     JG_LIST_FOREACH(world->collisions, info)
     {
          jgParticle A  = info->bodyA->pointMasses[info->bodyApm];
          jgParticle B1 = info->bodyB->pointMasses[info->bodyBpmA];
          jgParticle B2 = info->bodyB->pointMasses[info->bodyBpmB];

          jgVector2 bVel = jgVector2Multiply(jgVector2Add(B1.velocity, B2.velocity), 0.5);
          jgVector2 relVel = jgVector2Subtract(A.velocity, bVel);

          float relDot = jgVector2Dot(relVel, info->normal);

          if(info->penetration > world->penetrationThreshold)
          {
               world->penetrationCount++;
               continue;
          }

          float b1inf = 1.0 - info->edgeD;
          float b2inf = info->edgeD;

          float b2MassSum = B1.mass + B2.mass;
          float massSum = A.mass + b2MassSum;

          float Amove;
          float Bmove;
	  if(A.mass == INFINITY)
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
               Bmove = info->penetration * (A.mass / massSum);
          }

          float B1move = Bmove * b1inf;
          float B2move = Bmove * b2inf;

          float AinvMass = 1.0 / A.mass;
          float BinvMass = 1.0 / b2MassSum;

          float jDenom = AinvMass + BinvMass;
          float elas = info->bodyA->elasticity * info->bodyB->elasticity + 1;
          jgVector2 numV = jgVector2Multiply(relVel, elas);

          float jNumerator = jgVector2Dot(numV, info->normal);
          jNumerator = -jNumerator;

          float j = jNumerator / jDenom;

          if(A.mass != INFINITY)
	  {
               A.position = jgVector2Add(A.position, jgVector2Multiply(info->normal, Amove));
	  }

          if(b2MassSum != INFINITY)
          {
               B1.position = jgVector2Subtract(B1.position, jgVector2Multiply(info->normal, B1move));
               B2.position = jgVector2Subtract(B2.position, jgVector2Multiply(info->normal, B2move));
          }

          jgVector2 tangent = jgVector2Perpendicular(info->normal);
          float friction = info->bodyA->friction * info->bodyB->friction;

          float fNumerator = jgVector2Dot(relVel, tangent);
          fNumerator *= friction;
          float f = fNumerator / jDenom;

          if (relDot < 0)
          {
	       if(A.mass != INFINITY)
               {
                    A.velocity.x += (info->normal.x * (j / A.mass)) - (tangent.x * (f / A.mass));
                    A.velocity.y += (info->normal.y * (j / A.mass)) - (tangent.y * (f / A.mass));
               }
	       if(b2MassSum != INFINITY)
               {
                    B1.velocity.x -= (info->normal.x * (j / b2MassSum) * b1inf) - (tangent.x * (f / b2MassSum) * b1inf);
                    B1.velocity.y -= (info->normal.y * (j / b2MassSum) * b1inf) - (tangent.y * (f / b2MassSum) * b1inf);

                    B2.velocity.x -= (info->normal.x * (j / b2MassSum) * b2inf) - (tangent.x * (f / b2MassSum) * b2inf);
                    B2.velocity.y -= (info->normal.y * (j / b2MassSum) * b2inf) - (tangent.y * (f / b2MassSum) * b2inf);
               }
          }

          info->bodyA->pointMasses[info->bodyApm]  = A;
          info->bodyB->pointMasses[info->bodyBpmA] = B1;
          info->bodyB->pointMasses[info->bodyBpmB] = B2;

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

     jgBody *currentBody;
     JG_LIST_FOREACH(world->bodies, currentBody)
     {
          jgBodyDampenVelocity(currentBody, world->damping);
          jgBodyAddForce(currentBody, world->gravity);
     }

     JG_LIST_FOREACH(world->bodies, currentBody)
     {
          if(!currentBody->isActive)
               continue;

          jgBodyAccumulateForces(currentBody);
          jgBodyIntegrate(currentBody, timeStep);
          jgBodyDerive(currentBody);

          if(jgBodyIsInsideOut(currentBody))
               currentBody->isActive = false;

          jgBodyUpdateAABB(currentBody, timeStep, false);
          jgWorldUpdateBodyBitmask(world, currentBody);
     }

     jgBody *a;
     jgBody *b;
     JG_LIST_FOREACH(world->bodies, a)
     {
          JG_LIST_FOREACH_COMBO(world->bodies, b)
          {

               if((!a->bitmaskX & b->bitmaskX) && (!a->bitmaskY & b->bitmaskY))
                    continue;

               if(!a->isActive || !b->isActive)
                    continue;

               if(!(a->layers & b->layers))
                    continue;

               if(a->isStatic && b->isStatic)
                    continue;

               if(!jgAABBIntersects(a->aabb, b->aabb))
                    continue;

               jgWorldBodyCollide(world, a, b);
               jgWorldBodyCollide(world, b, a);
          }
     }

     jgWorldHandleCollisions(world);
}
