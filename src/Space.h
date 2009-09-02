/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_SPACE_H__
#define __JG_SPACE_H__

#include <stdbool.h>

#include "jiggle.h"

typedef struct jgSpace
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
} jgSpace;

extern jgSpace          *jgSpaceAlloc();
extern jgSpace          *jgSpaceInit(jgSpace *space);
extern jgSpace          *jgSpaceNew();
extern void              jgSpaceFree(jgSpace *space);
extern void              jgSpaceFreeChildren(jgSpace *space);

extern void              jgSpaceAddArea(jgSpace *space, jgArea *area);
extern void              jgSpaceRemoveArea(jgSpace *space, jgArea *area);
extern void              jgSpaceAddParticle(jgSpace *space, jgParticle *particle);
extern void              jgSpaceRemoveParticle(jgSpace *space, jgParticle *particle);
extern void              jgSpaceAddSpring(jgSpace *space, jgSpring *spring);
extern void              jgSpaceRemoveSpring(jgSpace *space, jgSpring *spring);

extern void              jgSpaceClearCollisions(jgSpace *space);

extern void              jgSpaceStep(jgSpace *space, float timestep);

#endif
