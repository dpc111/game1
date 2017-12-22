#ifndef TCP_NETWORK_H
#define TCP_NETWORK_H

class tcp_network_t {
public:
	typedef std::map<int, tcp_connection_t *> conn_map_t;

public:
	tcp_network_t(const char* ip, int port);

	~tcp_network_t();

private:
	conn_map_t 		 		conns_;
}

#endif