/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_WORLD_H__
#define __JG_WORLD_H__

#include <stdbool.h>

#include "Vector2.h"
#include "List.h"
#include "AABB.h"
#include "Area.h"
#include "Spring.h"

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
extern jgWorld          *jgWorldInit(jgWorld *world, jgAABB bounds, float ticksPerSecond);
extern jgWorld          *jgWorldNew(jgAABB bounds, float ticksPerSecon);
extern void              jgWorldFree(jgWorld *world);
extern void              jgWorldFreeChildren(jgWorld *world);

extern void              jgWorldSetSize(jgWorld *world, jgAABB bounds);

extern void              jgWorldAddArea(jgWorld *world, jgArea *area);
extern void              jgWorldRemoveArea(jgWorld *world, jgArea *area);
extern void              jgWorldAddParticle(jgWorld *world, jgParticle *particle);
extern void              jgWorldRemoveParticle(jgWorld *world, jgParticle *particle);
extern void              jgWorldAddSpring(jgWorld *world, jgSpring *spring);
extern void              jgWorldRemoveSpring(jgWorld *world, jgSpring *spring);

extern void              jgWorldStep(jgWorld *world, float timestep);
extern void              jgWorldUpdate(jgWorld *world, float newTime);

#endif
