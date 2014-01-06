#pragma once

#define MYE_MULTITHREAD_OFF 0
#define MYE_MULTITHREAD_BOOST 1

#define MYE_MULTITHREAD MYE_MULTITHREAD_OFF

#if MYE_MULTITHREAD == MYE_MULTITHREAD_OFF

#define MYE_RECURSIVE_MUTEX(x)
#define MYE_RECURSIVE_MUTEX_LOCK(x)
#define MYE_RECURSIVE_MUTEX_UNLOCK(x)

#elif MYE_MULTITHREAD == MYE_MULTITHREAD_BOOST

#include <boost/thread/recursive_mutex.hpp>

#define MYE_RECURSIVE_MUTEX(x) mutable boost::recursive_mutex x;
#define MYE_RECURSIVE_MUTEX_LOCK(x) (x).lock()
#define MYE_RECURSIVE_MUTEX_UNLOCK(x) (x).unlock()

#endif