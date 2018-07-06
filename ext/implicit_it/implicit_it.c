#include "implicit_it.h"

static int implicit_it_block_p(const rb_iseq_t *iseq) {
  ID id_it;
  if (!iseq) { return 0; }
  if (iseq->body->param.size) { return 0; }

  id_it = rb_intern("it");
  for (unsigned int i = 0; i < iseq->body->ci_size; i++) {
    if (id_it == iseq->body->ci_entries[i].mid) {
      return 1;
    }
  }
  return 0;
}

static VALUE it() {
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  return *(cfp + 2)->sp;
}

static VALUE setup_it_block_c_call() {
  const rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  rb_iseq_t *iseq = (rb_iseq_t *)(cfp + 2)->block_code;
  ID *ids;

  if (!iseq) { return Qnil; }
  if (!implicit_it_block_p(iseq)) { return Qnil; }

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


static inline enum rb_block_type block_type(const struct rb_block *block) {
    return block->type;
}

static const struct rb_block *proc_block(VALUE procval) {
  return &((rb_proc_t *)RTYPEDDATA_DATA(procval))->block;
}

static const rb_iseq_t *proc_iseq(VALUE procval);

static const rb_iseq_t *block_iseq(const struct rb_block *block) {
  switch (block_type(block)) {
    case block_type_iseq:
      return block->as.captured.code.iseq;
    case block_type_proc:
      return proc_iseq(block->as.proc);
    case block_type_ifunc:
    case block_type_symbol:
      break;
  }
  return NULL;
}

static const rb_iseq_t *proc_iseq(VALUE procval) {
  return block_iseq(proc_block(procval));
}

static int block_handler_type_iseq_p(VALUE block_handler) {
  return (block_handler & 0x03) == 0x01;
}

static int block_handler_type_ifunc_p(VALUE block_handler) {
  return (block_handler & 0x03) == 0x03;
}

static enum rb_block_handler_type block_handler_type(VALUE block_handler) {
  if (block_handler_type_iseq_p(block_handler)) {
    return block_handler_type_iseq;
  } else if (block_handler_type_ifunc_p(block_handler)) {
    return block_handler_type_ifunc;
  } else if (SYMBOL_P(block_handler)) {
    return block_handler_type_symbol;
  } else {
    if (!rb_obj_is_proc(block_handler)) {
      rb_raise(rb_eRuntimeError, "unknown block_handler type");
    }
    return block_handler_type_proc;
  }
}

static const rb_iseq_t *block_handler_iseq(VALUE block_handler) {
  switch (block_handler_type(block_handler)) {
    case block_handler_type_iseq:
      {
        struct rb_captured_block *captured = VM_TAGGED_PTR_REF(block_handler, 0x03);
        return captured->code.iseq;
      }
    case block_handler_type_proc:
      {
        return proc_iseq(block_handler);
      }
    case block_handler_type_ifunc:
    case block_handler_type_symbol:
      break;
  }
  return NULL;
}

static VALUE setup_it_block_call() {
  rb_control_frame_t *cfp = ruby_current_execution_context_ptr->cfp;
  VALUE block_handler = (cfp + 2)->ep[VM_ENV_DATA_INDEX_SPECVAL];
  const rb_iseq_t *iseq;
  ID *ids;

  if (!block_handler) { return Qnil; }

  iseq = block_handler_iseq(block_handler);

  if (!implicit_it_block_p(iseq)) { return Qnil; }

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
