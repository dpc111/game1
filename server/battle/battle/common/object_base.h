#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#define DEF_PROPERTY(type, property)\
private:\
	#type #property_;\
public:\
	#type get_#property() { return #property_; }\
public:\
	void set_#property(#type value) { #property_ = value; }\

#include "stdafx.h"

class object_base_t {
public:
	object_base_t(int32 object_id, int32 type_id);

	virtual	~object_base_t();

	int32 get_id() { return object_id_; }

	// int32 get_type_id() { return type_id_; }

	DEF_PROPERTY(int32, type_id)

private:
	int32 object_id_;

	// int32 type_id_;
};

#endif