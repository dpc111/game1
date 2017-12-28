#include "tcp_network.h"
#include "net_thread.h"
#include "tcp_connection.h"

#define NET_THREAD_NUM 4

tcp_network_t::tcp_network_t(const char *ip, int port) {
	is_quit_ = false;
	for (int i = 1; i <= NET_THREAD_NUM; i++) {
		net_thread_t *thread = new net_thread_t(this);
		threads_[i] = thread;
	}
	addr_ = net_address_t(ip, port);
	poller_ = new poller_t();
}

tcp_network_t::~tcp_network_t() {
	thread_map_t::iterator it = threads_.begin();
	while (it != threads_.end()) {
		net_thread_t *thread = it->second;
		delete thread;
		threads_.erase(it++);
	}
	delete poller_;
}

net_thread_t* tcp_network_t::get_idle_thread() {
	net_thread_t *thread = NULL;
	for (thread_map_t::iterator it = threads_.begin(); it != threads_.end(); it++) {
		net_thread_t *cur = it->second;
		if (!thread) {
			thread = cur;
		}
		if (cur->get_conn_num() < thread->get_conn_num()) {
			thread = cur;
		}
	}
	return thread;
}

void tcp_network_t::on_conn_add(tcp_connection_t *conn) {
	if (conn_add_cb_) {
		conn_add_cb_(conn);
	}
}

void tcp_network_t::on_conn_remove(int fd) {
	if (conn_remove_cb_) {
		conn_remove_cb_(fd);
	}
}

bool tcp_network_t::on_message_recv(tcp_connection_t *conn) {
	if (message_recv_cb_) {
		message_recv_cb_(conn);	
		return true;
	}
	return false;
}

void tcp_network_t::on_socket_connect(int fd, void *ud) {
	struct sockaddr_in peer_addr;
	socklen_t sz;
	int conn_fd = accept(fd, (struct sockaddr *)&peer_addr, &sz);
	if (conn_fd < 0) {
		return;
	}
	tcp_network_t *network = (tcp_network_t *)ud;
	net_thread_t *thread = network->get_idle_thread();
	tcp_connection_t *conn = new tcp_connection_t(conn_fd, peer_addr);
	conn->set_network(network);
	conn->set_thread(thread);
	network->get_conn_map()[conn_fd] = conn;	
	thread->post_cmd<cmd_add_t>(conn);
}

void tcp_network_t::start() {
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		return;
	}
	if (bind(fd, (sockaddr *)&(addr_.addr_), sizeof(addr_)) < 0) {
		return;
	} 
	poller_->register_listen(fd, this, tcp_network_t::on_socket_connect);
}

void tcp_network_t::process() {
	for (thread_map_t::iterator it = threads_.begin(); it != threads_.end(); it++) {
		net_thread_t *thread = it->second;
		thread->process();
	}
}