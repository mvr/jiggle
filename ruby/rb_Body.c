#include "rb_jiggle.h"

VALUE c_jgBody;
VALUE c_jgStaticBody;

FLOAT_GET(rb_jgBodyGetElasticity, BODY, elasticity)
FLOAT_SET(rb_jgBodySetElasticity, BODY, elasticity)

FLOAT_GET(rb_jgBodyGetFriction, BODY, friction)
FLOAT_SET(rb_jgBodySetFriction, BODY, friction)

UINT_GET(rb_jgBodyGetLayers, BODY, layers)
UINT_SET(rb_jgBodySetLayers, BODY, layers)

BOOL_GET(rb_jgBodyIsStatic, BODY, isStatic)
BOOL_GET(rb_jgBodyIsActive, BODY, isActive)
BOOL_GET(rb_jgBodyIsKinematic, BODY, isKinematic)

FLOAT_GET(rb_jgBodyGetEdgeSpringK, BODY, edgeSpringK)
FLOAT_SET(rb_jgBodySetEdgeSpringK, BODY, edgeSpringK)
FLOAT_GET(rb_jgBodyGetEdgeSpringDamp, BODY, edgeSpringDamp)
FLOAT_SET(rb_jgBodySetEdgeSpringDamp, BODY, edgeSpringDamp)

BOOL_GET(rb_jgBodyGetIsShapeMatching, BODY, isShapeMatching)
BOOL_SET(rb_jgBodySetIsShapeMatching, BODY, isShapeMatching)
FLOAT_GET(rb_jgBodyGetShapeSpringK, BODY, shapeSpringK)
FLOAT_SET(rb_jgBodySetShapeSpringK, BODY, shapeSpringK)
FLOAT_GET(rb_jgBodyGetShapeSpringDamp, BODY, shapeSpringDamp)
FLOAT_SET(rb_jgBodySetShapeSpringDamp, BODY, shapeSpringDamp)

VECTOR_GET(rb_jgBodyGetPosition, BODY, derivedPosition);
VECTOR_SET(rb_jgBodySetPosition, BODY, derivedPosition);
VECTOR_GET(rb_jgBodyGetVelocity, BODY, derivedVelocity);
VECTOR_SET(rb_jgBodySetVelocity, BODY, derivedVelocity);
FLOAT_GET(rb_jgBodyGetAngle, BODY, derivedAngle)
FLOAT_SET(rb_jgBodySetAngle, BODY, derivedAngle)

static VALUE rb_jgBodyAddForce(VALUE self, VALUE force)
{
     jgBodyAddForce(BODY(self), *VGET(force));
     return force;
}

static VALUE rb_jgBodyPointMasses(VALUE self)
{
     jgBody *body = BODY(self);
     VALUE array = rb_ary_new();
     for(int i = 0; i < body->numOfPoints; i++)
     {
	  VALUE pointMass = Data_Wrap_Struct(c_jgPointMass, NULL, NULL, &body->pointMasses[i]);
	  rb_ary_push(array, pointMass); 
     }
     return array;       
}

static VALUE rb_jgStaticBodyAlloc(VALUE klass)
{
     jgBody *staticbody = jgStaticBodyAlloc();
     return Data_Wrap_Struct(c_jgStaticBody, NULL, jgBodyFree, staticbody);
}

static VALUE rb_jgStaticBodyInitialize(int argc, VALUE *argv, VALUE self)
{
     VALUE points, attr;
     rb_scan_args(argc, argv, "11", &points, &attr);

     Check_Type(points, T_ARRAY);
     int numOfPoints = RARRAY(points)->len;
     jgVector2 vects[numOfPoints];
     for(int i = 0; i < numOfPoints; i++)
	  vects[i] = *VGET(RARRAY(points)->ptr[i]);

     jgVector2 position = rb_jgHashGetVector2(attr, "position", jgv(0,0));
     double angle = rb_jgHashGetFloat(attr, "angle", 0);
     jgVector2 scale = rb_jgHashGetVector2(attr, "scale", jgv(1,1));
	  
     jgStaticBodyInit(BODY(self), vects, numOfPoints, position, angle, scale);
     return self;
}

void Init_jgBody()
{
     c_jgBody = rb_define_class_under(m_Jiggle, "Body", rb_cObject);

     rb_define_method(c_jgBody, "elasticity",  rb_jgBodyGetElasticity, 0);
     rb_define_method(c_jgBody, "elasticity=", rb_jgBodySetElasticity, 1);
     rb_define_method(c_jgBody, "friction",    rb_jgBodyGetFriction, 0);
     rb_define_method(c_jgBody, "friction=",   rb_jgBodySetFriction, 1);
     rb_define_method(c_jgBody, "layers",      rb_jgBodyGetLayers, 0);
     rb_define_method(c_jgBody, "layers=",     rb_jgBodySetLayers, 1);

     rb_define_method(c_jgBody, "static?",     rb_jgBodyIsStatic, 0);
     rb_define_method(c_jgBody, "active?",     rb_jgBodyIsActive, 0);
     rb_define_method(c_jgBody, "kinematic?",  rb_jgBodyIsKinematic, 0);
     
     rb_define_method(c_jgBody, "edge_spring_k",  rb_jgBodyGetEdgeSpringK, 0);
     rb_define_method(c_jgBody, "edge_spring_k=", rb_jgBodySetEdgeSpringK, 1);
     rb_define_method(c_jgBody, "edge_spring_damp",  rb_jgBodyGetEdgeSpringDamp, 0);
     rb_define_method(c_jgBody, "edge_spring_damp=", rb_jgBodySetEdgeSpringDamp, 1);

     rb_define_method(c_jgBody, "shape_matching?", rb_jgBodyGetIsShapeMatching, 0);
     rb_define_method(c_jgBody, "shape_matching=", rb_jgBodySetIsShapeMatching, 1);
     rb_define_method(c_jgBody, "shape_spring_k",  rb_jgBodyGetShapeSpringK, 0);
     rb_define_method(c_jgBody, "shape_spring_k=", rb_jgBodySetShapeSpringK, 1);
     rb_define_method(c_jgBody, "shape_spring_damp",  rb_jgBodyGetShapeSpringDamp, 0);
     rb_define_method(c_jgBody, "shape_spring_damp=", rb_jgBodySetShapeSpringDamp, 1);
     
     rb_define_method(c_jgBody, "position",  rb_jgBodyGetPosition, 0);
     rb_define_method(c_jgBody, "position=", rb_jgBodySetPosition, 1);
     rb_define_method(c_jgBody, "velocity",  rb_jgBodyGetVelocity, 0);
     rb_define_method(c_jgBody, "velocity=", rb_jgBodySetVelocity, 1);
     rb_define_method(c_jgBody, "angle",  rb_jgBodyGetAngle, 0);
     rb_define_method(c_jgBody, "angle=", rb_jgBodySetAngle, 1);
     
     rb_define_method(c_jgBody, "masses", rb_jgBodyPointMasses, 0);

     rb_define_method(c_jgBody, "force", rb_jgBodyAddForce, 1);

     c_jgStaticBody = rb_define_class_under(m_Jiggle, "StaticBody", c_jgBody);
     rb_define_alloc_func(c_jgStaticBody, rb_jgStaticBodyAlloc);
     rb_define_method(c_jgStaticBody, "initialize", rb_jgStaticBodyInitialize, -1);
}
