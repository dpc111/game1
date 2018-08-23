#include "rudp.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_history_clear(udp_handle_t *h);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
push_queue_front(udp_chunk_queue_t *q, udp_chunk_t *c) {
	if (q->head == NULL) {
		q->head = c;
		q->tail = c;
		c->next = NULL;
		c->prev = NULL;
	} else {
		c->next = q->head;
		c->prev = NULL;
		q->head->prev = c;
		q->head = c; 
	}
}

void 
push_queue_back(udp_chunk_queue_t *q, udp_chunk_t *c) {
	if (q->head == NULL) {
		q->head = c;
		q->tail = c;
		c->next = NULL;
		c->prev = NULL;
	} else {
		c->prev = q->tail;
		c->next = NULL;
		q->tail->next = c;
		q->tail = c;
	}
}

void 
push_queue_after(udp_chunk_queue_t *q, udp_chunk_t *prev, udp_chunk_t *c) {
	if (q->head == NULL) {
		q->head = c;
		q->tail = c;
		c->next = NULL;
		c->prev = NULL;
	} else {
		if (prev == NULL) {
			c->next = q->head;
			c->prev = NULL;
			q->head->prev = c;
			q->head = c;
		} else {
			c->next = prev->next;
			if (c->next != NULL) {
				c->next->prev = c;
			} else {
				q->tail = c;
			}
			prev->next = c;
			c->prev = prev;
		}
	}
}

void
push_queue_back_sort(udp_chunk_queue_t *q, udp_chunk_t *c) {
	udp_chunk_t *tmp = q->tail;
	while (tmp != NULL) {
		if (tmp->seq <= c->seq) {
			push_queue_after(q, tmp, c);
			return;
		}
		tmp = tmp->prev;
	}
	push_queue_after(q, NULL, c);
}

bool
push_queue_back_sort_check_same(udp_chunk_queue_t *q, udp_chunk_t *c) {
	udp_chunk_t *tmp = q->tail;
	while (tmp != NULL) {
		if (tmp->seq == c->seq) {
			return false;
		}
		if (tmp->seq < c->seq) {
			push_queue_after(q, tmp, c);
			return true;
		}
		tmp = tmp->prev;
	}
	push_queue_after(q, NULL, c);
	return true;
}

udp_chunk_t* 
pop_queue_front(udp_chunk_queue_t *q) {
	if (q->head	== NULL) {
		return NULL;
	}
	udp_chunk_t *c = q->head;
	q->head = c->next;
	if (q->head == NULL) {
		q->tail = NULL;
	} else {
		q->head->prev = NULL;
	}
	c->next = NULL;
	c->prev = NULL;
	return c;
}

udp_chunk_t*
pop_queue_by_seq(udp_chunk_queue_t *q, int seq) {
	udp_chunk_t *tmp = q->tail;
	while (tmp != NULL) {
		if (tmp->seq == seq) {
			if (tmp->next == NULL) {
				q->tail = tmp->prev;
			} else {
				tmp->next->prev = tmp->prev;
			}
			if (tmp->prev == NULL) {
				q->head = tmp->next;
			} else {
				tmp->prev->next = tmp->next;
			}
			tmp->next = NULL;
			tmp->prev = NULL;
			return tmp;
		}
		tmp = tmp->prev;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
recv_min_seq_update(udp_handle_t *h) {
	udp_chunk_t *c = h->recv_queue.head;
	while (c != NULL) {
		if (c->seq > h->recv_min_seq) {
			if (c->seq == h->recv_min_seq + 1) {
				h->recv_min_seq = c->seq;
				h->req_times = 0;
			} else {
				return;
			}
		}
		c = c->next;
	}
}

void* 
recv_buff_in(udp_handle_t *h) {
	if (h->recv_cur_in != NULL) {
		return h->recv_cur_in;
	}
	h->recv_cur_in = chunk_pool_malloc(h->pool);
	return h->recv_cur_in;
}

void
recv_buff_in_process(udp_handle_t *h, int sz) {
	udp_chunk_t *c = h->recv_cur_in;
	if (c == NULL) {
		return;
	}
	if (sz < UDP_HEAD_BYTE_ALL 
		|| sz > UDP_DATA_MAX_LEN + UDP_HEAD_BYTE_ALL) {
		// ERROR
		return;
	}
	if (c->size + UDP_HEAD_BYTE_ALL != sz) {
		// ERROR
		return;
	}

	switch (c->type) {
		case UDP_TYPE_REQ_SEQ:
			{
				udp_chunk_t *c_again = pop_queue_by_seq(&h->send_history, c->ack);
				if (c_again == NULL) {
					break;
				}
				push_queue_front(&h->send_queue, c_again);
			}
			break;
		case UDP_TYPE_SYN_ACK:
			break;
		case UDP_TYPE_DATA:
		case UDP_TYPE_DATA_AGAIN:
			if (h->recv_min_seq >= c->seq) {
				break;
			}
			if (push_queue_back_sort_check_same(&h->recv_queue, c)) {
				h->recv_cur_in = NULL;
				if (h->recv_min_seq < c->seq) {
					recv_min_seq_update(h);					
				}
				if (h->recv_max_seq < c->seq) {
					h->recv_max_seq = c->seq;
				}
				if (h->recv_max_ack < c->ack) {
					h->recv_max_ack = c->ack;
					send_history_clear(h);
				}
				h->recv_tick = h->cur_tick; 
			}
			break;
		default :
			break;
	}
}

void 
recv_buff_write(udp_handle_t *h, udp_chunk_t *c) {
	switch (c->type) {
		case UDP_TYPE_REQ_SEQ:
			{
				udp_chunk_t *c_again = pop_queue_by_seq(&h->send_history, c->ack);
				if (c_again == NULL) {
					break;
				}
				push_queue_front(&h->send_queue, c_again);
			}
			break;
		case UDP_TYPE_SYN_ACK:
			break;
		case UDP_TYPE_DATA:
		case UDP_TYPE_DATA_AGAIN:
			if (h->recv_min_seq >= c->seq) {
				break;
			}
			if (push_queue_back_sort_check_same(&h->recv_queue, c)) {
				h->recv_cur_in = NULL;
				if (h->recv_min_seq < c->seq) {
					recv_min_seq_update(h);					
				}
				if (h->recv_max_seq < c->seq) {
					h->recv_max_seq = c->seq;
				}
				if (h->recv_max_ack < c->ack) {
					h->recv_max_ack = c->ack;
					send_history_clear(h);
				}
				h->recv_tick = h->cur_tick; 
			}
			break;
		default :
			break;
	}
}

udp_chunk_t*
recv_buff_out(udp_handle_t *h) {
	if (h->recv_cur_out != NULL) {
		return h->recv_cur_out;
	}
	udp_chunk_t *c = h->recv_queue.head;
	if (c->seq <= h->recv_min_seq) { 
		h->recv_cur_out = pop_queue_front(&h->recv_queue);
	}
	return h->recv_cur_out;
}

void
recv_buff_out_process(udp_handle_t *h) {
	if (h->recv_cur_out == NULL) {
		return;
	}
	chunk_pool_free(h->pool, h->recv_cur_out);
	h->recv_cur_out = NULL;
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
send_history_clear(udp_handle_t *h) {
	udp_chunk_t *c = h->send_history.head;
	while (c != NULL && c->seq <= h->recv_max_ack) {
		c = pop_queue_front(&h->send_history);
		chunk_pool_free(h->pool, c);
		c = h->send_history.head;
	}
}

void
send_req_again(udp_handle_t *h) {
	if (h->recv_min_seq == h->recv_max_seq) {
		return;
	}
	udp_chunk_t *c = chunk_pool_malloc(h->pool);
	c->size = 0;
	c->type = UDP_TYPE_REQ_SEQ;
	c->seq = 0;
	c->ack = h->recv_min_seq + 1;
	push_queue_front(&h->send_queue, c);
	++h->req_times;
}

void*
send_buff_in(udp_handle_t *h) {
	if (h->send_cur_in != NULL) {
		return h->send_cur_in->buff;
	}
	h->send_cur_in = chunk_pool_malloc(h->pool);
	return h->send_cur_in->buff;
}

void
send_buff_in_process(udp_handle_t *h, int size) {
	udp_chunk_t *c = h->send_cur_in;
	if (c == NULL) {
		return;
	}
	if (size > UDP_DATA_MAX_LEN) {
		// ERROR
		return;
	}
	c->size = size;
	c->type = UDP_TYPE_DATA;
	c->seq = ++h->send_seq;
	c->ack = h->recv_min_seq;
	push_queue_back(&h->send_queue, c);
	h->send_cur_in = NULL;
}

void 
send_buff_write(udp_handle_t *h, void *buff, int size) {
	assert(size <= UDP_DATA_MAX_LEN);
	udp_chunk_t *c = chunk_pool_malloc(h->pool);
	c->size = size;
	c->type = UDP_TYPE_DATA;
	c->seq = ++h->send_seq;
	c->ack = h->recv_min_seq;
	memcpy(c->buff, buff, size);
	push_queue_back(&h->send_queue, c);
}

void 
send_chunk_force(udp_handle_t *h, udp_chunk_t *c) {
	push_queue_front(&h->send_queue, c);
}

udp_chunk_t *
send_buff_out(udp_handle_t *h) {
	if (h->send_cur_out != NULL) {
		return h->send_cur_out;
	}
	h->send_cur_out = pop_queue_front(&h->send_queue);
	return h->send_cur_out;
}

void
send_buff_out_process(udp_handle_t *h) {
	if (h->send_cur_out == NULL) {
		return;
	}
	push_queue_back_sort(&h->send_history, h->send_cur_out);		
	h->send_cur_out = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void 
init_udp_handle(udp_handle_t *h, udp_chunk_pool_t *pool, int64 tick) {
	udp_chunk_t *c = pop_queue_front(&h->recv_queue);
	while (c != NULL) {
		chunk_pool_free(pool, c);
		c = pop_queue_front(&h->recv_queue);
	}
	if (h->recv_cur_in != NULL) {
		chunk_pool_free(pool, h->recv_cur_in);
		h->recv_cur_in = NULL;
	}
	if (h->recv_cur_out != NULL) {
		chunk_pool_free(pool, h->recv_cur_out);
		h->recv_cur_out = NULL;
	}
	h->recv_max_seq = 0;
	h->recv_min_seq = 0;
	h->recv_max_ack = 0;
	c = pop_queue_front(&h->send_queue);
	while (c != NULL) {
		chunk_pool_free(pool, c);
		c = pop_queue_front(&h->send_queue);
	}
	c = pop_queue_front(&h->send_history);
	while (c != NULL) {
		chunk_pool_free(pool, c);
		c = pop_queue_front(&h->send_history);
	}
	if (h->send_cur_in != NULL) {
		chunk_pool_free(pool, h->send_cur_in);
		h->send_cur_in = NULL;
	}
	if (h->send_cur_out != NULL) {
		chunk_pool_free(pool, h->send_cur_out);
		h->send_cur_out = NULL;
	}
	h->send_seq = 0;
	h->pool = pool;
	h->cur_tick = tick;
	h->recv_tick = tick;
	h->req_tick = 0;
	h->req_times = 0;
}

void 
destroy_udp_handle(udp_handle_t *h) {
	if (h->pool == NULL) {
		return;
	}
	udp_chunk_t *c = pop_queue_front(&h->recv_queue);
	while (c != NULL) {
		chunk_pool_free(h->pool, c);
		c = pop_queue_front(&h->recv_queue);
	}
	if (h->recv_cur_in != NULL) {
		chunk_pool_free(h->pool, h->recv_cur_in);
		h->recv_cur_in = NULL;
	}
	if (h->recv_cur_out != NULL) {
		chunk_pool_free(h->pool, h->recv_cur_out);
		h->recv_cur_out = NULL;
	}
	h->recv_max_seq = 0;
	h->recv_min_seq = 0;
	h->recv_max_ack = 0;
	c = pop_queue_front(&h->send_queue);
	while (c != NULL) {
		chunk_pool_free(h->pool, c);
		c = pop_queue_front(&h->send_queue);
	}
	c = pop_queue_front(&h->send_history);
	while (c != NULL) {
		chunk_pool_free(h->pool, c);
		c = pop_queue_front(&h->send_history);
	}
	if (h->send_cur_in != NULL) {
		chunk_pool_free(h->pool, h->send_cur_in);
		h->send_cur_in = NULL;
	}
	if (h->send_cur_out != NULL) {
		chunk_pool_free(h->pool, h->send_cur_out);
		h->send_cur_out = NULL;
	}
	h->send_seq = 0;
	h->pool = NULL;
	h->cur_tick = 0;
	h->recv_tick = 0;
	h->req_tick = 0;
	h->req_times = 0;
}

int
udp_process(udp_handle_t *h, int64 tick) {
	h->cur_tick = tick;
	if (h->cur_tick - h->recv_tick > UDP_TIME_OUT_TICKS) {
		return -1;
	}
	if (h->recv_max_seq - h->recv_min_seq > UDP_RECV_SEQ_MAX_DEV) {
		return -1;
	}
	if (h->req_times > UDP_REQ_AGAIN_MAX_TIMES) {
		return -1;	
	}
	if (h->cur_tick - h->req_tick > UDP_REQ_AGAIN_TICKS) {
		send_req_again(h);
		h->req_tick = h->cur_tick;
	}
	return 0;
}