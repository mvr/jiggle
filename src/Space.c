#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "jiggle.h"

jgSpace *jgSpaceAlloc()
{
     jgSpace *space = malloc(sizeof(jgSpace));
     space->particles = jgListNew();
     space->areas = jgListNew();
     space->springs = jgListNew();
     space->pendingCollisions = jgListNew();
     space->collisions = jgListNew();
     return space;
}

jgSpace *jgSpaceInit(jgSpace *space)
{
     space->penetrationThreshold = 0.3;

     space->damping = 0.999;
     space->gravity = jgVector2Zero();

     return space;
}

jgSpace *jgSpaceNew()
{
     return jgSpaceInit(jgSpaceAlloc());
}

void jgSpaceFree(jgSpace *space)
{
     jgListFree(space->particles);
     jgListFree(space->areas);
     jgListFree(space->springs);
     jgListFree(space->pendingCollisions);
     jgListFree(space->collisions);
     free(space);
}

void jgSpaceClearCollisions(jgSpace *space)
{
     jgCollision *collision;
     JG_LIST_FOREACH(space->collisions, collision)
          jgCollisionFree(collision);

     jgListClear(space->collisions);
}

void jgSpaceFreeChildren(jgSpace *space)
{
     jgSpaceClearCollisions(space);

     jgParticle *currentParticle;
     JG_LIST_FOREACH(space->particles, currentParticle)
     {
          jgParticleFree(currentParticle);
     }

     jgSpring *currentSpring;
     JG_LIST_FOREACH(space->springs, currentSpring)
     {
          jgSpringFree(currentSpring);
     }

     jgArea *currentArea;
     JG_LIST_FOREACH(space->areas, currentArea)
     {
          jgAreaFree(currentArea);
     }
}

#define CREATE_ADDS_AND_REMOVES(class, name)                            \
     void jgSpaceAdd ## class(jgSpace *space, jg ## class *name)        \
     {                                                                  \
          if(!jgListContains(space->name ## s, name))                   \
          {                                                             \
               jgListAdd(space->name ## s, name);                       \
          }                                                             \
     }                                                                  \
                                                                        \
     void jgSpaceRemove ## class(jgSpace *space, jg ## class *name)     \
     {                                                                  \
          if(jgListContains(space->name ## s, name))                    \
          {                                                             \
               jgListRemove(space->name ## s, name);                    \
          }                                                             \
     }                                                                  \

CREATE_ADDS_AND_REMOVES(Area,     area)
CREATE_ADDS_AND_REMOVES(Particle, particle)
CREATE_ADDS_AND_REMOVES(Spring,   spring)

#undef CREATE_ADDS_AND_REMOVES

void jgSpaceHandleCollisions(jgSpace *space)
{
     // I'm just copying, I don't understand any of this... yet.

     jgCollision *collision;
     JG_LIST_FOREACH(space->pendingCollisions, collision)
     {
          jgParticle *A  = collision->particle;
          jgParticle *B1 = collision->areaParticleA;
          jgParticle *B2 = collision->areaParticleB;

          if(collision->penetration > space->penetrationThreshold)
          {
               space->penetrationCount++;
               continue;
          }

//          float elas = collision->particle->elasticity * collision->area->elasticity + 1;

          if(A->mass != INFINITY)
	  {
               A->position = jgVector2Add(A->position, jgVector2Multiply(collision->normal, collision->Amove));
	  }

          if(B1->mass != INFINITY && B2->mass != INFINITY)
          {
               B1->position = jgVector2Subtract(B1->position, jgVector2Multiply(collision->normal, collision->B1move));
               B2->position = jgVector2Subtract(B2->position, jgVector2Multiply(collision->normal, collision->B2move));
          }
     }
     jgListClear(space->pendingCollisions);
}

void jgSpaceStep(jgSpace *space, float timeStep)
{
     space->penetrationCount = 0;

     jgParticle *currentParticle;
     JG_LIST_FOREACH(space->particles, currentParticle)
     {
          jgParticleDampenVelocity(currentParticle, space->damping);
          if(!currentParticle->floating)
               jgParticleAddMasslessForce(currentParticle, space->gravity);
     }

     jgSpring *currentSpring;
     JG_LIST_FOREACH(space->springs, currentSpring)
     {
          jgSpringExert(currentSpring);
     }

     JG_LIST_FOREACH(space->particles, currentParticle)
     {
          jgParticleIntegrate(currentParticle, timeStep);
     }

     jgArea *currentArea;
     JG_LIST_FOREACH(space->areas, currentArea)
     {
          currentArea->isValid = jgAreaIsInsideOut(currentArea);

          jgAreaUpdateAABB(currentArea, timeStep);

          jgAreaUpdateCenterOfMass(currentArea);
     }

     jgQuadtree *tree = jgQuadtreeNew(space->areas);

     jgArea *area;
     jgParticle *particle;
     JG_LIST_FOREACH(space->particles, particle)
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

               jgCollision *collision = jgCollisionFind(area, particle);

               jgListAdd(space->collisions, collision);
               jgListAdd(space->pendingCollisions, collision);
          }
          jgListFree(candidates);
     }

     jgQuadtreeFree(tree);

     jgSpaceHandleCollisions(space);
}
