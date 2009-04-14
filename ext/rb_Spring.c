#include "rb_jiggle.h"

VALUE c_jgSpring;

FLOAT_GET(rb_jgSpringGetLength, SPRING, springD)
FLOAT_SET(rb_jgSpringSetLength, SPRING, springD)

FLOAT_GET(rb_jgSpringGetStrength, SPRING, springK)
FLOAT_SET(rb_jgSpringSetStrength, SPRING, springK)

FLOAT_GET(rb_jgSpringGetDamping, SPRING, damping)
FLOAT_SET(rb_jgSpringSetDamping, SPRING, damping)

static VALUE rb_jgSpringAlloc(VALUE klass)
{
     jgSpring *spring = jgSpringAlloc();
     return Data_Wrap_Struct(klass, NULL, jgSpringFree, spring);
}


static VALUE rb_jgSpringInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE particleAvalue, particleBvalue, attr;
     rb_scan_args(argc, argv, "21", &particleAvalue, &particleBvalue, &attr);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     jgSpring *spring = SPRING(self);
     
     jgParticle *particleA = PARTICLE(particleAvalue);
     jgParticle *particleB = PARTICLE(particleBvalue);

     rb_iv_set(self, "@a", particleAvalue);
     rb_iv_set(self, "@b", particleBvalue);

     float defaultLength = jgVector2DistanceBetween(particleA->position,
                                                    particleB->position);

     float length   = rb_jgHashGetFloat(attr, "length",   defaultLength);
     float strength = rb_jgHashGetFloat(attr, "strength", 400.0);
     float damping  = rb_jgHashGetFloat(attr, "damping",  10.0);

     jgSpringInit(spring, particleA, particleB, length, strength, damping);

     return self;
}

void Init_jgSpring()
{
     c_jgSpring = rb_define_class_under(m_Jiggle, "Spring", rb_cObject);
     rb_define_alloc_func(c_jgSpring, rb_jgSpringAlloc);
     rb_define_method(c_jgSpring, "initialize", rb_jgSpringInitialize, -1);

     rb_define_attr(c_jgSpring, "a", 1, 0);
     rb_define_attr(c_jgSpring, "b", 1, 0);
     
     rb_define_method(c_jgSpring, "length",    rb_jgSpringGetLength, 0);
     rb_define_method(c_jgSpring, "length=",   rb_jgSpringSetLength, 1);

     rb_define_method(c_jgSpring, "strength",  rb_jgSpringGetStrength, 0);
     rb_define_method(c_jgSpring, "strength=", rb_jgSpringSetStrength, 1);

     rb_define_method(c_jgSpring, "damping",   rb_jgSpringGetDamping, 0);
     rb_define_method(c_jgSpring, "damping=",  rb_jgSpringSetDamping, 1);
}
