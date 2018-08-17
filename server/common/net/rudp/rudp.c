struct chunk_queue_t {
	struct chunk_t *head;
	struct chunk_t *tail;
};

struct handle_t {
	chunk_queue_t recv_queue;								// 新消息放后面
	chunk_t *recv_cur; 
	int recv_max_seq;
	int recv_min_seq;
	int recv_max_ack;

	chunk_queue_t send_queue;								// 新消息放后面
	chunk_queue_t send_history;								// 新消息放后面
	chunk_t *send_cur;
	int max_ack;
	chunk_pool_t *pool;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void 
push_queue(struct chunk_queue_t *q, struct chunk_t *c) {
	if (q->tail == NULL) {
		q->head = c;
		q->tail = c;
	} else {
		c->prev = q->tail;
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

bool 
insert_chunk(handle_t *h, struct chunk_t *c) {
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
revc_queue_clear(handle_t *h, int ack) {
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

void* 
revc_buff(handle_t *h) {
	if (h->recv_cur != NULL) {
		return h->recv_cur->buff;
	}
	h->recv_cur = chunk_pool_malloc(h->pool);
	return h->recv_cur->buff;
}

void
revc_buff_process(handle_t *h, int sz) {
	struct chunk_t *c = h->recv_cur;
	if (c == NULL) {
		return;
	}
	if (sz < UDP_HEAD_BYTE_ALL 
		|| sz > UDP_DATA_MAX_LEN + UDP_HEAD_BYTE_ALL) {
		// ERROR
		return;
	}
	memcpy(&c->size, c->buff, UDP_HEAD_BYTE_LEN);
	memcpy(&c->type, c->buff + UDP_HEAD_BYTE_LEN, UDP_HEAD_BYTE_TYPE);
	memcpy(&c->seq, c->buff + UDP_HEAD_BYTE_LEN + UDP_HEAD_BYTE_TYPE, UDP_HEAD_BYTE_SEQ);
	memcpy(&c->ack, c->buff + UDP_HEAD_BYTE_LEN + UDP_HEAD_BYTE_TYPE + UDP_HEAD_BYTE_SEQ, UDP_HEAD_BYTE_ACK);
	if (c->size != sz) {
		// ERROR
		return;
	}
	if (insert_chunk(h, c)) {
		if (h->recv_max_seq < c->seq) {
			h->recv_max_seq = c->seq;
		}
		if (h->recv_min_seq + 1 == c->seq) {
			h->recv_min_seq = c->seq;
		}
		if (h->recv_max_ack < c->ack) {
			h->recv_max_ack = c->ack;
			send_queue_clear(h);
		} 
		h->recv_cur = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
send_queue_clear(handle_t *h) {
	struct chunk_t *c = h->send_history.head;
	while (c != NULL && c->seq < h->recv_max_ack) {
		c = pop_queue(h->send_history);
		chunk_pool_free(h->pool, c);
	}
}

void*
send_buff(handle_t *h) {
	struct chunk_t *c = chunk_pool_malloc(h->pool);

}