#include <stdlib.h>
#include <math.h>

#include "PressureBody.h"

#include "Body.h"

static void jgPressureBodyAccumulateForces(jgBody *body)
{
     jgVector2 *norms = calloc(body->numOfPoints, sizeof(jgVector2));

     float volume = 0;

     for(int i = 0; i < body->numOfPoints; i++)
     {
          jgVector2 prev    = body->pointMasses[i == 0 ? body->numOfPoints - 1 : i - 1].position;
          jgVector2 current = body->pointMasses[i].position;
          jgVector2 next    = body->pointMasses[i == body->numOfPoints - 1 ? 0 : i + 1].position;

          float edgeLength = jgVector2DistanceBetween(next, prev);
          float xLength = fabs(current.x - next.x);

          jgVector2 norm = jgVector2ThreePointNormal(prev, current, next);

          norms[i] = norm;

          volume += (xLength * fabs(norm.x) * edgeLength) / 2.0;
     }

     if(volume != 0)
     {
          for(int i = 0; i < body->numOfPoints; i++)
          {
               int j = (i + 1) % body->numOfPoints;

               float pressure = (1.0 / volume) *
                    jgVector2DistanceBetween(body->pointMasses[i].position, body->pointMasses[j].position) *
                    ((jgPressureBody *)body)->gasAmount;

               body->pointMasses[i].force = jgVector2Add(body->pointMasses[i].force, jgVector2Multiply(norms[i], pressure));
               body->pointMasses[j].force = jgVector2Add(body->pointMasses[j].force, jgVector2Multiply(norms[j], pressure));
          }
     }

     free(norms);
}

jgBody *jgPressureBodyAlloc()
{
     return malloc(sizeof(jgPressureBody));
}

jgBody *jgPressureBodyInit(jgBody *body,
                           jgVector2        points[],
                           int              numOfPoints,
                           float            massPerPoint,
                           jgVector2        pos,
                           float            angle,
                           jgVector2        scale,
                           bool             isKinematic)
{
     jgPressureBody *pressurebody = (jgPressureBody *)body;

     jgBodyInit(body,
                JG_PRESSURE_BODY,
                points,
                numOfPoints,
                massPerPoint,
                pos,
                angle,
                scale,
                isKinematic);

     // Default
     body->isShapeMatching = false;
     body->shapeSpringK = 50;
     body->shapeSpringDamp = 10;
     pressurebody->gasAmount = 100;

     body->accumulateForces = jgPressureBodyAccumulateForces;

     return body;
}

jgBody *jgPressureBodyNew(jgVector2 points[],
                        int numOfPoints,
                        float massPerPoint,
                        jgVector2 pos,
                        float angle,
                        jgVector2 scale,
                        bool isKinematic)
{
     return jgPressureBodyInit(jgPressureBodyAlloc(),
                               points,
                               numOfPoints,
                               massPerPoint,
                               pos,
                               angle,
                               scale,
                               isKinematic);
}
