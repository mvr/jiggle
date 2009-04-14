#include "rb_jiggle.h"

VALUE c_jgWorld;

static float to_f(VALUE thing)
{
     return NUM2DBL(rb_funcall(thing, rb_intern("to_f"), 0));
}

static VALUE rb_jgWorldAlloc(VALUE klass)
{
     jgWorld *world = jgWorldAlloc();
     return Data_Wrap_Struct(klass, NULL, jgWorldFree, world);
}

static VALUE rb_jgWorldInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE attr;
     rb_scan_args(argc, argv, "01", &attr);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     float ticks   = rb_jgHashGetFloat(attr, "ticks_per_second", 100);

     jgWorldInit(WORLD(self), ticks);

     rb_iv_set(self, "@areas", rb_ary_new());
     rb_iv_set(self, "@particles", rb_ary_new());
     rb_iv_set(self, "@springs", rb_ary_new());

     return self;
}

static VALUE rb_jgWorldUpdate(VALUE self, VALUE currentTime)
{
     jgWorldUpdate(WORLD(self), to_f(currentTime));
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
     rb_define_method(c_jgWorld, "remove_" method_name, rb_jgWorldRemove ## class, 1); \
     rb_define_attr(  c_jgWorld, method_name "s", 1, 0); \

void Init_jgWorld()
{
     c_jgWorld = rb_define_class_under(m_Jiggle, "World", rb_cObject);
     rb_define_alloc_func(c_jgWorld, rb_jgWorldAlloc);
     rb_define_method(c_jgWorld, "initialize", rb_jgWorldInitialize, -1);

     rb_define_method(c_jgWorld, "update", rb_jgWorldUpdate, 1);

     RUBY_DEFINE_ADD_AND_REMOVE("particle", Particle);
     RUBY_DEFINE_ADD_AND_REMOVE("area",     Area);
     RUBY_DEFINE_ADD_AND_REMOVE("spring",   Spring);

     rb_define_method(c_jgWorld, "collisions", rb_jgWorldCollisions, 0);

     rb_define_method(c_jgWorld, "gravity",  rb_jgWorldGetGravity, 0);
     rb_define_method(c_jgWorld, "gravity=", rb_jgWorldSetGravity, 1);

     rb_define_method(c_jgWorld, "damping",  rb_jgWorldGetDamping, 0);
     rb_define_method(c_jgWorld, "damping=", rb_jgWorldSetDamping, 1);
}
