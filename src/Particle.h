/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Vector2.h"

typedef struct jgParticle
{
     float     mass;
     jgVector2 position;
     jgVector2 velocity;
     jgVector2 force;
} jgParticle;

extern jgParticle       *jgParticleNew(float mass, jgVector2 pos);
extern void              jgParticleIntegrate(jgParticle *point, float elapsed);

#endif
