/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __INTERNALSPRING_H__
#define __INTERNALSPRING_H__

#include "Vector2.h"
#include "PointMass.h"

typedef struct jgSpring
{
     jgPointMass        *massA;
     jgPointMass        *massB;
     float               springD;
     float               springK;
     float               damping;
} jgSpring;

extern jgSpring *jgSpringNew(jgPointMass *a,
                             jgPointMass *b,
                             float d,
                             float k,
                             float damp);
extern void      jgSpringFree(jgSpring *spring);
extern void      jgSpringExert(jgSpring *spring);

extern void      jgSpringDragTowards(jgPointMass *mass, jgVector2 point, float d, float k, float damp);
extern void      jgSpringDragTogether(jgPointMass *massA, jgPointMass *massB, float d, float k, float damp);

#endif
