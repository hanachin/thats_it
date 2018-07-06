/*
  copy from ruby source code
  https://github.com/ruby/ruby/blob/trunk/COPYING.ja
*/

#define VM_ENV_FLAG_LOCAL 0x0002
#define VM_ENV_DATA_INDEX_FLAGS      ( 0) /* ep[ 0] */
#define VM_ENV_DATA_INDEX_SPECVAL    (-1) /* ep[-1] */

/* for setup_it_block */

#define VM_TAGGED_PTR_REF(v, mask) ((void *)((v) & ~mask))

typedef struct rb_code_position_struct {
    int lineno;
    int column;
} rb_code_position_t;

typedef struct rb_code_location_struct {
    rb_code_position_t beg_pos;
    rb_code_position_t end_pos;
} rb_code_location_t;

typedef struct rb_iseq_location_struct {
    VALUE pathobj;      /* String (path) or Array [path, realpath]. Frozen. */
    VALUE base_label;   /* String */
    VALUE label;        /* String */
    VALUE first_lineno; /* TODO: may be unsigned short */
    rb_code_location_t code_location;
} rb_iseq_location_t;

typedef   signed long rb_snum_t;

struct rb_call_info {
    /* fixed at compile time */
    ID mid;
    unsigned int flag;
    int orig_argc;
};

struct rb_iseq_constant_body {
    enum iseq_type {
	ISEQ_TYPE_TOP,
	ISEQ_TYPE_METHOD,
	ISEQ_TYPE_BLOCK,
	ISEQ_TYPE_CLASS,
	ISEQ_TYPE_RESCUE,
	ISEQ_TYPE_ENSURE,
	ISEQ_TYPE_EVAL,
	ISEQ_TYPE_MAIN,
	ISEQ_TYPE_PLAIN
    } type;              /* instruction sequence type */

    unsigned int iseq_size;
    const VALUE *iseq_encoded; /* encoded iseq (insn addr and operands) */

    /**
     * parameter information
     *
     *  def m(a1, a2, ..., aM,                    # mandatory
     *        b1=(...), b2=(...), ..., bN=(...),  # optional
     *        *c,                                 # rest
     *        d1, d2, ..., dO,                    # post
     *        e1:(...), e2:(...), ..., eK:(...),  # keyword
     *        **f,                                # keyword_rest
     *        &g)                                 # block
     * =>
     *
     *  lead_num     = M
     *  opt_num      = N
     *  rest_start   = M+N
     *  post_start   = M+N+(*1)
     *  post_num     = O
     *  keyword_num  = K
     *  block_start  = M+N+(*1)+O+K
     *  keyword_bits = M+N+(*1)+O+K+(&1)
     *  size         = M+N+O+(*1)+K+(&1)+(**1) // parameter size.
     */

    struct {
	struct {
	    unsigned int has_lead   : 1;
	    unsigned int has_opt    : 1;
	    unsigned int has_rest   : 1;
	    unsigned int has_post   : 1;
	    unsigned int has_kw     : 1;
	    unsigned int has_kwrest : 1;
	    unsigned int has_block  : 1;

	    unsigned int ambiguous_param0 : 1; /* {|a|} */
	} flags;

	unsigned int size;

	int lead_num;
	int opt_num;
	int rest_start;
	int post_start;
	int post_num;
	int block_start;

	const VALUE *opt_table; /* (opt_num + 1) entries. */
	/* opt_num and opt_table:
	 *
	 * def foo o1=e1, o2=e2, ..., oN=eN
	 * #=>
	 *   # prologue code
	 *   A1: e1
	 *   A2: e2
	 *   ...
	 *   AN: eN
	 *   AL: body
	 * opt_num = N
	 * opt_table = [A1, A2, ..., AN, AL]
	 */

	const struct rb_iseq_param_keyword {
	    int num;
	    int required_num;
	    int bits_start;
	    int rest_start;
	    const ID *table;
	    const VALUE *default_values;
	} *keyword;
    } param;

    rb_iseq_location_t location;

    /* insn info, must be freed */
    struct iseq_insn_info {
	const struct iseq_insn_info_entry *body;
	unsigned int *positions;
	unsigned int size;
	struct succ_index_table *succ_index_table;
    } insns_info;

    ID *local_table;		/* must free */

    /* catch table */
    const struct iseq_catch_table *catch_table;

    /* for child iseq */
    const struct rb_iseq_struct *parent_iseq;
    struct rb_iseq_struct *local_iseq; /* local_iseq->flip_cnt can be modified */

    union iseq_inline_storage_entry *is_entries;
    struct rb_call_info *ci_entries; /* struct rb_call_info ci_entries[ci_size];
				      * struct rb_call_info_with_kwarg cikw_entries[ci_kw_size];
				      * So that:
				      * struct rb_call_info_with_kwarg *cikw_entries = &body->ci_entries[ci_size];
				      */
    struct rb_call_cache *cc_entries; /* size is ci_size = ci_kw_size */

    struct {
	rb_snum_t flip_count;
	VALUE coverage;
	VALUE *original_iseq;
    } variable;

    unsigned int local_table_size;
    unsigned int is_size;
    unsigned int ci_size;
    unsigned int ci_kw_size;
    unsigned int stack_max; /* for stack overflow check */

    /* The following fields are MJIT related info.  */
    VALUE (*jit_func)(struct rb_execution_context_struct *,
                      struct rb_control_frame_struct *); /* function pointer for loaded native code */
    long unsigned total_calls; /* number of total calls with `mjit_exec()` */
    struct rb_mjit_unit *jit_unit;
    char catch_except_p; /* If a frame of this ISeq may catch exception, set TRUE */
};

/* T_IMEMO/iseq */
/* typedef rb_iseq_t is in method.h */
struct rb_iseq_struct {
    VALUE flags;
    VALUE reserved1;
    struct rb_iseq_constant_body *body;

    union { /* 4, 5 words */
	/* struct iseq_compile_data *compile_data; /\* used at compile time *\/ */
	void *compile_data; /* used at compile time */

	struct {
	    VALUE obj;
	    int index;
	} loader;

	rb_event_flag_t trace_events;
    } aux;
};

typedef struct rb_iseq_struct rb_iseq_t;

struct rb_captured_block {
    VALUE self;
    const VALUE *ep;
    union {
	const rb_iseq_t *iseq;
	const struct vm_ifunc *ifunc;
	VALUE val;
    } code;
};


enum rb_block_type {
    block_type_iseq,
    block_type_ifunc,
    block_type_symbol,
    block_type_proc
};

enum rb_block_handler_type {
    block_handler_type_iseq,
    block_handler_type_ifunc,
    block_handler_type_symbol,
    block_handler_type_proc
};

struct rb_block {
  union {
    struct rb_captured_block captured;
    VALUE symbol;
    VALUE proc;
  } as;
  enum rb_block_type type;
};

typedef struct {
    const struct rb_block block;
    unsigned int is_from_method: 1;	/* bool */
    unsigned int is_lambda: 1;		/* bool */
} rb_proc_t;

/* for it */
enum method_missing_reason { FOO };

typedef struct rb_control_frame_struct {
    const VALUE *pc;
    VALUE *sp;
    const rb_iseq_t *iseq;
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
