#include "rb_jiggle.h"

VALUE c_jgSpace;

static void rb_jgSpaceFree(void *space)
{
     jgSpaceClearCollisions(space);
     jgSpaceFree(space);
}

static VALUE rb_jgSpaceAlloc(VALUE klass)
{
     jgSpace *space = jgSpaceAlloc();
     return Data_Wrap_Struct(klass, NULL, rb_jgSpaceFree, space);
}

static VALUE rb_jgSpaceInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE attr;
     rb_scan_args(argc, argv, "01", &attr);
     if(!NIL_P(attr)) Check_Type(attr, T_HASH);

     float ticks = rb_jgHashGetFloat(attr, "ticks_per_second", 100);

     jgSpaceInit(SPACE(self));

     rb_iv_set(self, "@ticks_per_second", rb_float_new(ticks));
     rb_iv_set(self, "@areas", rb_ary_new());
     rb_iv_set(self, "@particles", rb_ary_new());
     rb_iv_set(self, "@springs", rb_ary_new());

     rb_funcall(self, rb_intern("setup"), 0);

     return self;
}

static VALUE rb_jgSpaceStep(VALUE self, VALUE step)
{
     jgSpaceStep(SPACE(self), NUM2DBL(step));
     return self;
}

static VALUE rb_jgSpaceCollisions(VALUE self)
{
     jgSpace *space = SPACE(self);

     VALUE array = rb_ary_new();

     jgCollision *collision;
     JG_LIST_FOREACH(space->collisions, collision)
          rb_ary_push(array, rb_jgCollisionWrap(collision, self));

     return array;
}

static VALUE rb_jgSpaceClearCollisions(VALUE self)
{
     jgSpace *space = SPACE(self);
     
     jgSpaceClearCollisions(space);

     return self;
}

#define RUBY_ADD_AND_REMOVE(classname, iv, converter)                   \
     static VALUE rb_jgSpaceAdd ## classname(VALUE self, VALUE thing)   \
     {                                                                  \
          jgSpaceAdd ## classname(SPACE(self), converter(thing));       \
          rb_ary_push(rb_iv_get(self, "@" iv "s"), thing);                  \
          return thing;                                                 \
     }                                                                  \
                                                                        \
     static VALUE rb_jgSpaceRemove ## classname(VALUE self, VALUE thing) \
     {                                                                  \
          jgSpaceRemove ## classname(SPACE(self), converter(thing));    \
          return rb_ary_delete(rb_iv_get(self, "@" iv "s"), thing);         \
     }                                                                  \

RUBY_ADD_AND_REMOVE(Area, "area", AREA)
RUBY_ADD_AND_REMOVE(Particle, "particle", PARTICLE)
RUBY_ADD_AND_REMOVE(Spring, "spring", SPRING)

#undef RUBY_ADD_AND_REMOVE

VECTOR_GET(rb_jgSpaceGetGravity, SPACE, gravity)
VECTOR_SET(rb_jgSpaceSetGravity, SPACE, gravity)

FLOAT_GET(rb_jgSpaceGetDamping, SPACE, damping)
FLOAT_SET(rb_jgSpaceSetDamping, SPACE, damping)

#define RUBY_DEFINE_ADD_AND_REMOVE(method_name, class)                  \
     rb_define_method(c_jgSpace, "add_" method_name,    rb_jgSpaceAdd ## class,    1); \
     rb_define_method(c_jgSpace, "remove_" method_name, rb_jgSpaceRemove ## class, 1);

void Init_jgSpace()
{
     c_jgSpace = rb_define_class_under(m_Jiggle, "Space", rb_cObject);
     rb_define_alloc_func(c_jgSpace, rb_jgSpaceAlloc);
     rb_define_method(c_jgSpace, "initialize", rb_jgSpaceInitialize, -1);

     rb_define_method(c_jgSpace, "step",   rb_jgSpaceStep, 1);

     RUBY_DEFINE_ADD_AND_REMOVE("particle", Particle);
     RUBY_DEFINE_ADD_AND_REMOVE("area",     Area);
     RUBY_DEFINE_ADD_AND_REMOVE("spring",   Spring);

     rb_define_method(c_jgSpace, "collisions", rb_jgSpaceCollisions, 0);
     rb_define_method(c_jgSpace, "clear_collisions", rb_jgSpaceClearCollisions, 0);

     rb_define_method(c_jgSpace, "gravity",  rb_jgSpaceGetGravity, 0);
     rb_define_method(c_jgSpace, "gravity=", rb_jgSpaceSetGravity, 1);

     rb_define_method(c_jgSpace, "damping",  rb_jgSpaceGetDamping, 0);
     rb_define_method(c_jgSpace, "damping=", rb_jgSpaceSetDamping, 1);
}

#undef RUBY_DEFINE_ADD_AND_REMOVE
