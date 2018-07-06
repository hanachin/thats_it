/*
  copy from ruby source code
  https://github.com/ruby/ruby/blob/trunk/COPYING.ja
*/

#define VM_ENV_DATA_INDEX_SPECVAL    (-1) /* ep[-1] */

/* for setup_it_block */
#define VM_TAGGED_PTR_REF(v, mask) ((void *)((v) & ~mask))

typedef struct rb_code_location_struct {
    int lineno;
    int column;
} rb_code_location_t;

typedef struct rb_code_range_struct {
    rb_code_location_t first_loc;
    rb_code_location_t last_loc;
} rb_code_range_t;

typedef struct rb_iseq_location_struct {
    VALUE pathobj;      /* String (path) or Array [path, realpath]. Frozen. */
    VALUE base_label;   /* String */
    VALUE label;        /* String */
    VALUE first_lineno; /* TODO: may be unsigned short */
    rb_code_range_t code_range;
} rb_iseq_location_t;

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
	ISEQ_TYPE_DEFINED_GUARD
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
    const struct iseq_insn_info_entry *insns_info;

    const ID *local_table;		/* must free */

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

    VALUE mark_ary;     /* Array: includes operands which should be GC marked */

    unsigned int local_table_size;
    unsigned int is_size;
    unsigned int ci_size;
    unsigned int ci_kw_size;
    unsigned int insns_info_size;
    unsigned int stack_max; /* for stack overflow check */
};

/* T_IMEMO/iseq */
/* typedef rb_iseq_t is in method.h */
struct rb_iseq_struct {
    VALUE flags;
    VALUE reserved1;
    struct rb_iseq_constant_body *body;

    union { /* 4, 5 words */
	/* struct iseq_compile_data *compile_data; /\* used at compile time *\/ */
        void *compile_data;

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
    const VALUE *pc;		/* cfp[0] */
    VALUE *sp;			/* cfp[1] */
    const rb_iseq_t *iseq;	/* cfp[2] */
    VALUE self;			/* cfp[3] / block[0] */
    const VALUE *ep;		/* cfp[4] / block[1] */
    const void *block_code;     /* cfp[5] / block[2] */ /* iseq or ifunc */
} rb_control_frame_t;

typedef struct rb_execution_context_struct {
    /* execution information */
    VALUE *vm_stack;		/* must free, must mark */
    size_t vm_stack_size;       /* size in word (byte size / sizeof(VALUE)) */
    rb_control_frame_t *cfp;

    /* struct rb_vm_tag *tag; */
    /* struct rb_vm_protect_tag *protect_tag; */
    void *tag;
    void *protect_tag;
    int safe_level;
    int raised_flag;

    /* interrupt flags */
    /* rb_atomic_t interrupt_flag; */
    unsigned long interrupt_flag;
    unsigned long interrupt_mask;

    /* rb_fiber_t *fiber_ptr; */
    /* struct rb_thread_struct *thread_ptr; */
    void *fiber_ptr;
    void *thread_ptr;

    /* storage (ec (fiber) local) */
    /* st_table *local_storage; */
    void *local_storage;
    VALUE local_storage_recursive_hash;
    VALUE local_storage_recursive_hash_for_trace;

    /* eval env */
    const VALUE *root_lep;
    VALUE root_svar;

    /* ensure & callcc */
    /* rb_ensure_list_t *ensure_list; */
    void *ensure_list;

    /* trace information */
    /* struct rb_trace_arg_struct *trace_arg; */
    void *trace_arg;

    /* temporary places */
    VALUE errinfo;
    VALUE passed_block_handler; /* for rb_iterate */
    /* const rb_callable_method_entry_t *passed_bmethod_me; /\* for bmethod *\/ */
    /* enum method_missing_reason method_missing_reason; */
    void *passed_bmethod_me;
    enum method_missing_reason method_missing_reason;

    /* for GC */
    struct {
	VALUE *stack_start;
	VALUE *stack_end;
	size_t stack_maxsize;
#ifdef __ia64
	VALUE *register_stack_start;
	VALUE *register_stack_end;
	size_t register_stack_maxsize;
#endif
	jmp_buf regs;
    } machine;
} rb_execution_context_t;

extern rb_execution_context_t *ruby_current_execution_context_ptr;
