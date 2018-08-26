#ifndef THREAD_PRIVATE_POOL_H
#define THREAD_PRIVATE_POOL_H

#include "obj_pool.h"
#include "thread.h"

template <typename _t>
class thread_private_pool_t {
public:
	typedef obj_pool_t<_t> _t_pool_t;

	_t* alloc() {
		_t_pool_t *pool = (_t_pool_t *)tls_.get();
		if (pool == NULL) {
			pool = new _t_pool_t;
			tls_.set(pool);
		}
		_t *data = pool->alloc();
		assert(data);
		return data;
	}

	void free(_t *data) {
		_t_pool_t *pool = (_t_pool_t *)tls_.get();
		assert(pool);
		pool->free(data);
	}

private:
	tls_t tls_;
}

#endif