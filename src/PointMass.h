/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __POINTMASS_H__
#define __POINTMASS_H__

#include "Vector2.h"

typedef struct jgPointMass
{
     float     mass;
     jgVector2 position;
     jgVector2 velocity;
     jgVector2 force;
} jgPointMass;

extern jgPointMass       jgPointMassNew(float mass, jgVector2 pos);
extern void              jgPointMassIntegrate(jgPointMass *point, float elapsed);

#endif
