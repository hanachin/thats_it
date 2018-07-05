#ifndef IMPLICIT_IT_H
#define IMPLICIT_IT_H 1

#include "setjmp.h"
#include "ruby.h"
#include "ruby/ruby.h"
#include "ruby/version.h"

#endif /* IMPLICIT_IT_H */

#if RUBY_API_VERSION_CODE >= 20600
#include "implicit_it_20600.h"
#elif RUBY_API_VERSION_CODE >= 20500
#include "implicit_it_20500.h"
#endif
