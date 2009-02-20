#include <stdlib.h>
#include <math.h>

#include "Particle.h"
#include "Vector2.h"

jgParticle *jgParticleNew(float mass, jgVector2 pos)
{
     jgParticle *particle = malloc(sizeof(jgParticle));
     particle->mass = mass;
     particle->position = pos;
     particle->velocity = jgVector2Zero();
     particle->force = jgVector2Zero();
     return particle;
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

