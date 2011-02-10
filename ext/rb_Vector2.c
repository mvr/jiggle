#include "rb_jiggle.h"

VALUE c_jgVector2;

static VALUE rb_jgVector2Alloc(VALUE klass)
{
     jgVector2 *v = malloc(sizeof(jgVector2));
     return Data_Wrap_Struct(klass, NULL, free, v);
}

static VALUE rb_jgVector2Initialize(VALUE self, VALUE x, VALUE y)
{
	jgVector2 *v = VGET(self);
	v->x = NUM2DBL(x);
	v->y = NUM2DBL(y);
	
	return self;
}

FLOAT_GET(rb_jgVector2GetX, VGET, x)
FLOAT_SET(rb_jgVector2SetX, VGET, x)
FLOAT_GET(rb_jgVector2GetY, VGET, y)
FLOAT_SET(rb_jgVector2SetY, VGET, y)

VALUE rb_jgVector2Add(VALUE self, VALUE other)
{
     return VNEW(jgVector2Add(*VGET(self), *VGET(other)));
}

VALUE rb_jgVector2Subtract(VALUE self, VALUE other)
{
     return VNEW(jgVector2Subtract(*VGET(self), *VGET(other)));
}

VALUE rb_jgVector2Multiply(VALUE self, VALUE other)
{
     return VNEW(jgVector2Multiply(*VGET(self), NUM2DBL(other)));
}

VALUE rb_jgVector2Divide(VALUE self, VALUE other)
{
     return VNEW(jgVector2Divide(*VGET(self), NUM2DBL(other)));
}

VALUE rb_jgVector2Dot(VALUE self, VALUE other)
{
     return rb_float_new(jgVector2Dot(*VGET(self), *VGET(other)));
}

VALUE rb_jgVector2Length(VALUE self)
{
     return rb_float_new(jgVector2Length(*VGET(self)));
}

VALUE rb_jgVector2DistanceTo(VALUE self, VALUE other)
{
     return rb_float_new(jgVector2DistanceBetween(*VGET(self), *VGET(other)));
}

void Init_jgVector2()
{
     c_jgVector2 = rb_define_class_under(m_Jiggle, "Vector2", rb_cObject);
     rb_define_alloc_func(c_jgVector2, rb_jgVector2Alloc);
     rb_define_method(c_jgVector2, "initialize", rb_jgVector2Initialize, 2);
     
     rb_define_method(c_jgVector2, "x", rb_jgVector2GetX, 0);
     rb_define_method(c_jgVector2, "x=", rb_jgVector2SetX, 1);
     rb_define_method(c_jgVector2, "y", rb_jgVector2GetY, 0);
     rb_define_method(c_jgVector2, "y=", rb_jgVector2SetY, 1);

     rb_define_method(c_jgVector2, "+", rb_jgVector2Add, 1);
     rb_define_method(c_jgVector2, "-", rb_jgVector2Subtract, 1);
     rb_define_method(c_jgVector2, "*", rb_jgVector2Multiply, 1);
     rb_define_method(c_jgVector2, "/", rb_jgVector2Divide, 1);

     rb_define_method(c_jgVector2, "dot", rb_jgVector2Dot, 1);

     rb_define_method(c_jgVector2, "length", rb_jgVector2Length, 0);
     rb_define_method(c_jgVector2, "distance_to", rb_jgVector2DistanceTo, 1);
}
