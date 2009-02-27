#include "Body.h"
#include "Particle.h"
#include "Spring.h"
#include "Misc.h"


void jgBodyUpdateAABB(jgBody *area, float elapsed)
{
     body->aabb = jgAABBNull();

     jgParticle *currentParticle;
     JG_LIST_FOREACH(area->particles, currentParticle)
     {
          jgVector2 p = currentParticle->position;
          body->aabb = jgAABBExpandToInclude(body->aabb, p);

          p = jgVector2Add(p, jgVector2Multiply(currentParticle->velocity, elapsed));
          body->aabb = jgAABBExpandToInclude(body->aabb, p);
     }
}

float jgAreaArea(jgBody *area)
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

