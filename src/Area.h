/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_AREA_H__
#define __JG_AREA_H__

#include <stdbool.h>

#include "jiggle.h"

typedef struct jgArea
{
     jgList      *particles;

     float        elasticity;
     float        friction;

     jgAABB       aabb;
     
     bool         isValid;
} jgArea;

extern jgArea             *jgAreaAlloc();
extern jgArea             *jgAreaInit(jgArea *area, jgParticle **particles, int numOfParticles);
extern jgArea             *jgAreaNew(jgParticle **particles, int numOfParticles);
extern void                jgAreaFree(jgArea *area);

extern void                jgAreaUpdateAABB(jgArea *area, float elapsed);
extern bool                jgAreaContains(jgArea *area, jgVector2 point);

struct        jgCollision;
extern struct jgCollision *jgAreaFindCollision(jgArea *area, jgParticle *particle);

extern float               jgAreaArea(jgArea *area); // Heh.
extern bool                jgAreaIsInsideOut(jgArea *area);

#endif
