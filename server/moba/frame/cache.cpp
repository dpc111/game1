#include "cache.h"

cache_t::cache_t() {
	cache_list_.clear();
	size_ = 0;
}

cache_t::~cache_t() {
	cache_list_.clear();
	size_ = 0;
}