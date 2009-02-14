#include "rb_jiggle.h"

VALUE c_jgSpringBody;

static VALUE rb_jgSpringBodyAlloc(VALUE klass)
{
     jgBody *springbody = jgSpringBodyAlloc();
     return Data_Wrap_Struct(c_jgSpringBody, NULL, jgBodyFree, springbody);
}

static VALUE rb_jgSpringBodyInitialize(int argc, VALUE *argv, VALUE self)
{
     jgBody *body = BODY(self);
     VALUE points, attr;
     rb_scan_args(argc, argv, "11", &points, &attr);

     Check_Type(points, T_ARRAY);
     int numOfPoints = RARRAY(points)->len;
     jgVector2 vects[numOfPoints];
     for(int i = 0; i < numOfPoints; i++)
	  vects[i] = *VGET(RARRAY(points)->ptr[i]);
     
     double massPerPoint = rb_jgHashGetFloat(attr, "mass", 1.0);
     jgVector2 position = rb_jgHashGetVector2(attr, "position", jgv(0,0));
     double angle = rb_jgHashGetFloat(attr, "angle", 0);
     jgVector2 scale = rb_jgHashGetVector2(attr, "scale", jgv(1,1));
     bool isKinematic = rb_jgHashGetBool(attr, "kinematic", false);
     bool isShapeMatching = rb_jgHashGetBool(attr, "shape_matching", isKinematic);
     float edgeK = rb_jgHashGetFloat(attr, "edge_k", 150);
     float edgeDamp = rb_jgHashGetFloat(attr, "edge_damp", 5);
     float shapeK = rb_jgHashGetFloat(attr, "shape_k", 3);
     float shapeDamp = rb_jgHashGetFloat(attr, "shape_damp", 150);

     jgSpringBodyInit(body, vects, numOfPoints, massPerPoint, position, angle, scale, isKinematic);
     body->isShapeMatching = isShapeMatching;
     body->edgeSpringK = edgeK;
     body->edgeSpringDamp = edgeDamp;
     body->shapeSpringK = shapeK;
     body->shapeSpringDamp = shapeDamp;
     return self;
}

static VALUE rb_jgSpringBodyAddSpring(VALUE self, VALUE springA, VALUE springB, VALUE k, VALUE damp)
{
     jgSpringBodyAddSpring(BODY(self), NUM2UINT(springA), NUM2UINT(springB), NUM2DBL(k), NUM2DBL(damp));
     return self;
}

void Init_jgSpringBody()
{
     c_jgSpringBody = rb_define_class_under(m_Jiggle, "SpringBody", c_jgBody);
     rb_define_alloc_func(c_jgSpringBody, rb_jgSpringBodyAlloc);

     rb_define_method(c_jgSpringBody, "initialize", rb_jgSpringBodyInitialize, -1);
     rb_define_method(c_jgSpringBody, "add_spring", rb_jgSpringBodyAddSpring, 4);
}
