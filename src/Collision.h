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

     float       Amove;
     float       B1move;
     float       B2move;
} jgCollision;

extern jgCollision       *jgCollisionAlloc();
extern void               jgCollisionFree(jgCollision *collision);

extern jgCollision       *jgCollisionFind(jgArea *area, jgParticle *particle);


#endif
