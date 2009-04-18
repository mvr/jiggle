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

     jgList         *pendingCollisions;    // That need resolving
     jgList         *collisions;           // That hang around for peeking at

     float           penetrationThreshold;
     int             penetrationCount;

     float           damping;
     jgVector2       gravity;
} jgWorld;

extern jgWorld          *jgWorldAlloc();
extern jgWorld          *jgWorldInit(jgWorld *world);
extern jgWorld          *jgWorldNew();
extern void              jgWorldFree(jgWorld *world);
extern void              jgWorldFreeChildren(jgWorld *world);

extern void              jgWorldAddArea(jgWorld *world, jgArea *area);
extern void              jgWorldRemoveArea(jgWorld *world, jgArea *area);
extern void              jgWorldAddParticle(jgWorld *world, jgParticle *particle);
extern void              jgWorldRemoveParticle(jgWorld *world, jgParticle *particle);
extern void              jgWorldAddSpring(jgWorld *world, jgSpring *spring);
extern void              jgWorldRemoveSpring(jgWorld *world, jgSpring *spring);

extern void              jgWorldClearCollisions(jgWorld *world);

extern void              jgWorldStep(jgWorld *world, float timestep);

#endif
