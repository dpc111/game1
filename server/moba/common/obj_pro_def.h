#include "stdafx.h"

#define PROPERTY(type, property)\
private:\
	type property##_;\
public:\
	inline type get_##property() { return property##_; }\
public:\
	inline void set_##property(type value) { property##_ = value; }\

#define PROPERTY_QUOTE(type, property)\
private:\
	type property##_;\
public:\
	inline type & get_##property() { return property##_; }\
public:\
	inline void set_##property(const type & value) { property##_ = value; }\

#define PROPERTY_PRO(type, property)\
protected:\
	type property##_;\
public:\
	inline type get_##property() { return property##_; }\
public:\
	inline void set_##property(type value) { property##_ = value; }\

#define PROPERTY_PRO_QUOTE(type, property)\
protected:\
	type property##_;\
public:\
	inline type & get_##property() { return property##_; }\
public:\
	inline void set_##property(const type & value) { property##_ = value; }\