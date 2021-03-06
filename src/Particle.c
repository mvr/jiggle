#include <stdlib.h>
#include <math.h>

#include "jiggle.h"

jgParticle *jgParticleAlloc()
{
     jgParticle *p = malloc(sizeof(jgParticle));
     p->ownerAreas = jgListNew();
     return p;
}

jgParticle *jgParticleInit(jgParticle *particle, float mass, jgVector2 pos)
{
     particle->mass = mass;
     particle->position = pos;
     particle->prevPos = pos;
     particle->velocity = jgVector2Zero();
     particle->force = jgVector2Zero();
     particle->constantForce = jgVector2Zero();

     particle->friction  = 0.8;
     particle->elasticity  = 0.3;

     particle->collidable = true;
     particle->floating   = false;

     return particle;
}

jgParticle *jgParticleNew(float mass, jgVector2 pos)
{
     return jgParticleInit(jgParticleAlloc(), mass, pos);
}

void jgParticleFree(jgParticle *particle)
{
     free(particle);
     jgListFree(particle->ownerAreas);
}

void jgParticleDampenVelocity(jgParticle *particle, float damp)
{
     if(particle->mass != INFINITY)
          particle->prevPos = jgVector2Towards(particle->prevPos, 
                                               particle->position, 
                                               1.0f - damp);
}

void jgParticleAddMasslessForce(jgParticle *particle, jgVector2 force)
{
     // Multiply mass to negate dividing by mass during integration.
     particle->force = jgVector2Add(particle->force,
                                    jgVector2Multiply(force, particle->mass));
}

void jgParticleIntegrate(jgParticle *point, float elapsed)
{
     if(point->mass != INFINITY)
     {
          jgVector2 acceleration = jgVector2Divide(point->force, point->mass);
          jgVector2 temp = point->position;

          // x' = 2x - x* + at^2
          // Also eww.
          point->position = jgVector2Add(jgVector2Subtract(jgVector2Multiply(point->position, 2.0),
                                                           point->prevPos),
                                         jgVector2Multiply(acceleration,
                                                           elapsed * elapsed));
          point->prevPos = temp;
          
          point->velocity = jgVector2Divide(jgVector2Subtract(point->position, point->prevPos), elapsed);
     }
     //     point->force = jgVector2Zero();
     point->force = point->constantForce;
}

jgVector2 jgParticleAreaNormal(jgParticle *particle)
{
     if(particle->mass == INFINITY)
          return jgv(0, 0);

     if(particle->ownerAreas->length == 0)
          return jgv(0, 0);
     
     jgVector2 sum = jgv(0, 0);
     
     jgArea *currentArea;
     JG_LIST_FOREACH(particle->ownerAreas, currentArea)
     {
          sum = jgVector2Add(sum, currentArea->centerOfMass);
     }

     jgVector2 average = jgVector2Divide(sum, particle->ownerAreas->length);
     
     return jgVector2Normalize(jgVector2Subtract(particle->position, average));
}
