#ifndef UDP_RUDP_H
#define UDP_RUDP_H

#define UDP_DATA_MAX_LEN 512		// Internet(非局域网)上的标准MTU值为576字节 (控制在548字节 (576-8-20)以内)

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

void* revc_buff_in(struct udp_handle_t *h);

void revc_buff_in_process(struct udp_handle_t *h, int sz);

struct udp_chunk_t* recv_cur_out(struct udp_handle_t *h);

void recv_cur_out_process(struct udp_handle_t *h);

void* send_buff_in(struct udp_handle_t *h);

void send_buff_in_process(struct udp_handle_t *h, int size);

struct udp_chunk_t * send_buff_out(struct udp_handle_t *h)

void send_buff_out_process(struct udp_handle_t *h);

void udp_process(struct udp_handle_t *h, int64 tick);

#endif