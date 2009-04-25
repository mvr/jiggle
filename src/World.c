#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "World.h"
#include "Vector2.h"
#include "List.h"
#include "Particle.h"
#include "AABB.h"
#include "Collision.h"

jgWorld *jgWorldAlloc()
{
     jgWorld *world = malloc(sizeof(jgWorld));
     world->particles = jgListNew();
     world->areas = jgListNew();
     world->springs = jgListNew();
     world->pendingCollisions = jgListNew();
     world->collisions = jgListNew();
     return world;
}

jgWorld *jgWorldInit(jgWorld *world)
{
     world->penetrationThreshold = 0.3;

     world->damping = 0.999;
     world->gravity = jgVector2Zero();

     return world;
}

jgWorld *jgWorldNew()
{
     return jgWorldInit(jgWorldAlloc());
}

void jgWorldFree(jgWorld *world)
{
     jgListFree(world->particles);
     jgListFree(world->areas);
     jgListFree(world->springs);
     jgListFree(world->pendingCollisions);
     jgListFree(world->collisions);
     free(world);
}

void jgWorldClearCollisions(jgWorld *world)
{
     jgCollision *collision;
     JG_LIST_FOREACH(world->collisions, collision)
          jgCollisionFree(collision);

     jgListClear(world->collisions);
}

void jgWorldFreeChildren(jgWorld *world)
{
     jgWorldClearCollisions(world);

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

void jgWorldAreaCollide(jgWorld *world, jgArea *area, jgParticle *particle)
{
     jgCollision *collision = jgCollisionAlloc();
     
     collision->particle = particle;
     collision->area = area;
     collision->hitPt = jgAreaClosestOnEdge(area, 
                                            particle,
                                            &collision->areaParticleA, 
                                            &collision->areaParticleB, 
                                            &collision->edgeD, 
                                            &collision->normal);
     collision->penetration = jgVector2DistanceBetween(particle->position, collision->hitPt);

     jgListAdd(world->collisions, collision);
     jgListAdd(world->pendingCollisions, collision);
}

void jgWorldHandleCollisions(jgWorld *world)
{
     // I'm just copying, I don't understand any of this... yet.

     jgCollision *collision;
     JG_LIST_FOREACH(world->pendingCollisions, collision)
     {
          jgParticle *A  = collision->particle;
          jgParticle *B1 = collision->areaParticleA;
          jgParticle *B2 = collision->areaParticleB;

          jgVector2 bVel = jgVector2Multiply(jgVector2Add(B1->velocity, B2->velocity), 0.5);
          jgVector2 relVel = jgVector2Subtract(A->velocity, bVel);

          float relDot = jgVector2Dot(relVel, collision->normal);

          if(collision->penetration > world->penetrationThreshold)
          {
               world->penetrationCount++;
               continue;
          }

          float b1inf = 1.0 - collision->edgeD;
          float b2inf = collision->edgeD;

          float b2MassSum = B1->mass + B2->mass;
          float massSum = A->mass + b2MassSum;

          float Amove;
          float Bmove;
	  if(A->mass == INFINITY)
          {
               Amove = 0;
               Bmove = collision->penetration + 0.001;
          }
          else if(b2MassSum == INFINITY)
          {
               Amove = collision->penetration + 0.001;
               Bmove = 0;
          }
          else
          {
               Amove = collision->penetration * (b2MassSum / massSum);
               Bmove = collision->penetration * (A->mass / massSum);
          }

          float B1move = Bmove * b1inf;
          float B2move = Bmove * b2inf;

          float AinvMass = 1.0 / A->mass;
          float BinvMass = 1.0 / b2MassSum;

          float jDenom = AinvMass + BinvMass;
          float elas = collision->particle->elasticity * collision->area->elasticity + 1;
          jgVector2 numV = jgVector2Multiply(relVel, elas);

          float jNumerator = jgVector2Dot(numV, collision->normal);
          jNumerator = -jNumerator;

          float j = jNumerator / jDenom;

          if(A->mass != INFINITY)
	  {
               A->position = jgVector2Add(A->position, jgVector2Multiply(collision->normal, Amove));
	  }

          if(b2MassSum != INFINITY)
          {
               B1->position = jgVector2Subtract(B1->position, jgVector2Multiply(collision->normal, B1move));
               B2->position = jgVector2Subtract(B2->position, jgVector2Multiply(collision->normal, B2move));
          }

          jgVector2 tangent = jgVector2Perpendicular(collision->normal);
          float friction = collision->particle->friction * collision->area->friction;

          float fNumerator = jgVector2Dot(relVel, tangent);
          fNumerator *= friction;
          float f = fNumerator / jDenom;

          if (relDot < 0)
          {
	       if(A->mass != INFINITY)
               {
                    A->velocity.x += (collision->normal.x * (j / A->mass)) - (tangent.x * (f / A->mass));
                    A->velocity.y += (collision->normal.y * (j / A->mass)) - (tangent.y * (f / A->mass));
               }
	       if(b2MassSum != INFINITY)
               {
                    B1->velocity.x -= (collision->normal.x * (j / b2MassSum) * b1inf) - (tangent.x * (f / b2MassSum) * b1inf);
                    B1->velocity.y -= (collision->normal.y * (j / b2MassSum) * b1inf) - (tangent.y * (f / b2MassSum) * b1inf);

                    B2->velocity.x -= (collision->normal.x * (j / b2MassSum) * b2inf) - (tangent.x * (f / b2MassSum) * b2inf);
                    B2->velocity.y -= (collision->normal.y * (j / b2MassSum) * b2inf) - (tangent.y * (f / b2MassSum) * b2inf);
               }
          }
     }
     jgListClear(world->pendingCollisions);
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
               if(jgListContains(area->particles, particle))
                    continue;

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
