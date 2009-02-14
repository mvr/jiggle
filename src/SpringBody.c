#include <stdlib.h>
#include <stdio.h>

#include "SpringBody.h"
#include "Spring.h"
#include "Body.h"
#include "List.h"

static void jgSpringBodyDestroy(jgBody *body)
{
     jgSpringBody *springbody = (jgSpringBody *)body;

     jgSpring *currentSpring;
     JG_LIST_FOREACH(springbody->springs, currentSpring)
     {
          jgSpringFree(currentSpring);
     }

     jgListFree(springbody->springs);
}

static void jgSpringBodyAccumulateForces(jgBody *body)
{
     jgSpringBody *springbody = (jgSpringBody *)body;

     jgSpring *currentSpring;
     JG_LIST_FOREACH(springbody->springs, currentSpring)
     {
          jgSpringExert(currentSpring);
     }
}

void jgSpringBodyAddSpring(jgBody *body, int springA, int springB, int k, int damp)
{
     jgSpringBody *springbody = (jgSpringBody *)body;

     jgPointMass *a = &body->pointMasses[springA];
     jgPointMass *b = &body->pointMasses[springB];
     float dist = jgVector2DistanceBetween(a->position, b->position);

     jgSpring *spring = jgSpringNew(a, b, dist, k, damp);
     jgListAdd(springbody->springs, spring);
}

void jgSpringBodyClearSprings(jgBody *body)
{
     jgSpringBody *springbody = (jgSpringBody *)body;

     jgSpring *currentSpring;
     JG_LIST_FOREACH(springbody->springs, currentSpring)
     {
          jgSpringFree(currentSpring);
     }
     jgListClear(springbody->springs);
}

jgBody *jgSpringBodyAlloc()
{
     jgBody *body = malloc(sizeof(jgSpringBody));
     jgSpringBody *springbody = (jgSpringBody *)body;
     springbody->springs = jgListNew();
     return body;
}

jgBody *jgSpringBodyInit(jgBody          *body,
                         jgVector2        points[],
                         int              numOfPoints,
                         float            massPerPoint,
                         jgVector2        pos,
                         float            angle,
                         jgVector2        scale,
                         bool             isKinematic)
{
     jgBodyInit(body,
                JG_SPRING_BODY,
                points,
                numOfPoints,
                massPerPoint,
                pos,
                angle,
                scale,
                isKinematic);

     body->destroy = jgSpringBodyDestroy;
     body->accumulateForces = jgSpringBodyAccumulateForces;

     return body;
}


jgBody *jgSpringBodyNew(jgVector2 points[],
                        int numOfPoints,
                        float massPerPoint,
                        jgVector2 pos,
                        float angle,
                        jgVector2 scale,
                        bool isKinematic)
{
     return jgSpringBodyInit(jgSpringBodyAlloc(),
                             points,
                             numOfPoints,
                             massPerPoint,
                             pos,
                             angle,
                             scale,
                             isKinematic);

}
