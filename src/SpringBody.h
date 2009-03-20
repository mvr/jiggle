/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_SPRINGBODY_H__
#define __JG_SPRINGBODY_H__

#include <stdbool.h>

#include "Body.h"
#include "Spring.h"
#include "List.h"

typedef struct jgSpringBody
{
     jgBody body;

     jgList *springs;
} jgSpringBody;

extern jgBody *jgSpringBodyAlloc();
extern jgBody *jgSpringBodyInit(jgBody          *body,
                                jgVector2        points[],
                                int              numOfPoints,
                                float            massPerPoint,
                                jgVector2        pos,
                                float            angle,
                                jgVector2        scale,
                                bool             isKinematic);
extern jgBody *jgSpringBodyNew(jgVector2        points[],
                               int              numOfPoints,
                               float            massPerPoint,
                               jgVector2        pos,
                               float            angle,
                               jgVector2        scale,
                               bool             isKinematic);
extern void jgSpringBodyAddSpring(jgBody *body, int springA, int springB, int k, int damp);
extern void jgSpringBodyClearSprings(jgBody *body);

#endif

