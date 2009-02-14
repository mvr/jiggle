#include <stdlib.h>
#include <math.h>

#include "PointMass.h"
#include "Vector2.h"

jgPointMass jgPointMassNew(float mass, jgVector2 pos)
{
     return (jgPointMass){mass, pos, jgVector2Zero(), jgVector2Zero()};
}

void jgPointMassIntegrate(jgPointMass *point, float elapsed)
{
     if(point->mass != INFINITY)
     {
          float elapMass = elapsed / point->mass;
          point->velocity = jgVector2Add(point->velocity, jgVector2Multiply(point->force, elapMass));
          point->position = jgVector2Add(point->position, jgVector2Multiply(point->velocity, elapsed));
     }
     point->force = jgVector2Zero();
}

