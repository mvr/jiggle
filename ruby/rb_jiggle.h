/***************************/
/* Mitchell Riley (c) 2008 */
/***************************/

#ifndef __RB_JIGGLE_H__
#define __RB_JIGGLE_H__

#include "ruby.h"

#include "jiggle.h"

extern VALUE m_Jiggle;

extern VALUE c_jgVector2;
extern VALUE c_jgParticle;
extern VALUE c_jgArea;
extern VALUE c_jgSpring;
extern VALUE c_jgWorld;
extern VALUE c_jgCollision;

void Init_jiggle_ext(void);
void Init_jgVector2();
void Init_jgParticle();
void Init_jgSpring();
void Init_jgArea();
void Init_jgWorld();
void Init_jgCollision();

VALUE rb_jgHashGet(VALUE hash, char *key);
int rb_jgHashGetInt(VALUE hash, char *key, int de);
double rb_jgHashGetFloat(VALUE hash, char *key, double de);
jgVector2 rb_jgHashGetVector2(VALUE hash, char *key, jgVector2 de);
bool rb_jgHashGetBool(VALUE hash, char *key, bool de);

#define rb_jgParticleWrap(particle)                                     \
     Data_Wrap_Struct(c_jgParticle, NULL, jgParticleFree, (particle))
VALUE rb_jgAreaWrap(jgArea *area);
VALUE rb_jgCollisionWrap(jgCollision *collision);

static inline VALUE VNEW(jgVector2 v)
{
	jgVector2 *ptr = malloc(sizeof(jgVector2));
	*ptr = v;
	return Data_Wrap_Struct(c_jgVector2, NULL, free, ptr);	
}

#define SET_PARENT(value)                               \
     rb_iv_set(value, "parent", self)

#define BOOL2VAL(b)   ((b) ? Qtrue : Qfalse)
#define VAL2BOOL(b)   (RTEST(b))

#define GETTER_TEMPLATE(func_name, klass, klass_name, type)		\
     static inline type *						\
     func_name(VALUE self)						\
     {									\
	  if(!rb_obj_is_kind_of(self, klass))				\
	       rb_raise(rb_eTypeError, "wrong argument type %s (expected Jiggle::" klass_name ")", rb_obj_classname(self)); \
	  type *ptr;							\
	  Data_Get_Struct(self, type, ptr);				\
	  return ptr;							\
     }

GETTER_TEMPLATE(VGET,         c_jgVector2,      "Vector2",      jgVector2)
GETTER_TEMPLATE(AREA,         c_jgArea,         "Area",         jgArea)
GETTER_TEMPLATE(PARTICLE,     c_jgParticle,     "Particle",     jgParticle)
GETTER_TEMPLATE(SPRING,       c_jgSpring,       "Spring",       jgSpring)
GETTER_TEMPLATE(WORLD,        c_jgWorld,        "World",        jgWorld)
GETTER_TEMPLATE(COLLISION,    c_jgCollision,    "Collision",    jgCollision)

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
          VALUE wrapped = Data_Wrap_Struct(c_jgVector2, NULL, NULL, vector); \
          SET_PARENT(wrapped);                                          \
          return wrapped;                                               \
     }

#define VECTOR_SET(func_name, type_conv, attr)		\
     static VALUE func_name(VALUE self, VALUE vector)	\
     {							\
	  type_conv(self)->attr = *VGET(vector);	\
	  return self;					\
     }

#endif

