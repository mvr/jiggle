#include "rb_jiggle.h"

VALUE c_jgWorld;

static void rb_jgWorldFree(void *world)
{
     jgWorldClearCollisions(world);
     jgWorldFree(world);
}

static VALUE rb_jgWorldAlloc(VALUE klass)
{
     jgWorld *world = jgWorldAlloc();
     return Data_Wrap_Struct(klass, NULL, rb_jgWorldFree, world);
}

static VALUE rb_jgWorldInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE attr;
     rb_scan_args(argc, argv, "01", &attr);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     float ticks = rb_jgHashGetFloat(attr, "ticks_per_second", 100);

     jgWorldInit(WORLD(self));

     rb_iv_set(self, "@ticks_per_second", rb_float_new(ticks));
     rb_iv_set(self, "@areas", rb_ary_new());
     rb_iv_set(self, "@particles", rb_ary_new());
     rb_iv_set(self, "@springs", rb_ary_new());

     rb_funcall(self, rb_intern("setup"), 0);

     return self;
}

static VALUE rb_jgWorldStep(VALUE self, VALUE step)
{
     jgWorldStep(WORLD(self), NUM2DBL(step));
     return self;
}

static VALUE rb_jgWorldCollisions(VALUE self)
{
     jgWorld *world = WORLD(self);

     VALUE array = rb_ary_new();

     jgCollision *collision;
     JG_LIST_FOREACH(world->collisions, collision)
          rb_ary_push(array, rb_jgCollisionWrap(collision, self));

     return array;
}

static VALUE rb_jgWorldClearCollisions(VALUE self)
{
     jgWorld *world = WORLD(self);
     
     jgWorldClearCollisions(world);

     return self;
}

#define RUBY_ADD_AND_REMOVE(classname, iv, converter)                   \
     static VALUE rb_jgWorldAdd ## classname(VALUE self, VALUE thing)   \
     {                                                                  \
          jgWorldAdd ## classname(WORLD(self), converter(thing));       \
          rb_ary_push(rb_iv_get(self, "@" iv "s"), thing);                  \
          return thing;                                                 \
     }                                                                  \
                                                                        \
     static VALUE rb_jgWorldRemove ## classname(VALUE self, VALUE thing) \
     {                                                                  \
          jgWorldRemove ## classname(WORLD(self), converter(thing));    \
          return rb_ary_delete(rb_iv_get(self, "@" iv "s"), thing);         \
     }                                                                  \

RUBY_ADD_AND_REMOVE(Area, "area", AREA)
RUBY_ADD_AND_REMOVE(Particle, "particle", PARTICLE)
RUBY_ADD_AND_REMOVE(Spring, "spring", SPRING)

#undef RUBY_ADD_AND_REMOVE

VECTOR_GET(rb_jgWorldGetGravity, WORLD, gravity)
VECTOR_SET(rb_jgWorldSetGravity, WORLD, gravity)

FLOAT_GET(rb_jgWorldGetDamping, WORLD, damping)
FLOAT_SET(rb_jgWorldSetDamping, WORLD, damping)

#define RUBY_DEFINE_ADD_AND_REMOVE(method_name, class)                  \
     rb_define_method(c_jgWorld, "add_" method_name,    rb_jgWorldAdd ## class,    1); \
     rb_define_method(c_jgWorld, "remove_" method_name, rb_jgWorldRemove ## class, 1);

void Init_jgWorld()
{
     c_jgWorld = rb_define_class_under(m_Jiggle, "World", rb_cObject);
     rb_define_alloc_func(c_jgWorld, rb_jgWorldAlloc);
     rb_define_method(c_jgWorld, "initialize", rb_jgWorldInitialize, -1);

     rb_define_method(c_jgWorld, "step",   rb_jgWorldStep, 1);

     RUBY_DEFINE_ADD_AND_REMOVE("particle", Particle);
     RUBY_DEFINE_ADD_AND_REMOVE("area",     Area);
     RUBY_DEFINE_ADD_AND_REMOVE("spring",   Spring);

     rb_define_method(c_jgWorld, "collisions", rb_jgWorldCollisions, 0);
     rb_define_method(c_jgWorld, "clear_collisions", rb_jgWorldClearCollisions, 0);

     rb_define_method(c_jgWorld, "gravity",  rb_jgWorldGetGravity, 0);
     rb_define_method(c_jgWorld, "gravity=", rb_jgWorldSetGravity, 1);

     rb_define_method(c_jgWorld, "damping",  rb_jgWorldGetDamping, 0);
     rb_define_method(c_jgWorld, "damping=", rb_jgWorldSetDamping, 1);
}

#undef RUBY_DEFINE_ADD_AND_REMOVE
