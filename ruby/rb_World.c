#include "rb_jiggle.h"

VALUE c_jgWorld;

static VALUE rb_jgWorldAlloc(VALUE klass)
{
     jgWorld *world = jgWorldAlloc();
     return Data_Wrap_Struct(klass, NULL, jgWorldFree, world);
}

static VALUE rb_jgWorldInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE attr;
     rb_scan_args(argc, argv, "01", &attr);
     
     jgVector2 min = rb_jgHashGetVector2(attr, "min", jgv(-20, -20));
     jgVector2 max = rb_jgHashGetVector2(attr, "max", jgv(20, 20));
     jgWorldInit(WORLD(self), min, max);
     rb_iv_set(self, "bodies", rb_ary_new());
     return self;
}

static VALUE rb_jgWorldUpdate(VALUE self, VALUE stepsize)
{
     jgWorldUpdate(WORLD(self), NUM2DBL(stepsize));
     return self;
}

static VALUE rb_jgWorldAddBody(VALUE self, VALUE body)
{
     jgWorldAddBody(WORLD(self), BODY(body));
     rb_ary_push(rb_iv_get(self, "bodies"), body);
     return body;
}

static VALUE rb_jgWorldBodies(VALUE self)
{
     return rb_iv_get(self, "bodies");
}

static VALUE rb_jgWorldRemoveBody(VALUE self, VALUE body)
{
     jgWorldRemoveBody(WORLD(self), BODY(body));
     return rb_ary_delete(rb_iv_get(self, "bodies"), body);
}

VECTOR_GET(rb_jgWorldGetGravity, WORLD, gravity)
VECTOR_SET(rb_jgWorldSetGravity, WORLD, gravity)

FLOAT_GET(rb_jgWorldGetDamping, WORLD, damping)
FLOAT_SET(rb_jgWorldSetDamping, WORLD, damping)

void Init_jgWorld()
{
     c_jgWorld = rb_define_class_under(m_Jiggle, "World", rb_cObject);
     rb_define_alloc_func(c_jgWorld, rb_jgWorldAlloc);
     rb_define_method(c_jgWorld, "initialize", rb_jgWorldInitialize, -1);

     rb_define_method(c_jgWorld, "update", rb_jgWorldUpdate, 1);
     rb_define_method(c_jgWorld, "add", rb_jgWorldAddBody, 1);
     rb_define_method(c_jgWorld, "remove", rb_jgWorldRemoveBody, 1);
     rb_define_method(c_jgWorld, "bodies", rb_jgWorldBodies, 0);

     rb_define_method(c_jgWorld, "gravity",  rb_jgWorldGetGravity, 0);
     rb_define_method(c_jgWorld, "gravity=", rb_jgWorldSetGravity, 1);

     rb_define_method(c_jgWorld, "damping",  rb_jgWorldGetDamping, 0);
     rb_define_method(c_jgWorld, "damping=", rb_jgWorldSetDamping, 1);
}
