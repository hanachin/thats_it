#include "implicit_it.h"

static VALUE it() {
  /* trunk 10485be8c2acd97b165c5d6bd9e53122a821562d */
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  return *(cfp + 2)->sp;
}

void
Init_implicit_it(void)
{
  rb_define_global_function("it", it, 0);
}
