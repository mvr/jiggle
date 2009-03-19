/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __BODY_H__
#define __BODY_H__

#include <stdbool.h>

#include "Vector2.h"
#include "Particle.h"
#include "AABB.h"
#include "Misc.h"

// Different types of bodies
typedef enum jgBodyType
{
     JG_BLANK_BODY,
     JG_STATIC_BODY,
     JG_SPRING_BODY,
     JG_PRESSURE_BODY
} jgBodyType;

typedef struct jgBody
{
     jgBodyType type;

     void (*destroy)(struct jgBody *body);
     void (*accumulateForces)(struct jgBody *body);

     jgParticle         *pointMasses;
     jgVector2          *baseShape;
     jgVector2          *globalShape;
     int                 numOfPoints;

     float               elasticity;
     float               friction;
     jgBitmask           layers;

     bool                isStatic;
     bool                isActive;
     bool                isKinematic;

     float               edgeSpringK;
     float               edgeSpringDamp;

     bool                isShapeMatching;
     float               shapeSpringK;
     float               shapeSpringDamp;

     jgVector2           derivedPosition;
     jgVector2           derivedVelocity;
     float               derivedAngle;
     float               lastAngle;     // TODO: Is this neccessary?
     float               derivedOmega;
     jgVector2           scale;

     jgAABB              aabb;

     jgBitmask           bitmaskX;
     jgBitmask           bitmaskY;
} jgBody;

extern jgBody  *jgBodyAlloc();
extern jgBody  *jgBodyInit(jgBody       *body,
                           jgBodyType    type,
                           jgVector2     points[],
                           int           numOfPoints,
                           float         massPerPoint,
                           jgVector2     position,
                           float         angle,
                           jgVector2     scale,
                           bool          isKinematic);
extern jgBody  *jgBodyNew(jgVector2     points[],
                          int           numOfPoints,
                          float         massPerPoint,
                          jgVector2     position,
                          float         angle,
                          jgVector2     scale,
                          bool          isKinematic);

extern void             jgBodyFree(jgBody *body);

extern void             jgBodyAccumulateForces(jgBody *body);
extern void             jgBodyDerive(jgBody *body);
extern void             jgBodyIntegrate(jgBody *body, float elapsed);
extern void             jgBodyUpdateGlobalShape(jgBody *body);

extern void             jgBodyDampenVelocity(jgBody *body, float damp);
extern void             jgBodyAddForce(jgBody *body, jgVector2 force);
extern void             jgBodyAddGlobalForce(jgBody *body, jgVector2 point, jgVector2 force);

// For convenience
extern jgBody  *jgStaticBodyAlloc();
extern jgBody  *jgStaticBodyInit(jgBody *body,
				 jgVector2       points[],
				 int             numOfPoints,
				 jgVector2       position,
				 float           angle,
				 jgVector2       scale);
extern jgBody  *jgStaticBodyNew(jgVector2       points[],
                                int             numOfPoints,
                                jgVector2       position,
                                float           angle,
                                jgVector2       scale);

#endif
