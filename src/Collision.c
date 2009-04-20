#include <stdlib.h>

#include "Collision.h"

jgCollision *jgCollisionAlloc()
{
     return malloc(sizeof(jgCollision));
}

void jgCollisionFree(jgCollision *collision)
{
     free(collision);
}
