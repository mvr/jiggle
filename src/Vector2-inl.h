/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __VECTOR2_INL_H__
#define __VECTOR2_INL_H__


static inline jgVector2 jgVector2New(float x, float y)
{
     return (jgVector2){x, y};
}

static inline jgVector2 jgv(float x, float y)
{
     return (jgVector2){x, y};
}

static inline jgVector2 jgVector2Zero()
{
     return jgVector2New(0.0, 0.0);
}

static inline jgVector2 jgVector2One()
{
     return jgVector2New(1.0, 1.0);
}

//// Math

static inline jgVector2 jgVector2Negative(jgVector2 a)
{
     return jgVector2New(-a.x, -a.y);
}

static inline jgVector2 jgVector2Add(jgVector2 a, jgVector2 b)
{
     return jgVector2New(a.x + b.x, a.y + b.y);
}

static inline jgVector2 jgVector2Subtract(jgVector2 a, jgVector2 b)
{
     return jgVector2Add(a, jgVector2Negative(b));
}

static inline jgVector2 jgVector2Multiply(jgVector2 a, float scale)
{
     return jgVector2New(a.x * scale, a.y * scale);
}

static inline jgVector2 jgVector2Divide(jgVector2 a, float scale)
{
     return jgVector2Multiply(a, 1 / scale);
}

static inline float jgVector2Dot(jgVector2 a, jgVector2 b)
{
     return (a.x * b.x) + (a.y * b.y);
}

static inline float jgVector2Cross(jgVector2 a, jgVector2 b)
{
     return (a.x * b.y) - (a.y * b.x);
}

static inline jgVector2 jgVector2Scale(jgVector2 a, jgVector2 b)
{
     return jgVector2New(a.x * b.x, a.y * b.y);
}

//// Transformations

static inline jgVector2 jgVector2Perpendicular(jgVector2 a)
{
     return jgVector2New(-a.y, a.x);
}

static inline jgVector2 jgVector2Rotate(jgVector2 a, float angle)
{
     float s = sin(angle);
     float c = cos(angle);
     return jgVector2New((c * a.x) - (s * a.y), (c * a.y) + (s * a.x));
}

static inline jgVector2 jgVector2Reflect(jgVector2 a, jgVector2 n)
{
     return jgVector2Subtract(a, jgVector2Multiply(n,
                                                   2.0 * jgVector2Dot(a, n)));
}

static inline float jgVector2Length(jgVector2 a)
{
     return sqrt(a.x * a.x + a.y * a.y);
}

static inline jgVector2 jgVector2Normalize(jgVector2 a)
{
     float l = jgVector2Length(a);
     if(l == 0)
          return jgVector2Zero();
     return jgVector2Divide(a, l);
}

static inline jgVector2 jgVector2Normal(jgVector2 a1, jgVector2 a2)
{
     return jgVector2Perpendicular(jgVector2Normalize(jgVector2Subtract(a2, a1)));
}

static inline jgVector2 jgVector2ThreePointNormal(jgVector2 prev, jgVector2 current, jgVector2 next)
{
     return jgVector2Normalize(jgVector2Perpendicular(jgVector2Add(jgVector2Subtract(current, prev),
                                                                   jgVector2Subtract(next, current))));
}

//// Misc

static inline float jgVector2DistanceBetween(jgVector2 a, jgVector2 b)
{
     return jgVector2Length(jgVector2Subtract(b, a));
}

static inline float jgVector2AngleBetween(jgVector2 a, jgVector2 b)
{
     return atan2(b.y, b.x) - atan2(a.y, a.x);
}

static inline bool jgVector2Equal(jgVector2 a, jgVector2 b)
{
     return (a.x == b.x) && (a.y == b.y);
}

static inline bool jgVector2CCW(jgVector2 a, jgVector2 b)
{
     return jgVector2Dot(b, jgVector2Perpendicular(a)) >= 0.0f;
}

static inline bool jgVector2CW(jgVector2 a, jgVector2 b)
{
     return !jgVector2CCW(a, b);
}

#endif
