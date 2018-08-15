#define UDP_DATA_MAX_LEN 512		// Internet(非局域网)上的标准MTU值为576字节 (控制在548字节 (576-8-20)以内)

#define UDP_DATA_LEN_LEN 1
#define UDP_DATA_TYPE_LEN 1
#define UDP_DATA_SEQ_LEN 4
#define UDP_DATA_ACK_LEN 4

struct udp_head {
	short 
}

