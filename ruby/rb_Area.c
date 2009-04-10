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
     rb_iv_set(self, "particles", rb_ary_new());

     VALUE particles, attr;
     rb_scan_args(argc, argv, "11", &particles, &attr);
     Check_Type(particles, T_ARRAY);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     jgArea *area = AREA(self);
     
     jgParticle **unwrapped;
     unwrapped = malloc(RARRAY(particles)->len * sizeof(jgParticle *));
     for(int i = 0; i < RARRAY(particles)->len; i++)
          unwrapped[i] = PARTICLE(RARRAY(particles)->ptr[i]);
     jgAreaInit(area, unwrapped, RARRAY(particles)->len);
     free(unwrapped);

     rb_iv_set(self, "particles", particles);

     float friction   = rb_jgHashGetFloat(attr, "friction", 0.8);
     float elasticity = rb_jgHashGetFloat(attr, "elasticity", 0.3);

     area->friction = friction;
     area->elasticity = elasticity;

     return self;
}

static VALUE rb_jgAreaParticles(VALUE self)
{
     return rb_iv_get(self, "particles");
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

     rb_define_method(c_jgArea, "particles", rb_jgAreaParticles, 0);
}
