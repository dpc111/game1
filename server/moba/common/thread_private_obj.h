#ifndef THREAD_PRIVATE_OBJ_H
#define THREAD_PRIVATE_OBJ_H

#include "obj_pool.h"
#include "thread.h"

template <typename _t>
class thread_private_obj_t {
public:
	_t *self() {
		_t *obj = (_t *)tls_.get();
		if (obj == NULL) {
			obj = new _t;
			tls_.set(obj);
		}
		return obj;
	}

private:
	tls_t tls_;
};

#endif
