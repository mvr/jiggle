#include <stdlib.h>

#include "jiggle.h"

jgCollision *jgCollisionAlloc()
{
     return malloc(sizeof(jgCollision));
}

void jgCollisionFree(jgCollision *collision)
{
     free(collision);
}
