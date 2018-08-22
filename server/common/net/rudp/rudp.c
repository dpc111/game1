#include "rudp.h"
#include "udp_pool.h"

struct udp_chunk_queue_t {
	struct udp_chunk_t *head;
	struct udp_chunk_t *tail;
};

struct udp_handle_t {
	struct udp_chunk_queue_t recv_queue;								// 接收队列
	struct udp_chunk_t *recv_cur_in; 									// 网络写
	struct udp_chunk_t *recv_cur_out;				 					// 逻辑读
	int recv_max_seq;
	int recv_min_seq;
	int recv_max_ack;

	struct udp_chunk_queue_t send_queue;								// 发送队列
	struct udp_chunk_queue_t send_history;								// 发送历史
	struct udp_chunk_t *send_cur_in;									// 逻辑写
	struct udp_chunk_t *send_cur_out;									// 网络读
	int send_seq;
	
	int64 req_tick;
	int req_times;

	struct udp_chunk_pool_t *pool;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
push_queue_front(struct udp_chunk_queue_t *q, struct udp_chunk_t *c) {
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
push_queue_back(struct udp_chunk_queue_t *q, struct udp_chunk_t *c) {
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
push_queue_after(struct udp_chunk_queue_t *q, struct udp_chunk_t *prev, struct udp_chunk_t *c) {
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
push_queue_back_sort(struct udp_chunk_queue_t *q, struct udp_chunk_t *c) {
	struct udp_chunk_t *tmp = q->tail;
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
push_queue_back_sort_check_same(struct udp_chunk_queue_t *q, struct udp_chunk_t *c) {
	struct udp_chunk_t *tmp = q->tail;
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

struct udp_chunk_t* 
pop_queue_front(struct udp_chunk_queue_t *q) {
	if (q->head	== NULL) {
		return NULL;
	}
	struct udp_chunk_t *c = q->head;
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

struct udp_chunk_t*
pop_queue_by_seq(struct udp_chunk_queue_t *q, int seq) {
	struct udp_chunk_t *tmp = q->tail;
	while (tmp != NULL) {
		if (tmp->seq == seq) {
			if (tmp->next == NULL) {
				h->tail = tmp->prev;
			} else {
				tmp->next->prev = tmp->prev;
			}
			if (tmp->prev == NULL) {
				h->head = tmp->next;
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
recv_min_seq_update(struct udp_handle_t *h) {
	struct udp_chunk_t *c = h->recv_queue.head;
	while (c != NULL) {
		if (c->seq > h->recv_min_seq) {
			if (c->seq == h->recv_min_seq + 1) {
				h->recv_min_seq = c->seq;
				h->req_times = 0
			} else {
				return;
			}
		}
		c = c->next;
	}
}

void* 
revc_buff_in(struct udp_handle_t *h) {
	if (h->recv_cur_in != NULL) {
		return h->recv_cur_in;
	}
	h->recv_cur_in = chunk_pool_malloc(h->pool);
	return h->recv_cur_in;
}

void
revc_buff_in_process(struct udp_handle_t *h, int sz) {
	struct udp_chunk_t *c = h->recv_cur_in;
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
			struct udp_chunk_t *c_again = pop_queue_by_seq(&h->send_history, c->ack);
			if (c_again == NULL) {
				break;
			}
			push_queue_front(&h->send_queue, c_again);
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
			}
			break;
		default :
			break;
	}
}

struct udp_chunk_t*
recv_cur_out(struct udp_handle_t *h) {
	if (h->recv_cur_out != NULL) {
		return h->recv_cur_out;
	}
	struct udp_chunk_t *c = h->recv_queue.head;
	if (c->seq <= h->recv_min_seq) { 
		h->recv_cur_out = pop_queue_front(&h->recv_queue);
	}
	return h->recv_cur_out;
}

void
recv_cur_out_process(struct udp_handle_t *h) {
	if (h->recv_cur_out == NULL) {
		return;
	}
	chunk_pool_free(h->pool, h->recv_cur_out);
	h->recv_cur_out = NULL;
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
send_history_clear(struct udp_handle_t *h) {
	struct udp_chunk_t *c = h->send_history.head;
	while (c != NULL && c->seq <= h->recv_max_ack) {
		c = pop_queue_front(h->send_history);
		chunk_pool_free(h->pool, c);
		c = h->send_history.head;
	}
}

void
send_req_again(struct udp_handle_t *h) {
	if (h->recv_min_seq == h->recv_max_seq) {
		return;
	}
	struct udp_chunk_t *c = chunk_pool_malloc(h->pool);
	c->size = 0;
	c->type = UDP_TYPE_REQ_SEQ;
	c->seq = 0;
	c->ack = h->recv_min_seq + 1;
	push_queue_front(&h->send_queue, c);
}

void*
send_buff_in(struct udp_handle_t *h) {
	if (h->send_cur_in != NULL) {
		return h->send_cur_in->buff;
	}
	h->send_cur_in = chunk_pool_malloc(h->pool);
	return h->send_cur_in->buff;
}

void
send_buff_in_process(struct udp_handle_t *h, int size) {
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

struct udp_chunk_t *
send_buff_out(struct udp_handle_t *h) {
	if (h->send_cur_out != NULL) {
		return h->send_cur_out;
	}
	h->send_cur_out = pop_queue_front(&h->send_queue);
	return h->send_cur_out;
}

void
send_buff_out_process(struct udp_handle_t *h) {
	if (h->send_cur_out == NULL) {
		return;
	}
	push_queue_back_sort(&h->send_history, h->send_cur_out);		
	h->send_cur_out = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
udp_process(struct udp_handle_t *h, int64 tick) {
	if (h->recv_max_seq - h->recv_min_seq > UDP_RECV_SEQ_MAX_DEV) {
		return -1;
	}
	if (h->req_times > UDP_REQ_AGAIN_MAX_TIMES) {
		return -1;	
	}
	if (tick - h->req_tick > UDP_REQ_AGAIN_TICKS) {
		send_req_again(h);
		++h->req_times;
	}
	return 0;
}