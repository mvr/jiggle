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

void jgSpaceHandleCollisions(jgSpace *space, float timeStep)
{
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
          
          jgVector2 averageBVelocity = jgVector2Divide(jgVector2Add(B1->velocity, B2->velocity), 2);
          jgVector2 relativeVelocity = jgVector2Subtract(A->velocity, averageBVelocity);
          float relativeDot = jgVector2Dot(collision->normal, relativeVelocity);

          float jDenom = 1.0/A->mass + 1.0/(B1->mass + B2->mass);
          float elasticity = 1 + A->elasticity * (B1->elasticity + B2->elasticity) / 2;

          jgVector2 numV = jgVector2Multiply(relativeVelocity, elasticity);
          float jNumerator = -jgVector2Dot(numV, collision->normal);
          float j = jNumerator / jDenom;

          jgVector2 tangent = jgVector2Perpendicular(collision->normal);
          float friction = A->friction * (B1->friction + B2->friction) / 2;

          float fNumerator = jgVector2Dot(relativeVelocity, tangent) * friction;
          float f = fNumerator / jDenom;

          if(A->mass != INFINITY)
	  {
               A->position = jgVector2Add(A->position, jgVector2Multiply(collision->normal, collision->Amove));
	  }

          if(B1->mass + B2->mass != INFINITY)
          {
               B1->position = jgVector2Subtract(B1->position, jgVector2Multiply(collision->normal, collision->B1move));
               B2->position = jgVector2Subtract(B2->position, jgVector2Multiply(collision->normal, collision->B2move));
          }

          if(relativeDot < 0)
          {
               jgVector2 Achange = jgVector2Subtract(jgVector2Multiply(collision->normal, j / A->mass),
                                                     jgVector2Multiply(tangent,           f / A->mass));
               A->prevPos = jgVector2Subtract(A->prevPos, jgVector2Multiply(Achange, timeStep));

               jgVector2 B1change = jgVector2Subtract(jgVector2Multiply(collision->normal, j / B1->mass * (1.0 - collision->edgeD)),
                                                      jgVector2Multiply(tangent,           f / B1->mass * (1.0 - collision->edgeD)));
               B1->prevPos = jgVector2Add(B1->prevPos, jgVector2Multiply(B1change, timeStep));

               jgVector2 B2change = jgVector2Subtract(jgVector2Multiply(collision->normal, j / B2->mass * collision->edgeD),
                                                      jgVector2Multiply(tangent,           f / B2->mass * collision->edgeD));
               B2->prevPos = jgVector2Add(B2->prevPos, jgVector2Multiply(B2change, timeStep));
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
     
     jgArea *currentArea;
     JG_LIST_FOREACH(space->areas, currentArea)
     {
          jgAreaDerive(currentArea);
          if(currentArea->isShapeMatching)
            jgAreaMatchShape(currentArea);
     }
     
     JG_LIST_FOREACH(space->particles, currentParticle)
     {
          jgParticleIntegrate(currentParticle, timeStep);
     }

     JG_LIST_FOREACH(space->areas, currentArea)
     {
          currentArea->isValid = !jgAreaIsInsideOut(currentArea);

          jgAreaUpdateAABB(currentArea, timeStep);

          jgAreaUpdateCenterOfMass(currentArea);
     }

     jgQuadtree *tree = jgQuadtreeNew(space->areas);

     jgParticle *particle;
     JG_LIST_FOREACH(space->particles, particle)
     {
          if(!particle->collidable)
               continue;

          void jgSpaceEachCandidate(jgArea *area)
          {
               if(jgListContains(area->particles, particle))
                    return;

               if(!jgAABBContains(area->aabb, particle->position))
                    return;

               if(!jgAreaContains(area, particle->position))
                    return;

               if(!area->isValid)
                    return;

               jgCollision *collision = jgCollisionFind(area, particle);

               jgListAdd(space->collisions, collision);
               jgListAdd(space->pendingCollisions, collision);               
          }

          jgQuadtreeEachCandidate(tree, particle->position, jgSpaceEachCandidate);
     }

     jgQuadtreeFree(tree);

     jgSpaceHandleCollisions(space, timeStep);
}



