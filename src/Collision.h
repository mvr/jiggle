#ifndef __JG_COLLISION_H__
#define __JG_COLLISION_H__

#include "jiggle.h"

typedef struct jgCollision
{
     jgParticle *particle;
     jgArea     *area;
     jgParticle *areaParticleA;
     jgParticle *areaParticleB;
     jgVector2   hitPt;
     float       edgeD;
     jgVector2   normal;
     float       penetration;
} jgCollision;

extern jgCollision       *jgCollisionAlloc();
extern void               jgCollisionFree(jgCollision *collision);

#endif
