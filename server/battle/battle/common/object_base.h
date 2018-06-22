#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "stdafx.h"

#define PROPERTY(type, property)\
private:\
	type property_;\
public:\
	inline type get_##property() { return property_; }\
public:\
	inline void set_##property(type value) { property_ = value; }\

class object_base_t {
public:
	object_base_t(int32 id, int32 type_id);

	virtual	~object_base_t();

	PROPERTY(int32, type_id)

	PROPERTY(int32, id)
};

#endif