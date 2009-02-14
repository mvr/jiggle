#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Body.h"
#include "PointMass.h"
#include "Spring.h"
#include "Misc.h"

jgBody *jgBodyAlloc()
{
     return (jgBody *)malloc(sizeof(jgBody));
}

jgBody *jgBodyInit(jgBody       *body,
                   jgBodyType    type,
                   jgVector2     points[],
                   int           numOfPoints,
                   float         massPerPoint,
                   jgVector2     position,
                   float         angle,
                   jgVector2     scale,
                   bool          isKinematic)
{
     body->type            = type;

     body->derivedPosition = position;
     body->derivedAngle    = angle;
     body->lastAngle       = angle;
     body->scale           = scale;
     body->isStatic        = massPerPoint == INFINITY;
     body->isKinematic     = isKinematic;
     body->isActive        = true;
     body->aabb            = jgAABBNull();

     body->destroy = NULL;
     body->accumulateForces = NULL;

     body->numOfPoints = numOfPoints;
     body->baseShape   = calloc(numOfPoints, sizeof(jgVector2));
     body->pointMasses = calloc(numOfPoints, sizeof(jgPointMass));

     for(int i = 0; i < numOfPoints; i++)
     {
          body->baseShape[i] = points[i];
     }

     body->globalShape = NULL;
     jgBodyUpdateGlobalShape(body);

     for(int i = 0; i < numOfPoints; i++)
     {
          body->pointMasses[i] = jgPointMassNew(massPerPoint, body->globalShape[i]);
     }

     body->edgeSpringK = 300;
     body->edgeSpringDamp = 10;

     body->layers = 0xFFFF;

     body->friction  = 0.8;
     body->elasticity = 0.3;

     body->isShapeMatching = isKinematic;
     // But give some sensible defaults
     body->shapeSpringK = 15;
     body->shapeSpringDamp = 5;

     jgBodyUpdateAABB(body, 0.0, true);

     JG_BITMASK_CLEAR(body->bitmaskX);
     JG_BITMASK_CLEAR(body->bitmaskY);

     jgBodyDerive(body);

     return body;
}

jgBody *jgBodyNew(jgVector2     points[],
                  int           numOfPoints,
                  float         massPerPoint,
                  jgVector2     position,
                  float         angle,
                  jgVector2     scale,
                  bool          isKinematic)
{
     return jgBodyInit(jgBodyAlloc(),
                       JG_BLANK_BODY,
                       points,
                       numOfPoints,
                       massPerPoint,
                       position,
                       angle,
                       scale,
                       isKinematic);
}

void jgBodyFree(jgBody *body)
{
     if(body->destroy) body->destroy(body);

     free(body->baseShape);
     free(body->globalShape);
     free(body->pointMasses);
     free(body);
}

void jgBodyDerive(jgBody *body)
{
     // No sense in deriving a static body
     if(body->isStatic || body->isKinematic)
          return;

     jgVector2 center   = jgVector2Zero();
     jgVector2 velocity = jgVector2Zero();

     for(int i = 0; i < body->numOfPoints; i++)
     {
          center   = jgVector2Add(center,   body->pointMasses[i].position);
          velocity = jgVector2Add(velocity, body->pointMasses[i].velocity);
     }

     center   = jgVector2Divide(center,   body->numOfPoints);
     velocity = jgVector2Divide(velocity, body->numOfPoints);

     body->derivedPosition = center;
     body->derivedVelocity = velocity;

     float angle = 0;
     int originalSign = 0;
     float originalAngle = 0;
     for(int i = 0; i < body->numOfPoints; i++)
     {

          float thisAngle = jgVector2AngleBetween(body->baseShape[i],
                                                  jgVector2Subtract(body->pointMasses[i].position, body->derivedPosition));

          if (i == 0)
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

     body->derivedAngle = angle / body->numOfPoints;
}

void jgBodyIntegrate(jgBody *body, float elapsed)
{
     if(body->isStatic)
          return;

     for(int i = 0; i < body->numOfPoints; i++)
          jgPointMassIntegrate(&body->pointMasses[i], elapsed);
}

void jgBodyUpdateAABB(jgBody *body, float elapsed, bool force)
{
     if(!force && body->isStatic)
          return;

     body->aabb = jgAABBNull();
     for (int i = 0; i < body->numOfPoints; i++)
     {
          jgVector2 p = body->pointMasses[i].position;
          body->aabb = jgAABBExpandToInclude(body->aabb, p);

          // Expanding for velocity only makes sense for dynamic objects.
          if (!body->isStatic)
          {
               p = jgVector2Add(p, jgVector2Multiply(body->pointMasses[i].velocity, elapsed));
               body->aabb = jgAABBExpandToInclude(body->aabb, p);
          }
     }
}

void jgBodyUpdateGlobalShape(jgBody *body)
{
     free(body->globalShape);
     body->globalShape = jgVector2Transform(body->baseShape,
                                            body->numOfPoints,
                                            body->derivedPosition,
                                            body->derivedAngle,
                                            body->scale);
}

bool jgBodyContains(jgBody *body, jgVector2 point)
{
     // Draw a line from the point to a point known to be outside the body.
     // Count the number of lines in the polygon it intersects. If that
     // number is odd, we are inside.  If it's even, we are outside.

     // out is guarenteed to be outside the shape.
     jgVector2 out = jgVector2Add(body->aabb.max, jgVector2One());

     int inside = false;
     for(int i = 0; i < body->numOfPoints; i++)
     {
          jgVector2 start = body->pointMasses[i].position;
          jgVector2 end;
          if(i == body->numOfPoints - 1)
               end = body->pointMasses[0].position;
          else
               end = body->pointMasses[i + 1].position;

          if(jgVector2Intersect(point, out, start, end))
               inside = !inside;
     }

     return inside;
}

jgVector2 jgBodyClosestOnEdge(jgBody *body, jgVector2 pt, jgVector2 normal,
                              int *Aout, int *Bout, float *Dout, jgVector2 *normout)
{
     float sameDist, awayDist;
     int sameA, awayA;
     int sameB, awayB;
     jgVector2 sameClosest, awayClosest;

     sameDist = awayDist = INFINITY;
     sameA = sameB = awayA = awayB = 0;
     sameClosest = awayClosest = jgVector2Zero();

     bool awayFound = false;

     for (int i = 0; i < body->numOfPoints; i++)
     {
          int j;
          if (i < body->numOfPoints - 1)
               j = i + 1;
          else
               j = 0;

          jgVector2 hitPt = jgVector2ClosestPointOnLine(body->pointMasses[i].position,
                                                        body->pointMasses[j].position,
                                                        pt);
          float dist = jgVector2DistanceBetween(pt, hitPt);

          float dot = jgVector2Dot(normal, jgVector2Normal(body->pointMasses[i].position, body->pointMasses[j].position));
          if(dot <= 0)
          {
               if(dist < awayDist)
               {
                    awayDist = dist;
                    awayA = i;
                    awayB = j;
                    awayClosest = hitPt;
                    awayFound = true;
               }
          }
          else
          {
               if(dist < sameDist)
               {
                    sameDist = dist;
                    sameA = i;
                    sameB = j;
                    sameClosest = hitPt;
               }
          }
     }

     if(awayFound)
     {
          *Aout = awayA;
          *Bout = awayB;

          *Dout = jgVector2PositionAlong(body->pointMasses[awayA].position,
                                         body->pointMasses[awayB].position,
                                         awayClosest);
          *normout  = jgVector2Normal(body->pointMasses[awayA].position,
                                      body->pointMasses[awayB].position);
          return awayClosest;
     }
     else
     {
          *Aout = sameA;
          *Bout = sameB;

          *Dout = jgVector2PositionAlong(body->pointMasses[sameA].position,
                                         body->pointMasses[sameB].position,
                                         sameClosest);
          *normout  = jgVector2Normal(body->pointMasses[sameA].position,
                                      body->pointMasses[sameB].position);
          return sameClosest;
     }
}

float jgBodyArea(jgBody *body)
{
     float sum = 0;
     for(int i = 0; i < body->numOfPoints; i++)
     {
          jgVector2 current = body->pointMasses[i].position;
          jgVector2 next = body->pointMasses[i == body->numOfPoints - 1 ? 0 : i + 1].position;

          sum += (current.x * next.y) - (next.x * current.y);
     }
     return -(sum / 2);
}

bool jgBodyIsInsideOut(jgBody *body)
{
     return jgBodyArea(body) < 0;
}

void jgBodyAccumulateForces(jgBody *body)
{
     if(body->isStatic)
          return;

     for(int i = 0; i < body->numOfPoints; i++)
     {
          int next = (i + 1) % body->numOfPoints;
          float dist = jgVector2DistanceBetween(body->baseShape[i], body->baseShape[next]);
          jgSpringDragTogether(&body->pointMasses[i],
                               &body->pointMasses[next],
                               dist,
                               body->edgeSpringK,
                               body->edgeSpringDamp);
     }

     if(body->isShapeMatching)
     {
          jgBodyUpdateGlobalShape(body);
          for(int i = 0; i < body->numOfPoints; i++)
          {
               jgSpringDragTowards(&body->pointMasses[i],
                                   body->globalShape[i],
                                   0,
                                   body->shapeSpringK,
                                   body->shapeSpringDamp);
          }
     }

     if(body->accumulateForces) body->accumulateForces(body);
}

void jgBodyDampenVelocity(jgBody *body, float damp)
{
     if(body->isStatic)
          return;

     for(int i = 0; i < body->numOfPoints; i++)
          body->pointMasses[i].velocity = jgVector2Multiply(body->pointMasses[i].velocity, damp);
}

void jgBodyAddForce(jgBody *body, jgVector2 force)
{
     // Multiply mass to negate dividing by mass during integration.
     for(int i = 0; i < body->numOfPoints; i++)
          body->pointMasses[i].force = jgVector2Add(body->pointMasses[i].force,
                                                    jgVector2Multiply(force, body->pointMasses[i].mass));
}

void jgBodyAddGlobalForce(jgBody *body, jgVector2 point, jgVector2 force)
{
     jgVector2 R = jgVector2Subtract(body->derivedPosition, point);
     float torqueF = jgVector2Cross(R, force);

     for(int i = 0; i < body->numOfPoints; i++)
     {
          jgVector2 toPt = jgVector2Subtract(body->pointMasses[i].position, body->derivedPosition);
          jgVector2 torque = jgVector2Rotate(toPt, -(PI/2));

          body->pointMasses[i].force = jgVector2Add(body->pointMasses[i].force, jgVector2Multiply(torque, torqueF));
          body->pointMasses[i].force = jgVector2Add(body->pointMasses[i].force, force);
     }
}

jgBody *jgStaticBodyAlloc()
{
     return (jgBody *)malloc(sizeof(jgBody));
}

jgBody *jgStaticBodyInit(jgBody *body,
			 jgVector2 points[],
			 int numOfPoints,
			 jgVector2 position,
			 float angle,
			 jgVector2 scale)
{
     return jgBodyInit(body,
		       JG_STATIC_BODY,
		       points,
		       numOfPoints,
		       INFINITY,
		       position,
		       angle,
		       scale,
		       false);
}

jgBody *jgStaticBodyNew(jgVector2 points[],
                        int numOfPoints,
                        jgVector2 position,
                        float angle,
                        jgVector2 scale)
{
     return jgStaticBodyInit(jgStaticBodyAlloc(),
			     points,
			     numOfPoints,
			     position,
			     angle,
			     scale);
}
