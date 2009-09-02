#include "rb_jiggle.h"

VALUE c_jgCollision;

static VALUE rb_jgCollisionAlloc(VALUE klass)
{
     jgCollision *v = jgCollisionAlloc();
     return Data_Wrap_Struct(klass, NULL, jgCollisionFree, v);
}

static VALUE rb_jgFindInArray(VALUE array, void *element)
{
     for(int i = 0; i < RARRAY_LEN(array); i++)
     {
          VALUE value = RARRAY_PTR(array)[i];
          if(DATA_PTR(value) == element)
               return value;
     }
     return Qnil;
}

VALUE rb_jgCollisionWrap(jgCollision *collision, VALUE space)
{     
     VALUE wrappedCollision = Data_Wrap_Struct(c_jgCollision, NULL, NULL, collision);
     
     rb_iv_set(wrappedCollision, "@particle",        
               rb_jgFindInArray(rb_iv_get(space, "@particles"), collision->particle));

     rb_iv_set(wrappedCollision, "@area",
               rb_jgFindInArray(rb_iv_get(space, "@areas"), collision->area));

     rb_iv_set(wrappedCollision, "@area_particle_a", 
               rb_jgFindInArray(rb_iv_get(space, "@particles"), collision->areaParticleA));

     rb_iv_set(wrappedCollision, "@area_particle_b", 
               rb_jgFindInArray(rb_iv_get(space, "@particles"), collision->areaParticleB));
     
     return wrappedCollision;
}

VECTOR_GET(rb_jgCollisionGetHitPoint,    COLLISION, hitPt)
FLOAT_GET( rb_jgCollisionGetEdgeD,       COLLISION, edgeD)
VECTOR_GET(rb_jgCollisionGetNormal,      COLLISION, normal)
FLOAT_GET( rb_jgCollisionGetPenetration, COLLISION, penetration)

void Init_jgCollision()
{
     c_jgCollision = rb_define_class_under(m_Jiggle, "Collision", rb_cObject);
     rb_define_alloc_func(c_jgCollision, rb_jgCollisionAlloc);

     rb_define_method(c_jgCollision, "hit_point", rb_jgCollisionGetHitPoint, 0);
     rb_define_method(c_jgCollision, "edge_d", rb_jgCollisionGetEdgeD, 0);
     rb_define_method(c_jgCollision, "normal", rb_jgCollisionGetNormal, 0);
     rb_define_method(c_jgCollision, "penetration", rb_jgCollisionGetPenetration, 0);
}
