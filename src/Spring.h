/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_INTERNALSPRING_H__
#define __JG_INTERNALSPRING_H__

#include "jiggle.h"

typedef struct jgSpring
{
     jgParticle         *particleA;
     jgParticle         *particleB;
     float               springD;
     float               springK;
     float               damping;
} jgSpring;

extern jgSpring *jgSpringAlloc();
extern jgSpring *jgSpringInit(jgSpring *spring, 
                              jgParticle *a,
                              jgParticle *b,
                              float d,
                              float k,
                              float damp);
extern jgSpring *jgSpringNew(jgParticle *a,
                             jgParticle *b,
                             float d,
                             float k,
                             float damp);
extern void      jgSpringFree(jgSpring *spring);
extern void      jgSpringExert(jgSpring *spring);

extern void      jgSpringDragTowards(jgParticle *particle, jgVector2 point, float d, float k, float damp);
extern void      jgSpringDragTogether(jgParticle *particleA, jgParticle *particleB, float d, float k, float damp);

#endif
