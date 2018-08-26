#include "cache.h"

cache_t::cache_t() {
	cache_list_.clear();
	total_size_ = 0;
	size_ = 0;
}

cache_t::~cache_t() {
	cache_list_.clear();
	total_size_ = 0;
	size_ = 0;
}

void cache_t::read(void *buff, int size) {

}

void cache_t::write(void *buff, int size) {

}

void cache_t::finish() {

}