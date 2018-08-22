#ifndef UDP_RUDP_H
#define UDP_RUDP_H

#define UDP_HEAD_BYTE_LEN 1
#define UDP_HEAD_BYTE_TYPE 1
#define UDP_HEAD_BYTE_SEQ 4
#define UDP_HEAD_BYTE_ACK 4
#define UDP_HEAD_BYTE_ALL 10

#define UDP_TYPE_REQ_SEQ 1
#define UDP_TYPE_SYN_ACK 2
#define UDP_TYPE_DATA 3
#define UDP_TYPE_DATA_AGAIN 4

#define UDP_REQ_AGAIN_TICKS 10
#define UDP_REQ_AGAIN_MAX_TIMES 10
#define UDP_RECV_SEQ_MAX_DEV 20

#include "udp_pool.h"
#include "udp_chunk.h"

typedef struct udp_chunk_queue {
	udp_chunk_t *head;
	udp_chunk_t *tail;
} udp_chunk_queue_t;

typedef struct udp_handle {
	udp_chunk_queue_t recv_queue;								// 接收队列
	udp_chunk_t *recv_cur_in; 									// 网络写
	udp_chunk_t *recv_cur_out;				 					// 逻辑读
	int recv_max_seq;
	int recv_min_seq;
	int recv_max_ack;

	udp_chunk_queue_t send_queue;								// 发送队列
	udp_chunk_queue_t send_history;								// 发送历史
	udp_chunk_t *send_cur_in;									// 逻辑写
	udp_chunk_t *send_cur_out;									// 网络读
	int send_seq;
	
	int64 req_tick;
	int req_times;

	udp_chunk_pool_t *pool;
} udp_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

void* revc_buff_in(udp_handle_t *h);

void revc_buff_in_process(udp_handle_t *h, int sz);

udp_chunk_t* recv_cur_out(udp_handle_t *h);

void recv_cur_out_process(udp_handle_t *h);

void* send_buff_in(udp_handle_t *h);

void send_buff_in_process(udp_handle_t *h, int size);

udp_chunk_t * send_buff_out(udp_handle_t *h);

void send_buff_out_process(udp_handle_t *h);

int udp_process(udp_handle_t *h, int64 tick);

#ifdef __cplusplus
}
#endif

#endif