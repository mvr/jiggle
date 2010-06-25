#include "rb_jiggle.h"

VALUE c_jgArea;

VECTOR_GET(rb_jgAreaCenterOfMass, AREA, centerOfMass)

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

     rb_iv_set(self, "@particles", rb_funcall(particles, rb_intern("dup"), 0));

     return self;
}

static VALUE rb_jgAreaContains(VALUE self, VALUE point)
{
     return BOOL2VAL(jgAreaContains(AREA(self), *VGET(point)));
}

static VALUE rb_jgAreaArea(VALUE self)
{
     return rb_float_new(jgAreaArea(AREA(self)));
}

void Init_jgArea()
{
     c_jgArea = rb_define_class_under(m_Jiggle, "Area", rb_cObject);
     rb_define_alloc_func(c_jgArea, rb_jgAreaAlloc);
     rb_define_method(c_jgArea, "initialize", rb_jgAreaInitialize, -1);

     rb_define_private_method(c_jgArea, "_contains?",   rb_jgAreaContains, 1);

     rb_define_method(c_jgArea, "area",             rb_jgAreaArea, 0);
     rb_define_method(c_jgArea, "center_of_mass",   rb_jgAreaCenterOfMass, 0);
}
