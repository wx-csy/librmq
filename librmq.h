#ifndef __LIBRMQ_H__
#define __LIBRMQ_H__

#include "librmq/naive.h"
#include "librmq/st.h"
#include "librmq/block.h"
#include "librmq/ind.h"
#include "librmq/zkw.h"

namespace librmq {

template <typename T>
using rmq_default = rmq_block<T>;

}

#endif
