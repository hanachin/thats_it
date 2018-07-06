#ifndef THATS_IT_H
#define THATS_IT_H 1

#include "setjmp.h"
#include "ruby.h"
#include "ruby/ruby.h"
#include "ruby/version.h"

#endif /* THATS_IT_H */

#if RUBY_API_VERSION_CODE >= 20600
#include "thats_it_20600.h"
#elif RUBY_API_VERSION_CODE >= 20500
#include "thats_it_20500.h"
#endif
