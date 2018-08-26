#ifndef UDP_MSG_H
#define UDP_MSG_H

#include "stdafx.h"

class udp_msg_t {
public:
	udp_msg_t();

	~udp_msg_t();

	void on_message(int32 sid, void *buff, void size);
};

#endif