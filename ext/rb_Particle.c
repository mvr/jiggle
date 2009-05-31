#include "rb_jiggle.h"

VALUE c_jgParticle;

FLOAT_GET(rb_jgParticleGetMass, PARTICLE, mass)
FLOAT_SET(rb_jgParticleSetMass, PARTICLE, mass)

VECTOR_GET(rb_jgParticleGetPosition, PARTICLE, position)
VECTOR_SET(rb_jgParticleSetPosition, PARTICLE, position)

VECTOR_GET(rb_jgParticleGetForce, PARTICLE, force)
VECTOR_SET(rb_jgParticleSetForce, PARTICLE, force)

FLOAT_GET(rb_jgParticleGetElasticity, PARTICLE, elasticity)
FLOAT_SET(rb_jgParticleSetElasticity, PARTICLE, elasticity)

FLOAT_GET(rb_jgParticleGetFriction, PARTICLE, friction)
FLOAT_SET(rb_jgParticleSetFriction, PARTICLE, friction)

BOOL_GET(rb_jgParticleGetCollidable, PARTICLE, collidable)
BOOL_SET(rb_jgParticleSetCollidable, PARTICLE, collidable)

BOOL_GET(rb_jgParticleGetFloating, PARTICLE, floating)
BOOL_SET(rb_jgParticleSetFloating, PARTICLE, floating)

static VALUE rb_jgParticleAlloc(VALUE klass)
{
     jgParticle *particle = jgParticleAlloc();
     return Data_Wrap_Struct(klass, NULL, jgParticleFree, particle);
}


static VALUE rb_jgParticleInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE wrappedposition, attr;
     rb_scan_args(argc, argv, "11", &wrappedposition, &attr);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     jgParticle *particle = PARTICLE(self);
     
     jgVector2 position = *VGET(wrappedposition);

     float friction   = rb_jgHashGetFloat(attr, "friction",   0.8);
     float elasticity = rb_jgHashGetFloat(attr, "elasticity", 0.3);
     float mass       = rb_jgHashGetFloat(attr, "mass",       1);
     bool  collidable = rb_jgHashGetBool (attr, "collidable", true);
     bool  floating   = rb_jgHashGetBool (attr, "floating",   false);

     jgParticleInit(particle, mass, position);

     particle->friction = friction;
     particle->elasticity = elasticity;
     particle->collidable = collidable;
     particle->floating = floating;

     return self;
}

static VALUE rb_jgParticleGetVelocity(VALUE self)
{
     return VNEW(jgParticleEffectiveVelocity(PARTICLE(self)));
}

void Init_jgParticle()
{
     c_jgParticle = rb_define_class_under(m_Jiggle, "Particle", rb_cObject);
     rb_define_alloc_func(c_jgParticle, rb_jgParticleAlloc);
     rb_define_method(c_jgParticle, "initialize", rb_jgParticleInitialize, -1);

     rb_define_method(c_jgParticle, "mass" , rb_jgParticleGetMass, 0);
     rb_define_method(c_jgParticle, "mass=", rb_jgParticleSetMass, 1);

     rb_define_method(c_jgParticle, "position" , rb_jgParticleGetPosition, 0);
     rb_define_method(c_jgParticle, "position=", rb_jgParticleSetPosition, 1);

     rb_define_method(c_jgParticle, "velocity" , rb_jgParticleGetVelocity, 0);

     rb_define_method(c_jgParticle, "force" , rb_jgParticleGetForce, 0);
     rb_define_method(c_jgParticle, "force=", rb_jgParticleSetForce, 1);

     rb_define_method(c_jgParticle, "elasticity",  rb_jgParticleGetElasticity, 0);
     rb_define_method(c_jgParticle, "elasticity=", rb_jgParticleSetElasticity, 1);

     rb_define_method(c_jgParticle, "friction",    rb_jgParticleGetFriction, 0);
     rb_define_method(c_jgParticle, "friction=",   rb_jgParticleSetFriction, 1);

     rb_define_method(c_jgParticle, "collidable?",    rb_jgParticleGetCollidable, 0);
     rb_define_method(c_jgParticle, "collidable=",    rb_jgParticleSetCollidable, 1);

     rb_define_method(c_jgParticle, "floating?",    rb_jgParticleGetFloating, 0);
     rb_define_method(c_jgParticle, "floating=",    rb_jgParticleSetFloating, 1);
}


