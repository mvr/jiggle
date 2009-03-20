/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __JG_MISC_H__
#define __JG_MISC_H__

#define PI 3.14159265358979323846

typedef unsigned int jgBitmask;

#define JG_BITMASK_CLEAR(bitmask) bitmask = 0;
#define JG_BITMASK_SET_ON(bitmask, bit) bitmask |= (1 << (bit))
#define JG_BITMASK_SET_OFF(bitmask, bit) bitmask &= ~(1 << (bit))
#define JG_BITMASK_GET(bitmask, bit) (((bitmask) & (1 << (bit))) != 0)

#endif
