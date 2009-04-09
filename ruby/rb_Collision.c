#include "rb_jiggle.h"

VALUE c_jgCollision;

static VALUE rb_jgCollisionAlloc(VALUE klass)
{
     jgCollision *v = jgCollisionAlloc();
     return Data_Wrap_Struct(klass, NULL, jgCollisionFree, v);
}

VALUE rb_jgCollisionWrap(jgCollision *collision)
{     
     VALUE wrappedCollision = Data_Wrap_Struct(c_jgCollision, NULL, NULL, collision);
     
     rb_iv_set(wrappedCollision, "particle",        rb_jgParticleWrap(collision->particle));
     rb_iv_set(wrappedCollision, "area",            rb_jgAreaWrap(collision->area));
     rb_iv_set(wrappedCollision, "area_particle_a", rb_jgParticleWrap(collision->areaParticleA));
     rb_iv_set(wrappedCollision, "area_particle_b", rb_jgParticleWrap(collision->areaParticleB));
     
     return wrappedCollision;
}

static VALUE rb_jgCollisionGetParticle(VALUE self)
{
     return rb_iv_get(self, "particle");
}

static VALUE rb_jgCollisionGetArea(VALUE self)
{
     return rb_iv_get(self, "area");
}

static VALUE rb_jgCollisionGetAreaA(VALUE self)
{
     return rb_iv_get(self, "area_particle_a");
}

static VALUE rb_jgCollisionGetAreaB(VALUE self)
{
     return rb_iv_get(self, "area_particle_b");
}

VECTOR_GET(rb_jgCollisionGetHitPoint,    COLLISION, hitPt)
FLOAT_GET( rb_jgCollisionGetEdgeD,       COLLISION, edgeD)
VECTOR_GET(rb_jgCollisionGetNormal,      COLLISION, normal)
FLOAT_GET( rb_jgCollisionGetPenetration, COLLISION, penetration)

void Init_jgCollision()
{
     c_jgCollision = rb_define_class_under(m_Jiggle, "Collision", rb_cObject);
     rb_define_alloc_func(c_jgCollision, rb_jgCollisionAlloc);

     rb_define_method(c_jgCollision, "particle", rb_jgCollisionGetParticle, 0);
     rb_define_method(c_jgCollision, "area", rb_jgCollisionGetArea, 0);
     rb_define_method(c_jgCollision, "area_particle_a", rb_jgCollisionGetAreaA, 0);
     rb_define_method(c_jgCollision, "area_particle_b", rb_jgCollisionGetAreaB, 0);

     rb_define_method(c_jgCollision, "hit_point", rb_jgCollisionGetHitPoint, 0);
     rb_define_method(c_jgCollision, "edge_d", rb_jgCollisionGetEdgeD, 0);
     rb_define_method(c_jgCollision, "normal", rb_jgCollisionGetNormal, 0);
     rb_define_method(c_jgCollision, "penetration", rb_jgCollisionGetPenetration, 0);
}
