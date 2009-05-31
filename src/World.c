#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "jiggle.h"

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

void jgWorldHandleCollisions(jgWorld *world)
{
     // I'm just copying, I don't understand any of this... yet.

     jgCollision *collision;
     JG_LIST_FOREACH(world->pendingCollisions, collision)
     {
          jgParticle *A  = collision->particle;
          jgParticle *B1 = collision->areaParticleA;
          jgParticle *B2 = collision->areaParticleB;

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

//          float elas = collision->particle->elasticity * collision->area->elasticity + 1;

          if(A->mass != INFINITY)
	  {
               A->position = jgVector2Add(A->position, jgVector2Multiply(collision->normal, Amove));
	  }

          if(b2MassSum != INFINITY)
          {
               B1->position = jgVector2Subtract(B1->position, jgVector2Multiply(collision->normal, B1move));
               B2->position = jgVector2Subtract(B2->position, jgVector2Multiply(collision->normal, B2move));
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
          if(!currentParticle->floating)
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

          jgAreaUpdateCenterOfMass(currentArea);
     }

     jgQuadtree *tree = jgQuadtreeNew(world->areas);

     jgArea *area;
     jgParticle *particle;
     JG_LIST_FOREACH(world->particles, particle)
     {
          if(!particle->collidable)
               continue;

          jgList *candidates = jgQuadtreeCandidates(tree, particle->position);
          JG_LIST_FOREACH2(candidates, area)
          {
               if(jgListContains(area->particles, particle))
                    continue; 

               if(!jgAABBContains(area->aabb, particle->position))
                    continue;

               if(!jgAreaContains(area, particle->position))
                    continue;

               if(!area->isValid)
                    continue;

               jgCollision *collision = jgAreaFindCollision(area, particle);

               jgListAdd(world->collisions, collision);
               jgListAdd(world->pendingCollisions, collision);
          }
          jgListFree(candidates);
     }

     jgQuadtreeFree(tree);

     jgWorldHandleCollisions(world);
}
