#ifndef __JG_PARTICLE_H__
#define __JG_PARTICLE_H__

#include "jiggle.h"

typedef struct jgParticle
{
     float     mass;
     jgVector2 position;
     jgVector2 prevPos;
     jgVector2 velocity;
     jgVector2 force;
     jgVector2 constantForce;

     float     friction;

     bool      collidable;
     bool      floating;

     jgList   *ownerAreas;
} jgParticle;

extern jgParticle       *jgParticleAlloc();
extern jgParticle       *jgParticleInit(jgParticle *particle, float mass, jgVector2 pos);
extern jgParticle       *jgParticleNew(float mass, jgVector2 pos);
extern void              jgParticleFree(jgParticle *particle);

extern void              jgParticleDampenVelocity(jgParticle *particle, float damp);
extern void              jgParticleAddMasslessForce(jgParticle *particle, jgVector2 force);

extern void              jgParticleIntegrate(jgParticle *point, float elapsed);

extern jgVector2         jgParticleAreaNormal(jgParticle *point);

#endif
