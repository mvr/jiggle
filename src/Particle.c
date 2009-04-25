#include <stdlib.h>
#include <math.h>

#include "jiggle.h"

jgParticle *jgParticleAlloc()
{
     return malloc(sizeof(jgParticle));
}

jgParticle *jgParticleInit(jgParticle *particle, float mass, jgVector2 pos)
{
     particle->mass = mass;
     particle->position = pos;
     particle->velocity = jgVector2Zero();
     particle->force = jgVector2Zero();

     particle->friction  = 0.8;
     particle->elasticity = 0.3;

     return particle;
}

jgParticle *jgParticleNew(float mass, jgVector2 pos)
{
     return jgParticleInit(jgParticleAlloc(), mass, pos);
}

void jgParticleFree(jgParticle *particle)
{
     free(particle);
}

void jgParticleDampenVelocity(jgParticle *particle, float damp)
{
     if(particle->mass != INFINITY)
          particle->velocity = jgVector2Multiply(particle->velocity, damp);
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
          float elapMass = elapsed / point->mass;
          point->velocity = jgVector2Add(point->velocity, jgVector2Multiply(point->force, elapMass));
          point->position = jgVector2Add(point->position, jgVector2Multiply(point->velocity, elapsed));
     }
     point->force = jgVector2Zero();
}

