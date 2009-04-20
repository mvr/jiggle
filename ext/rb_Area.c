#include "rb_jiggle.h"

VALUE c_jgArea;

FLOAT_GET(rb_jgAreaGetElasticity, AREA, elasticity)
FLOAT_SET(rb_jgAreaSetElasticity, AREA, elasticity)

FLOAT_GET(rb_jgAreaGetFriction, AREA, friction)
FLOAT_SET(rb_jgAreaSetFriction, AREA, friction)

static VALUE rb_jgAreaAlloc(VALUE klass)
{
     jgArea *area = jgAreaAlloc();
     return Data_Wrap_Struct(klass, NULL, jgAreaFree, area);
}

static VALUE rb_jgAreaInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE particles, attr;
     rb_scan_args(argc, argv, "11", &particles, &attr);
     Check_Type(particles, T_ARRAY);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     jgArea *area = AREA(self);

     jgParticle **unwrapped;
     int numParticles = RARRAY_LEN(particles);
     unwrapped = malloc(numParticles * sizeof(jgParticle *));
     for(int i = 0; i < numParticles; i++)
          unwrapped[i] = PARTICLE(RARRAY_PTR(particles)[i]);
     jgAreaInit(area, unwrapped, numParticles);
     free(unwrapped);

     rb_iv_set(self, "@particles", particles);

     float friction   = rb_jgHashGetFloat(attr, "friction", 0.8);
     float elasticity = rb_jgHashGetFloat(attr, "elasticity", 0.3);

     area->friction = friction;
     area->elasticity = elasticity;

     return self;
}

static VALUE rb_jgAreaContains(VALUE self, VALUE point)
{
     return BOOL2VAL(jgAreaContains(AREA(self), *VGET(point)));
}

void Init_jgArea()
{
     c_jgArea = rb_define_class_under(m_Jiggle, "Area", rb_cObject);
     rb_define_alloc_func(c_jgArea, rb_jgAreaAlloc);
     rb_define_method(c_jgArea, "initialize", rb_jgAreaInitialize, -1);

     rb_define_method(c_jgArea, "elasticity",  rb_jgAreaGetElasticity, 0);
     rb_define_method(c_jgArea, "elasticity=", rb_jgAreaSetElasticity, 1);
     rb_define_method(c_jgArea, "friction",    rb_jgAreaGetFriction, 0);
     rb_define_method(c_jgArea, "friction=",   rb_jgAreaSetFriction, 1);

     rb_define_private_method(c_jgArea, "_contains?",   rb_jgAreaContains, 1);

     rb_define_attr(c_jgArea, "particles", 1, 0);
}