#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////
// main => thread 
template <typename _t>
void post_cmd() {
	output_chunk_t *chunk = output_pool_alloc();
	chunk->conn_ = NULL;
	_t *cmd = (_t *)chunk->buff_;
	new (cmd) _t;
	output_queue_.push(chunk);
}

template <typename _t, typename _p1>
void post_cmd(_p1 p1) {
	output_chunk_t *chunk = output_pool_alloc();
	chunk->conn_ = NULL;
	_t *cmd = (_t *)chunk->buff_;
	new (cmd) _t(p1);
	output_queue_.push(chunk);
}

template <typename _t, typename _p1, typename _p2>
void post_cmd(_p1 p1, _p2 p2) {
	output_chunk_t *chunk = output_pool_alloc();
	chunk->conn_ = NULL;
	_t *cmd = (_t *)chunk->buff_;
	new (cmd) _t(p1, p2);
	output_queue_.push(chunk);
}

///////////////////////////////////////////////////////////////////////////////////////
// thread => main
template <typename _t>
void thread_post_cmd() {
	input_chunk_t *chunk = input_pool_alloc();
	chunk->conn_ = NULL;
	_t *cmd = (_t *)chunk->buff_;
	new (cmd) _t;
	input_queue_.push(chunk);
}

template <typename _t, typename _p1>
void thread_post_cmd(_p1 p1) {
	input_chunk_t *chunk = input_pool_alloc();
	chunk->conn_ = NULL;
	_t *cmd = (_t *)chunk->buff_;
	new (cmd) _t(p1);
	input_queue_.push(chunk);
}

template <typename _t, typename _p1, typename _p2>
void thread_post_cmd(_p1 p1, _p2 p2) {
	input_chunk_t *chunk = input_pool_alloc();
	chunk->conn_ = NULL;
	_t *cmd = (_t *)chunk->buff_;
	new (cmd) _t(p1, p2);
	input_queue_.push(chunk);
}