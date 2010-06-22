#ifndef __JG_VECTOR2_H__
#define __JG_VECTOR2_H__

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

typedef struct jgVector2
{
     float x, y;
} jgVector2;

static inline jgVector2 jgVector2New(float x, float y);
static inline jgVector2 jgv(float x, float y);
static inline jgVector2 jgVector2Zero();
static inline jgVector2 jgVector2One();

//// Math

static inline jgVector2 jgVector2Negative(jgVector2 a);
static inline jgVector2 jgVector2Add(jgVector2 a, jgVector2 b);
static inline jgVector2 jgVector2Subtract(jgVector2 a, jgVector2 b);
static inline jgVector2 jgVector2Multiply(jgVector2 a, float scale);
static inline jgVector2 jgVector2Divide(jgVector2 a, float scale);
static inline float     jgVector2Dot(jgVector2 a, jgVector2 b);
static inline float     jgVector2Cross(jgVector2 a, jgVector2 b);
static inline jgVector2 jgVector2Scale(jgVector2 a, jgVector2 b);

//// Transformations

static inline jgVector2 jgVector2Perpendicular(jgVector2 a);
static inline jgVector2 jgVector2Rotate(jgVector2 a, float angle);
static inline jgVector2 jgVector2Reflect(jgVector2 a, jgVector2 n);
static inline float     jgVector2Length(jgVector2 a);
static inline jgVector2 jgVector2Normalize(jgVector2 a);
static inline jgVector2 jgVector2Normal(jgVector2 a1, jgVector2 a2);
static inline jgVector2 jgVector2ThreePointNormal(jgVector2 prev, jgVector2 current, jgVector2 next);

//// Misc

static inline float     jgVector2DistanceBetween(jgVector2 a, jgVector2 b);
static inline float     jgVector2AngleBetween(jgVector2 a, jgVector2 b);
static inline bool      jgVector2Equal(jgVector2 a, jgVector2 b);
static inline bool      jgVector2CCW(jgVector2 a, jgVector2 b);
static inline bool      jgVector2CW(jgVector2 a, jgVector2 b);

//// Lines

extern bool             jgVector2Intersect(jgVector2 a1, jgVector2 a2, jgVector2 b1, jgVector2 b2);
extern jgVector2        jgVector2ClosestPointOnLine(jgVector2 a1, jgVector2 a2, jgVector2 pt);
extern float            jgVector2PositionAlong(jgVector2 a1, jgVector2 a2, jgVector2 pt);
static inline jgVector2 jgVector2Towards(jgVector2 point, jgVector2 target, float amount);
extern jgVector2        jgVector2Normal(jgVector2 a1, jgVector2 a2);
extern jgVector2        jgVector2ThreePointNormal(jgVector2 prev, jgVector2 current, jgVector2 next);

#include "Vector2-inl.h"

#endif
