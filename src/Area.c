#include <stdlib.h>

#include "jiggle.h"

jgArea *jgAreaAlloc()
{
     return malloc(sizeof(jgArea));
}

jgArea *jgAreaInit(jgArea *area, jgParticle **particles, int numOfParticles)
{
     area->particles = jgListNewFromArray((void **)particles, numOfParticles);
     area->baseShape = malloc(numOfParticles * sizeof(jgParticle));

     jgVector2 center;

     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgListAdd(currentParticle->ownerAreas, area);
          center = jgVector2Add(center, currentParticle->position);
     }
     
     center = jgVector2Divide(center, numOfParticles);

     // Recenter the 'baseShape' so its position is zero.
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          area->baseShape[_i_] = jgVector2Subtract(currentParticle->position, center);
     }

     area->isShapeMatching = false;
     area->shapeStrength = 20;
     area->shapeDamping = 5;

     area->isKinematic = false;

     jgAreaDerive(area);

     jgAreaUpdateAABB(area, 0.0);
     jgAreaUpdateCenterOfMass(area);

     return area;
}

jgArea *jgAreaNew(jgParticle **particles, int numOfParticles)
{
     return jgAreaInit(jgAreaAlloc(), particles, numOfParticles);
}

void jgAreaFree(jgArea *area)
{
     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgListRemove(currentParticle->ownerAreas, area);
     }

     jgListFree(area->particles);
     free(area->baseShape);
     free(area);
}

void jgAreaUpdateAABB(jgArea *area, float elapsed)
{
     area->aabb = jgAABBNull();

     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgVector2 p = currentParticle->position;
          area->aabb = jgAABBExpandToInclude(area->aabb, p);

//          p = jgVector2Add(p, jgVector2Multiply(currentParticle->velocity, elapsed));
//          area->aabb = jgAABBExpandToInclude(area->aabb, p);
     }
}

bool jgAreaContains(jgArea *area, jgVector2 point)
{
     if(!jgAABBContains(area->aabb, point))
          return false;
     
     // Draw a line from the point to a point known to be outside the area.
     // Count the number of lines in the polygon it intersects. If that
     // number is odd, we are inside.  If it's even, we are outside.

     // out is guarenteed to be outside the shape.
     jgVector2 out = jgVector2Add(area->aabb.max, jgv(0, 1));

     int inside = false;
     jgParticle *currentParticle;
     jgParticle *nextParticle;
     JG_LIST_FOREACH_PAIR(area->particles, currentParticle, nextParticle)
     {
          jgVector2 start = currentParticle->position;
          jgVector2 end = nextParticle->position;

          if(jgVector2Intersect(point, out, start, end))
               inside = !inside;
     }

     return inside;
}

void jgAreaDerive(jgArea *area)
{
     if(area->isKinematic) 
          return;

     jgVector2 center   = jgVector2Zero();
     jgVector2 velocity = jgVector2Zero();

     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          center   = jgVector2Add(center,   currentParticle->position);
          velocity = jgVector2Add(velocity, currentParticle->velocity);
     }

     center   = jgVector2Divide(center,   area->particles->length);
     velocity = jgVector2Divide(velocity, area->particles->length);

     area->derivedPosition = center;
     area->derivedVelocity = velocity;

     float angle = 0;
     int originalSign = 0;
     float originalAngle = 0;
     const float PI = 3.14159265;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          float thisAngle = jgVector2AngleBetween(area->baseShape[_i_],
                                                  jgVector2Subtract(currentParticle->position, center));

          if (_i_ == 0)
          {
               originalSign = (thisAngle >= 0.0f) ? 1 : -1;
               originalAngle = thisAngle;
          }
          else
          {
               float diff = (thisAngle - originalAngle);
               int thisSign = (thisAngle >= 0) ? 1 : -1;

               if (abs(diff) > PI && thisSign != originalSign)
               {
                    thisAngle = (thisSign == -1) ? (thisAngle + 2 * PI) : (thisAngle - 2 * PI);
               }
          }

          angle += thisAngle;
     }

     area->derivedAngle = angle / area->particles->length;
}

void jgAreaMatchShape(jgArea *area)
{
     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgVector2 rigidLocation = area->baseShape[_i_];
          rigidLocation = jgVector2Rotate(rigidLocation, area->derivedAngle);
          rigidLocation = jgVector2Add(rigidLocation, area->derivedPosition);

          jgSpringDragTowards(currentParticle,
                              rigidLocation,
                              0,
                              area->shapeStrength,
                              area->shapeDamping);
     }
}

float jgAreaArea(jgArea *area)
{
     float sum = 0;
     jgParticle *currentParticle;
     jgParticle *nextParticle;
     JG_LIST_FOREACH_PAIR(area->particles, currentParticle, nextParticle)
     {
          jgVector2 current = currentParticle->position;
          jgVector2 next = nextParticle->position;

          sum += (current.x * next.y) - (next.x * current.y);
     }
     return -sum / 2;
}

bool jgAreaIsInsideOut(jgArea *area)
{
     return jgAreaArea(area) < 0;
}

void jgAreaUpdateCenterOfMass(jgArea *area)
{
     float totalMass = 0;
     jgVector2 center = jgv(0, 0);

     jgParticle *particle;
     JG_LIST_FOREACH(area->particles, particle)
     {
          totalMass += particle->mass;
          center = jgVector2Add(center, jgVector2Multiply(particle->position, particle->mass));
     }
     
     center = jgVector2Divide(center, totalMass);

     area->centerOfMass = center;
}
