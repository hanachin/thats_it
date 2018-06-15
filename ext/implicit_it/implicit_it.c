#include "implicit_it.h"

VALUE rb_mImplicitIt;

void
Init_implicit_it(void)
{
  rb_mImplicitIt = rb_define_module("ImplicitIt");
}
