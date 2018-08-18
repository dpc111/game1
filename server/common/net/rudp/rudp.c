struct chunk_queue_t {
	struct chunk_t *head;
	struct chunk_t *tail;
};

struct handle_t {
	struct chunk_queue_t recv_queue;								// 接收队列
	struct chunk_t *recv_cur; 
	int recv_max_seq;
	int recv_min_seq;
	int recv_max_ack;

	struct chunk_queue_t send_queue;								// 发送队列
	struct chunk_queue_t send_history;								// 发送历史
	struct chunk_t *send_cur_in;									// 当前逻辑写入
	struct chunk_t *send_cur_out;									// 当前网络写出		
	int send_seq;

	int max_ack;
	
	struct chunk_pool_t *pool;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void 
push_queue_back(struct chunk_queue_t *q, struct chunk_t *c) {
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

struct chunk_t* 
pop_queue(struct chunk_queue_t *q) {
	if (q->head	== NULL) {
		return NULL;
	}
	struct chunk_t *c = q->head;
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

void 
insert_queue(struct chunk_queue_t *q, struct chunk_t *prev, struct chunk_t *c) {
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

// struct chunk_t *
// get_chunk_by_seq(struct chunk_queue_t *q, int seq) {
// 	struct chunk_t *c = q->head;
// 	while (c != NULL) {
// 		if (c->seq == seq) {
// 			return c;
// 		}
// 		c = c->next;
// 	} 
// }

bool 
insert_chunk(struct handle_t *h, struct chunk_t *c) {
	if (c->seq <= h->recv_min_seq) {
		return false;
	}
	if (c->seq > h->recv_max_seq) {
		insert_queue(&h->recv_queue, NULL, c);
		return true;
	}
	struct chunk_t *tmp = h->recv_queue.tail;
	while(tmp) {
		if (tmp->seq == c->seq) {
			return false;
		}
		if (tmp->seq < c->seq) {
			insert_queue(&h->recv_queue, tmp->seq, c);
			return true;
		}
		tmp = tmp->prev;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void 
revc_queue_clear(struct handle_t *h, int ack) {
	while (1) {
		struct chunk_t *c = h->recv_queue.head;
		if (c == NULL) {
			return;
		}
		if (c->seq <= ack) {
			c = pop_chunk_queue(&h->recv_queue);
			chunk_pool_free(h->pool, c);
		}
	}
}

void * 
revc_buff(struct handle_t *h) {
	if (h->recv_cur != NULL) {
		return h->recv_cur;
	}
	h->recv_cur = chunk_pool_malloc(h->pool);
	return h->recv_cur;
}

void
revc_buff_process(struct handle_t *h, int sz) {
	struct chunk_t *c = h->recv_cur;
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

	if (h->recv_max_seq < c->seq) {
		h->recv_max_seq = c->seq;
	}
	if (h->recv_min_seq + 1 == c->seq) {
		h->recv_min_seq = c->seq;
	}

	switch (c->type) {
		case UDP_TYPE_REQ_SEQ:
			
			break;
		case UDP_TYPE_SYN_ACK:
			break;
		case UDP_TYPE_DATA:
		case UDP_TYPE_DATA_AGAIN:
			if (h->recv_max_ack < c->ack) {
				h->recv_max_ack = c->ack;
				send_history_clear(h);
			} 
			if (insert_chunk(h, c)) {
				h->recv_cur = NULL;
			}
			break;
		default :
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
send_history_clear(struct handle_t *h) {
	struct chunk_t *c = h->send_history.head;
	while (c != NULL && c->seq <= h->recv_max_ack) {
		c = pop_queue(h->send_history);
		chunk_pool_free(h->pool, c);
	}
}

void*
send_buff_in(struct handle_t *h) {
	if (h->send_cur_in != NULL) {
		return h->send_cur_in->buff;
	}
	h->send_cur_in = chunk_pool_malloc(h->pool);
	return h->send_cur_in->buff;
}

void
send_buff_in_process(struct handle_t *h, int size) {
	chunk_t *c = h->send_cur_in;
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
	push_queue_back(h->send_queue, c);
	h->send_cur_in = NULL;
}

struct chunk_t *
send_buff_out(struct handle_t *h) {
	if (h->send_cur_out != NULL) {
		return h->send_cur_out;
	}
	h->send_cur_out = pop_queue(h->send_queue);
	return h->send_cur_out;
}

void
send_buff_out_process(struct handle_t *h) {
	if (h->send_cur_out == NULL) {
		return;
	}
	if (h->send_cur_out->type == UDP_TYPE_DATA_AGAIN) {
		chunk_pool_free(h->pool, h->send_cur_out);
	} else {
		push_queue_back(h->send_history, h->send_cur_out);		
	}
	h->send_cur_out = NULL;
}
