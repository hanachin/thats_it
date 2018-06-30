#ifndef IMPLICIT_IT_H
#define IMPLICIT_IT_H 1

#include "setjmp.h"
#include "ruby.h"

#endif /* IMPLICIT_IT_H */

#define VM_ENV_DATA_INDEX_SPECVAL    (-1) /* ep[-1] */

enum method_missing_reason { FOO };

typedef struct rb_control_frame_struct {
    const VALUE *pc;
    VALUE *sp;
    /* const rb_iseq_t *iseq; */
    const void *iseq;
    VALUE self;
    const VALUE *ep;
    const void *block_code;
    const VALUE *bp;
} rb_control_frame_t;

typedef struct rb_execution_context_struct {
    VALUE *vm_stack;
    size_t vm_stack_size;
    rb_control_frame_t *cfp;
    /* struct rb_vm_tag *tag; */
    /* struct rb_vm_protect_tag *protect_tag; */
    void *tag;
    void *protect_tag;
    int raised_flag;
    /* rb_atomic_t interrupt_flag; */
    /* rb_atomic_t interrupt_mask; */
    unsigned int interrupt_flag;
    unsigned int interrupt_mask;
    /* rb_fiber_t *fiber_ptr; */
    /* struct rb_thread_struct *thread_ptr; */
    /* st_table *local_storage; */
    void *fiber_ptr;
    void *thread_ptr;
    void *local_storage;
    VALUE local_storage_recursive_hash;
    VALUE local_storage_recursive_hash_for_trace;
    const VALUE *root_lep;
    VALUE root_svar;
    /* rb_ensure_list_t *ensure_list; */
    /* struct rb_trace_arg_struct *trace_arg; */
    void *ensure_list;
    void *trace_arg;
    VALUE errinfo;
    VALUE passed_block_handler;
    /* const rb_callable_method_entry_t *passed_bmethod_me; */
    /* enum method_missing_reason method_missing_reason; */
    void *passed_bmethod_me;
    enum method_missing_reason method_missing_reason;
    struct {
 VALUE *stack_start;
 VALUE *stack_end;
 size_t stack_maxsize;
 __attribute__((__aligned__(8))) jmp_buf regs;
    } machine;
} rb_execution_context_t;

extern rb_execution_context_t *ruby_current_execution_context_ptr;
