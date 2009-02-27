/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __AREA_H__
#define __AREA_H__

#include "Vector2.h"

typedef struct jgArea
{
     jgList     *particles;

     float       elasticity;
     float       friction;

     jgAABB      aabb;
} jgArea;

extern jgArea *jgAreaNew(jgParticle *particles, int numOfParticles);
extern void    jgAreaFree(jgArea *area);
extern void    jgAreaUpdateAABB(jgArea *area, float elapsed);
extern float   jgAreaArea(jgArea *area); // Heh.
extern bool    jgAreaIsInsideOut(jgArea *area);

#endif
