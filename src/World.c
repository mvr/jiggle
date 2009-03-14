#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "World.h"
#include "Body.h"
#include "Vector2.h"
#include "List.h"
#include "AABB.h"

static void jgWorldUpdateBodyBitmask(jgWorld *world, jgBody *body)
{
     jgAABB box = body->aabb;

     int minX = (int)floor((box.min.x - world->limits.min.x) / world->gridstep.x);
     int maxX = (int)floor((box.max.x - world->limits.min.x) / world->gridstep.x);

     minX = minX % 32;
     maxX = maxX % 32;

     int minY = (int)floor((box.min.y - world->limits.min.y) / world->gridstep.y);
     int maxY = (int)floor((box.max.y - world->limits.min.y) / world->gridstep.y);

     minY = minY % 32;
     maxY = maxY % 32;

     JG_BITMASK_CLEAR(body->bitmaskX);
     for (int i = minX; i <= maxX; i++)
          JG_BITMASK_SET_ON(body->bitmaskX, i);

     JG_BITMASK_CLEAR(body->bitmaskY);
     for (int i = minY; i <= maxY; i++)
          JG_BITMASK_SET_ON(body->bitmaskY, i);
}

jgWorld *jgWorldAlloc()
{
     jgWorld *world = malloc(sizeof(jgWorld));
     world->bodies = jgListNew();
     world->collisions = jgListNew();
     return world;
}

jgWorld *jgWorldInit(jgWorld *world, jgVector2 min, jgVector2 max)
{
     jgWorldSetSize(world, min, max);

     world->penetrationThreshold = 0.3;

     world->damping = 0.999;
     world->gravity = jgVector2Zero();

     return world;
}

jgWorld *jgWorldNew(jgVector2 min, jgVector2 max)
{
     return jgWorldInit(jgWorldAlloc(), min, max);
}

void jgWorldFree(jgWorld *world)
{
     jgListFree(world->bodies);
     jgListFree(world->collisions);
     free(world);
}

void jgWorldFreeBodies(jgWorld *world)
{
     jgBody *currentBody;
     JG_LIST_FOREACH(world->bodies, currentBody)
     {
          jgBodyFree(currentBody);
     }
}

void jgWorldSetSize(jgWorld *world, jgVector2 min, jgVector2 max)
{
     world->limits = jgAABBNewFromVector2(min, max);
     world->size = jgVector2Subtract(max, min);
     world->gridstep = jgVector2Divide(world->size, 32);
}

void jgWorldAddBody(jgWorld *world, jgBody *body)
{
     if(!jgListContains(world->bodies, body))
     {
          jgListAdd(world->bodies, body);
     }
}

void jgWorldRemoveBody(jgWorld *world, jgBody *body)
{
     if(jgListContains(world->bodies, body))
     {
          jgListRemove(world->bodies, body);
     }
}

jgBody *jgWorldBodyContaining(jgWorld *world, jgVector2 point)
{
     jgBody *currentBody;
     JG_LIST_FOREACH(world->bodies, currentBody)
     {
          if(jgBodyContains(currentBody, point))
               return currentBody;
     }
     return NULL;
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

void jgWorldUpdate(jgWorld *world, float elapsed)
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
          jgBodyIntegrate(currentBody, elapsed);
          jgBodyDerive(currentBody);

          if(jgBodyIsInsideOut(currentBody))
               currentBody->isActive = false;

          jgBodyUpdateAABB(currentBody, elapsed, false);
          jgWorldUpdateBodyBitmask(world, currentBody);
     }

     jgBody *a;
     jgBody *b;
     JG_LIST_FOREACH(world->bodies, a)
     {
          JG_LIST_FOREACH_COMBO(b)
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
