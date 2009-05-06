/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_QUADTREE_H__
#define __JG_QUADTREE_H__

#include "jiggle.h"

typedef struct jgQuadtree
{
     jgVector2          center;
     struct jgQuadtree *ne, *nw, *se, *sw;
     jgList            *items;
} jgQuadtree;

extern jgQuadtree *jgQuadtreeNew(jgList *areas);
extern void        jgQuadtreeFree(jgQuadtree *tree);
extern jgList     *jgQuadtreeCandidates(jgQuadtree *tree, jgVector2 point);

#endif
