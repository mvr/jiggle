#include <stdlib.h>
#include <stdio.h>

#include "jiggle.h"

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

jgSpring *jgSpringAlloc()
{
     return malloc(sizeof(jgSpring));
}

jgSpring *jgSpringInit(jgSpring *spring,
                       jgParticle *a,
                       jgParticle *b,
                       float d,
                       float k,
                       float damp)
{
     spring->particleA = a;
     spring->particleB = b;
     spring->springD = d;
     spring->springK = k;
     spring->damping = damp;
     return spring;
}

jgSpring *jgSpringNew(jgParticle *a,
                      jgParticle *b,
                      float d,
                      float k,
                      float damp)
{
     return jgSpringInit(jgSpringAlloc(), a, b, d, k, damp);
}

void jgSpringFree(jgSpring *spring)
{
     free(spring);
}

void jgSpringExert(jgSpring *spring)
{
     jgSpringDragTogether(spring->particleA,
                          spring->particleB,
                          spring->springD,
                          spring->springK,
                          spring->damping);
}

void jgSpringDragTowards(jgParticle *particle, jgVector2 point, float d, float k, float damp)
{
     jgVector2 force = jgSpringCalculateForce(particle->position,
                                              jgParticleEffectiveVelocity(particle),
                                              point,
                                              jgv(0, 0),
                                              d,
                                              k,
                                              damp);

     particle->force = jgVector2Add(particle->force, force);
}

void jgSpringDragTogether(jgParticle *particleA, jgParticle *particleB, float d, float k, float damp)
{
     jgVector2 force = jgSpringCalculateForce(particleA->position,
                                              jgParticleEffectiveVelocity(particleA),
                                              particleB->position,
                                              jgParticleEffectiveVelocity(particleB),
                                              d,
                                              k,
                                              damp);

     particleA->force = jgVector2Add     (particleA->force, force);
     particleB->force = jgVector2Subtract(particleB->force, force);
}
