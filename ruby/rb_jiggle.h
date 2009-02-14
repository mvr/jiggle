/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __RB_JIGGLE_H__
#define __RB_JIGGLE_H__

#include "ruby.h"

#include "jiggle.h"

extern VALUE m_Jiggle;

extern VALUE c_jgVector2;
extern VALUE c_jgPointMass;
extern VALUE c_jgBody;
extern VALUE c_jgStaticBody;
extern VALUE c_jgSpringBody;
extern VALUE c_jgPressureBody;
extern VALUE c_jgWorld;

void Init_jiggle(void);
void Init_jgVector2();
void Init_jgPointMass();
void Init_jgWorld();
void Init_jgBody();
void Init_jgSpringBody();

VALUE rb_jgHashGet(VALUE hash, char *key);
int rb_jgHashGetInt(VALUE hash, char *key, int de);
double rb_jgHashGetFloat(VALUE hash, char *key, double de);
jgVector2 rb_jgHashGetVector2(VALUE hash, char *key, jgVector2 de);
bool rb_jgHashGetBool(VALUE hash, char *key, bool de);

#define BOOL2VAL(b)   ((b) ? Qtrue : Qfalse)
#define VAL2BOOL(b)   (RTEST(b))

#define GETTER_TEMPLATE(func_name, klass, klass_name, type)		\
     static inline type *						\
     func_name(VALUE self)						\
     {									\
	  if(!rb_obj_is_kind_of(self, klass))				\
	       rb_raise(rb_eTypeError, "wrong argument type %s (expected Jiggle::klass_name)", rb_obj_classname(self)); \
	  type *ptr;							\
	  Data_Get_Struct(self, type, ptr);				\
	  return ptr;							\
     }

GETTER_TEMPLATE(VGET,         c_jgVector2,      Vector2,      jgVector2)
GETTER_TEMPLATE(POINTMASS,    c_jgPointMass,    PointMass,    jgPointMass)
GETTER_TEMPLATE(BODY,         c_jgBody,         Body,         jgBody)
GETTER_TEMPLATE(WORLD,        c_jgWorld,        World,        jgWorld)

#define FLOAT_GET(func_name, type_conv, attr)			\
     static VALUE func_name(VALUE self)				\
     {								\
	  return rb_float_new(type_conv(self)->attr);		\
     }								\

#define FLOAT_SET(func_name, type_conv, attr)			\
     static VALUE func_name(VALUE self, VALUE value)		\
     {								\
	  type_conv(self)->attr = NUM2DBL(value);		\
	  return self;						\
     }								

#define BOOL_GET(func_name, type_conv, attr)			\
     static VALUE func_name(VALUE self)				\
     {								\
	  return BOOL2VAL(type_conv(self)->attr);		\
     }								\

#define BOOL_SET(func_name, type_conv, attr)			\
     static VALUE func_name(VALUE self, VALUE value)		\
     {								\
	  type_conv(self)->attr = VAL2BOOL(value);		\
	  return self;						\
     }								

#define UINT_GET(func_name, type_conv, attr)			\
     static VALUE func_name(VALUE self)				\
     {								\
	  return UINT2NUM(type_conv(self)->attr);		\
     }								\

#define UINT_SET(func_name, type_conv, attr)			\
     static VALUE func_name(VALUE self, VALUE value)		\
     {								\
	  type_conv(self)->attr = NUM2UINT(value);		\
	  return self;						\
     }								
#define VECTOR_GET(func_name, type_conv, attr)				\
     static VALUE func_name(VALUE self)					\
     {									\
	  jgVector2 *vector = &(type_conv(self)->attr);			\
	  return Data_Wrap_Struct(c_jgVector2, NULL, NULL, vector);	\
     }

#define VECTOR_SET(func_name, type_conv, attr)		\
     static VALUE func_name(VALUE self, VALUE vector)	\
     {							\
	  type_conv(self)->attr = *VGET(vector);	\
	  return self;					\
     }

#endif

