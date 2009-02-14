#include "rb_jiggle.h"

VALUE c_jgPointMass;

FLOAT_GET(rb_jgPointMassGetMass, POINTMASS, mass)
FLOAT_SET(rb_jgPointMassSetMass, POINTMASS, mass)

VECTOR_GET(rb_jgPointMassGetPosition, POINTMASS, position)
VECTOR_SET(rb_jgPointMassSetPosition, POINTMASS, position)

VECTOR_GET(rb_jgPointMassGetVelocity, POINTMASS, velocity)
VECTOR_SET(rb_jgPointMassSetVelocity, POINTMASS, velocity)

VECTOR_GET(rb_jgPointMassGetForce, POINTMASS, force)
VECTOR_SET(rb_jgPointMassSetForce, POINTMASS, force)

void Init_jgPointMass()
{
     c_jgPointMass = rb_define_class_under(m_Jiggle, "PointMass", rb_cObject);

     rb_define_method(c_jgPointMass, "mass" , rb_jgPointMassGetMass, 0);
     rb_define_method(c_jgPointMass, "mass=", rb_jgPointMassSetMass, 1);

     rb_define_method(c_jgPointMass, "position" , rb_jgPointMassGetPosition, 0);
     rb_define_method(c_jgPointMass, "position=", rb_jgPointMassSetPosition, 1);

     rb_define_method(c_jgPointMass, "velocity" , rb_jgPointMassGetVelocity, 0);
     rb_define_method(c_jgPointMass, "velocity=", rb_jgPointMassSetVelocity, 1);

     rb_define_method(c_jgPointMass, "force" , rb_jgPointMassGetForce, 0);
     rb_define_method(c_jgPointMass, "force=", rb_jgPointMassSetForce, 1);
}


