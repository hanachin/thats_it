#include "implicit_it.h"

static VALUE it() {
  /* trunk 10485be8c2acd97b165c5d6bd9e53122a821562d */
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  return *(cfp + 2)->sp;
}

static VALUE setup_it_block() {
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  VALUE block_handler = (cfp + 2)->ep[VM_ENV_DATA_INDEX_SPECVAL];
  struct rb_captured_block *captured = VM_TAGGED_PTR_REF(block_handler, 0x03);
  const rb_iseq_t *iseq = captured->code.iseq;
  ID *ids = (ID *)ALLOC_N(ID, 1);
  ids[0] = rb_intern("it");
  iseq->body->param.size = 1;
  iseq->body->param.flags.has_lead = 1;
  iseq->body->param.lead_num = 1;
  iseq->body->param.flags.ambiguous_param0 = 1;
  iseq->body->local_table_size = 1;
  ruby_xfree((void *)iseq->body->local_table);
  iseq->body->local_table = ids;
  return Qnil;
}

void
Init_implicit_it(void)
{
  rb_define_global_function("setup_it_block", setup_it_block, 0);
  rb_define_global_function("it", it, 0);
}
