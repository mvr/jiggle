#ifndef __JG_AREA_H__
#define __JG_AREA_H__

#include <stdbool.h>

#include "jiggle.h"

typedef struct jgArea
{
     jgList      *particles;

     jgAABB       aabb;
     jgVector2    centerOfMass;

     jgVector2   *baseShape;

     bool         isKinematic;
     
     jgVector2    derivedPosition;
     jgVector2    derivedVelocity;
     float        derivedAngle;
     
     bool         isShapeMatching;
     float        shapeStrength;
     float        shapeDamping;

     bool         isValid;
} jgArea;

extern jgArea             *jgAreaAlloc();
extern jgArea             *jgAreaInit(jgArea *area, jgParticle **particles, int numOfParticles);
extern jgArea             *jgAreaNew(jgParticle **particles, int numOfParticles);
extern void                jgAreaFree(jgArea *area);

extern void                jgAreaUpdateAABB(jgArea *area, float elapsed);
extern bool                jgAreaContains(jgArea *area, jgVector2 point);

extern void                jgAreaDerive(jgArea *area);
extern void                jgAreaMatchShape(jgArea *area);

extern float               jgAreaArea(jgArea *area); // Heh.
extern bool                jgAreaIsInsideOut(jgArea *area);

extern void                jgAreaUpdateCenterOfMass(jgArea *area);

#endif
