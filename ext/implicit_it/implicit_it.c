#include "implicit_it.h"

static VALUE it() {
  /* trunk 10485be8c2acd97b165c5d6bd9e53122a821562d */
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  return *(cfp + 2)->sp;
}

static VALUE setup_it_block_c_call() {
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  rb_iseq_t *iseq = (rb_iseq_t *)(cfp + 2)->block_code;
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

static VALUE setup_it_block_call() {
  rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  VALUE block_handler = (cfp + 2)->ep[VM_ENV_DATA_INDEX_SPECVAL];
  struct rb_captured_block *captured;
  const rb_iseq_t *iseq;
  ID *ids;

  if (!block_handler) { return Qnil; }

  captured = VM_TAGGED_PTR_REF(block_handler, 0x03);
  iseq = captured->code.iseq;
  ids = (ID *)ALLOC_N(ID, 1);
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
  rb_define_global_function("setup_it_block_c_call", setup_it_block_c_call, 0);
  rb_define_global_function("setup_it_block_call", setup_it_block_call, 0);
  rb_define_global_function("it", it, 0);
}
