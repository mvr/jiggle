/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_WORLD_H__
#define __JG_WORLD_H__

#include <stdbool.h>

#include "Vector2.h"
#include "Body.h"
#include "List.h"
#include "AABB.h"
#include "Misc.h"

typedef struct jgCollisionInfo
{
     jgParticle *collisionParticle;
     jgArea     *area;
     jgParticle *areaParticleA;
     jgParticle *areaParticleB;
     jgVector2   hitPt;
     float       edgeD;
     jgVector2   normal;
     float       penetration;
} jgCollisionInfo;

typedef struct jgWorld
{
     jgList         *particles;
     jgList         *areas;
     jgList         *springs;

     jgList         *collisions;

     jgAABB          limits;
     jgVector2       size;
     jgVector2       gridstep;

     float           penetrationThreshold;
     int             penetrationCount;

     float           damping;
     jgVector2       gravity;

     float           ticksPerSecond;
     float           currentTime;
     float           timeAccumulator;
} jgWorld;

extern jgWorld          *jgWorldAlloc();
extern jgWorld          *jgWorldInit(jgWorld *world, jgVector2 min, jgVector2 max, float ticksPerSecond, float currentTime);
extern jgWorld          *jgWorldNew(jgVector2 min, jgVector2 max, float ticksPerSecond, float currentTime);
extern void              jgWorldFree(jgWorld *world);
extern void              jgWorldFreeChildren(jgWorld *world);

extern void              jgWorldSetSize(jgWorld *world, jgVector2 min, jgVector2 max);

extern void              jgWorldAddBody(jgWorld *world, jgBody *body);
extern void              jgWorldRemoveBody(jgWorld *world, jgBody *body);

extern void              jgWorldBodyCollide(jgWorld *world, jgBody *a, jgBody *b);
extern void              jgWorldStep(jgWorld *world, float timestep);
extern void              jgWorldUpdate(jgWorld *world, float newTime);

#endif
