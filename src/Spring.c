#include <stdlib.h>
#include <stdio.h>

#include "Spring.h"
#include "PointMass.h"
#include "Vector2.h"

static jgVector2 jgSpringCalculateForce(jgVector2 posA,
                                        jgVector2 velA,
                                        jgVector2 posB,
                                        jgVector2 velB,
                                        float springD,
                                        float springK,
                                        float damping)
{
     // I don't get this bit, I'm just copying
     // Something to do with Hooke's law
     jgVector2 BtoA = jgVector2Subtract(posA, posB);
     float dist = springD - jgVector2Length(BtoA);
     BtoA = jgVector2Normalize(BtoA);

     jgVector2 relVel = jgVector2Subtract(velA, velB);
     float totalRelVel = jgVector2Dot(relVel, BtoA);

     return jgVector2Multiply(BtoA, (dist * springK) - (totalRelVel * damping));
}

jgSpring *jgSpringNew(jgPointMass *a,
                      jgPointMass *b,
                      float d,
                      float k,
                      float damp)
{
     jgSpring *ret = malloc(sizeof(jgSpring));
     ret->massA = a;
     ret->massB = b;
     ret->springD = d;
     ret->springK = k;
     ret->damping = damp;
     return ret;
}

void jgSpringFree(jgSpring *spring)
{
     free(spring);
}

void jgSpringExert(jgSpring *spring)
{
     jgSpringDragTogether(spring->massA,
                          spring->massB,
                          spring->springD,
                          spring->springK,
                          spring->damping);
}

void jgSpringDragTowards(jgPointMass *mass, jgVector2 point, float d, float k, float damp)
{
     jgVector2 force = jgSpringCalculateForce(mass->position,
                                              mass->velocity,
                                              point,
                                              mass->velocity,
                                              d,
                                              k,
                                              damp);

     mass->force = jgVector2Add(mass->force, force);
}

void jgSpringDragTogether(jgPointMass *massA, jgPointMass *massB, float d, float k, float damp)
{
     jgVector2 force = jgSpringCalculateForce(massA->position,
                                              massA->velocity,
                                              massB->position,
                                              massB->velocity,
                                              d,
                                              k,
                                              damp);

     massA->force = jgVector2Add     (massA->force, force);
     massB->force = jgVector2Subtract(massB->force, force);
}
