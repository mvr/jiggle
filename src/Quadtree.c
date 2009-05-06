#include <stdlib.h>

#include "jiggle.h"

static jgQuadtree *jgQuadtreeBuild(jgList *areas, int depth, jgAABB bounds)
{
     jgQuadtree *tree = malloc(sizeof(jgQuadtree));
     
     tree->ne = tree->nw = tree->se = tree->sw = NULL;
     
     depth--;
     if(depth == 0)
     {
          tree->items = jgListDuplicate(areas);
          return tree;
     }
          
     jgVector2 center = jgAABBCenter(bounds);
     tree->center = center;
     
     tree->items = jgListNew();

     jgList *ne_items = jgListNew();
     jgList *nw_items = jgListNew();
     jgList *se_items = jgListNew();
     jgList *sw_items = jgListNew();
     
     jgArea *currentArea;
     JG_LIST_FOREACH(areas, currentArea)
     {
          jgAABB areaBounds = currentArea->aabb;

          bool in_nw = areaBounds.min.x <= center.x && areaBounds.min.y <= center.y;
          bool in_ne = areaBounds.max.x >= center.x && areaBounds.min.y <= center.y;
          bool in_sw = areaBounds.min.x <= center.x && areaBounds.max.y >= center.y;
          bool in_se = areaBounds.max.x >= center.x && areaBounds.max.y >= center.y;
          
          if(in_nw && in_ne && in_sw && in_se)
          {
               jgListAdd(tree->items, currentArea);
          } 
          else
          {
               if(in_nw) jgListAdd(nw_items, currentArea);
               if(in_ne) jgListAdd(ne_items, currentArea);
               if(in_sw) jgListAdd(sw_items, currentArea);
               if(in_se) jgListAdd(se_items, currentArea);
          }
     }

     if(nw_items->length != 0)
          tree->nw = jgQuadtreeBuild(nw_items, depth, 
                                     jgAABBNewFromFloat(bounds.min.x, bounds.min.y,
                                                        center.x,     center.y));
     if(ne_items->length != 0)
          tree->ne = jgQuadtreeBuild(ne_items, depth, 
                                     jgAABBNewFromFloat(center.x,     bounds.min.y,
                                                        bounds.max.x, center.y));
     if(sw_items->length != 0)
          tree->sw = jgQuadtreeBuild(sw_items, depth, 
                                     jgAABBNewFromFloat(bounds.min.x, center.y,
                                                        center.x,     bounds.max.y));
     if(se_items->length != 0)
          tree->se = jgQuadtreeBuild(se_items, depth, 
                                     jgAABBNewFromFloat(center.x,     center.y,
                                                        bounds.max.x, bounds.max.y));

     jgListFree(nw_items);
     jgListFree(ne_items);
     jgListFree(sw_items);
     jgListFree(se_items);

     return tree;
}

jgQuadtree *jgQuadtreeNew(jgList *areas) 
{
     jgAABB bounds = jgAABBNull();

     jgArea *currentArea;
     JG_LIST_FOREACH(areas, currentArea)
     {
          bounds = jgAABBCombine(bounds, currentArea->aabb);
     }
     return jgQuadtreeBuild(areas, 8, bounds);
}

void jgQuadtreeFree(jgQuadtree *tree)
{
     jgQuadtreeFree(tree->nw);
     jgQuadtreeFree(tree->ne);
     jgQuadtreeFree(tree->sw);
     jgQuadtreeFree(tree->se);
     jgListFree(tree->items);
     free(tree);
}

jgList *jgQuadtreeCandidates(jgQuadtree *tree, jgVector2 point)
{
     jgList *results = jgListDuplicate(tree->items);
     jgVector2 center = tree->center;
     
     jgList *subitems = NULL;

     if(tree->nw && point.x <= center.x && point.y <= center.y)
          subitems = jgQuadtreeCandidates(tree->nw, point);
     if(tree->sw && point.x <= center.x && point.y >= center.y)
          subitems = jgQuadtreeCandidates(tree->sw, point);
     if(tree->ne && point.x >= center.x && point.y <= center.y)
          subitems = jgQuadtreeCandidates(tree->ne, point);
     if(tree->se && point.x >= center.x && point.y >= center.y)
          subitems = jgQuadtreeCandidates(tree->se, point);

     jgListAppend(results, subitems);
     jgListFree(subitems);

     return results;
}
