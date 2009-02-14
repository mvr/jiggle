#include "rb_jiggle.h"

VALUE m_Jiggle;

VALUE rb_jgHashGet(VALUE hash, char *key)
{
     VALUE v;
     
     if (!NIL_P(hash))
     {
	  v = rb_hash_aref(hash, ID2SYM(rb_intern(key)));
	  if (!NIL_P(v)) return v;
     }
     
     return Qnil;
}

int rb_jgHashGetInt(VALUE hash, char *key, int de)	
{							
     VALUE v = rb_jgHashGet(hash, key);		
     if (!NIL_P(v)) return NUM2INT(v);		
     return de;					
} 

double rb_jgHashGetFloat(VALUE hash, char *key, double de)	
{							
     VALUE v = rb_jgHashGet(hash, key);		
     if (!NIL_P(v)) return NUM2DBL(v);		
     return de;					
} 

jgVector2 rb_jgHashGetVector2(VALUE hash, char *key, jgVector2 de)	
{							
     VALUE v = rb_jgHashGet(hash, key);		
     if (!NIL_P(v)) return *VGET(v);		
     return de;					
} 

bool rb_jgHashGetBool(VALUE hash, char *key, bool de)
{
     VALUE v = rb_jgHashGet(hash, key);		
     if (!NIL_P(v)) return VAL2BOOL(v);		
     return de;					
}

void Init_jiggle(void)
{
     m_Jiggle = rb_define_module("Jiggle");

     Init_jgVector2();
     Init_jgPointMass();
     Init_jgBody();
     Init_jgSpringBody();
     Init_jgWorld();
}
