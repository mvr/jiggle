#include "rb_jiggle.h"

VALUE c_jgArea;

VECTOR_GET(rb_jgAreaCenterOfMass, AREA, centerOfMass)
VECTOR_GET(rb_jgAreaPosition,     AREA, derivedPosition)
VECTOR_GET(rb_jgAreaVelocity,     AREA, derivedVelocity)
FLOAT_GET( rb_jgAreaAngle,        AREA, derivedAngle)

BOOL_GET(rb_jgAreaGetIsShapeMatching, AREA, isShapeMatching)
BOOL_SET(rb_jgAreaSetIsShapeMatching, AREA, isShapeMatching)

BOOL_GET(rb_jgAreaGetIsKinematic, AREA, isKinematic)
BOOL_SET(rb_jgAreaSetIsKinematic, AREA, isKinematic)

FLOAT_GET(rb_jgAreaGetShapeStrength, AREA, shapeStrength)
FLOAT_SET(rb_jgAreaSetShapeStrength, AREA, shapeStrength)

FLOAT_GET(rb_jgAreaGetShapeDamping, AREA, shapeDamping)
FLOAT_SET(rb_jgAreaSetShapeDamping, AREA, shapeDamping)

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

     area->isKinematic = rb_jgHashGetBool(attr, "kinematic", false);

     if(area->isKinematic)
     {
          area->isShapeMatching = rb_jgHashGetBool(attr, "shape_matching", true);
          area->shapeStrength   = rb_jgHashGetFloat(attr, "shape_strength", 400);
          area->shapeDamping    = rb_jgHashGetFloat(attr, "shape_damping", 30);
     }
     else
     {
          area->isShapeMatching = rb_jgHashGetBool(attr, "shape_matching", false);
          area->shapeStrength   = rb_jgHashGetFloat(attr, "shape_strength", 20);
          area->shapeDamping    = rb_jgHashGetFloat(attr, "shape_damping", 5);
     }

     return self;
}

static VALUE rb_jgAreaUpdate(VALUE self)
{
     jgArea *area = AREA(self);
    
     jgParticle **unwrapped;
     VALUE particles = rb_iv_get(self, "@particles");
     int numParticles = RARRAY_LEN(particles);
     unwrapped = malloc(numParticles * sizeof(jgParticle *));
     for(int i = 0; i < numParticles; i++)
          unwrapped[i] = PARTICLE(RARRAY_PTR(particles)[i]);

     jgListFree(area->particles);
     area->particles = jgListNewFromArray((void **)unwrapped, numParticles);
     free(unwrapped); 

     return Qnil;
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

     rb_define_method(c_jgArea, "update!",   rb_jgAreaUpdate, 0);

     rb_define_method(c_jgArea, "area",             rb_jgAreaArea, 0);
     rb_define_method(c_jgArea, "center_of_mass",   rb_jgAreaCenterOfMass, 0);

     rb_define_method(c_jgArea, "position",   rb_jgAreaPosition, 0);
     rb_define_method(c_jgArea, "velocity",   rb_jgAreaVelocity, 0);
     rb_define_method(c_jgArea, "angle",      rb_jgAreaAngle, 0);

     rb_define_method(c_jgArea, "shape_matching?",    rb_jgAreaGetIsShapeMatching, 0);
     rb_define_method(c_jgArea, "shape_matching=",    rb_jgAreaSetIsShapeMatching, 1);

     rb_define_method(c_jgArea, "kinematic?",    rb_jgAreaGetIsKinematic, 0);
     rb_define_method(c_jgArea, "kinematic=",    rb_jgAreaSetIsKinematic, 1);

     rb_define_method(c_jgArea, "shape_strength",    rb_jgAreaGetShapeStrength, 0);
     rb_define_method(c_jgArea, "shape_strength=",   rb_jgAreaSetShapeStrength, 1);
     rb_define_method(c_jgArea, "shape_damping",     rb_jgAreaGetShapeDamping, 0);
     rb_define_method(c_jgArea, "shape_damping=",    rb_jgAreaSetShapeDamping, 1);
}
