#include "object_base.h"

object_base_t::object_base_t(int32 object_id, int32 type_id) {
	object_id_ = object_id;
	type_id_ = type_id;
}

object_base_t::~object_base_t() {
	object_id_ = 0;
	type_id_ = 0;
}