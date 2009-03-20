/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_PRESSUREBODY_C__
#define __JG_PRESSUREBODY_C__

#include "Body.h"

typedef struct jgPressureBody
{
     jgBody body;

     float gasAmount;
} jgPressureBody;

extern jgBody *jgPressureBodyAlloc();
extern jgBody *jgPressureBodyInit(jgBody *body,
                                  jgVector2        points[],
                                  int              numOfPoints,
                                  float            massPerPoint,
                                  jgVector2        pos,
                                  float            angle,
                                  jgVector2        scale,
                                  bool             isKinematic);
extern jgBody *jgPressureBodyNew(jgVector2        points[],
                                 int              numOfPoints,
                                 float            massPerPoint,
                                 jgVector2        pos,
                                 float            angle,
                                 jgVector2        scale,
                                 bool             isKinematic);

#endif
