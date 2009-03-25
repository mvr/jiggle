/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_PARTICLE_H__
#define __JG_PARTICLE_H__

#include "Vector2.h"

typedef struct jgParticle
{
     float     mass;
     jgVector2 position;
     jgVector2 velocity;
     jgVector2 force;
} jgParticle;

extern jgParticle       *jgParticleNew(float mass, jgVector2 pos);

extern void              jgParticleDampenVelocity(jgParticle *particle, float damp);
extern void              jgParticleAddMasslessForce(jgParticle *particle, jgVector2 force);

extern void              jgParticleIntegrate(jgParticle *point, float elapsed);

#endif
