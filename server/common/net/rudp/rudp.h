#define UDP_DATA_MAX_LEN 512		// Internet(非局域网)上的标准MTU值为576字节 (控制在548字节 (576-8-20)以内)

#define UDP_HEAD_BYTE_LEN 1
#define UDP_HEAD_BYTE_TYPE 1
#define UDP_HEAD_BYTE_SEQ 4
#define UDP_HEAD_BYTE_ACK 4
#define UDP_HEAD_BYTE_ALL 10

#define UDP_TYPE_REQ_SEQ 1
#define UDP_TYPE_SYN_ACK 2
#define UDP_TYPE_DATA 3
