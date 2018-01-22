#include "net_thread.h"
#include "tcp_network.h"
#include "tcp_connection.h"
#include "net_output_stream.h"
#include "net_input_stream.h"

net_thread_t::net_thread_t(tcp_network_t *network) {
	network_ = network;
	poller_ = new poller_t();
	conn_num_ = 0;
}

net_thread_t::~net_thread_t() {
	delete poller_;
}

int net_thread_t::run(void *arg) {
	net_thread_t* self = (net_thread_t *)arg;
	while (self->get_owner()->is_quit()) {
		self->get_poller()->process_once();
		self->thread_process();
		sleep(1);
	}
}

void net_thread_t::thread_process() {
	while (output_queue_.size() > 0) {
		output_chunk_t *chunk = output_queue_.pop();
		if (!chunk->conn_) {
			thread_cmd_t *cmd = (thread_cmd_t *)chunk->buff_;
			switch (cmd->op_) {
				case CMD_CONN_ADD: {
					on_conn_add(((cmd_add_t *)cmd)->conn_);
				}
				case CMD_CONN_REMOVE: {
					on_conn_remove(((cmd_remove_t *)cmd)->conn_);
				}
				default : {

				}
			}
			output_pool_free(chunk);
		} 
		else if (!chunk->conn_->connected()) {
			output_pool_free(chunk);
		}
		else {
			chunk->conn_->get_output_stream().append(chunk);
			// add event write ???
		}
	}
}

void net_thread_t::process(timestamp tm) {
	while (input_queue_.size() > 0) {
		input_chunk_t *chunk = input_queue_.pop();
		if (!chunk->conn_) {
			thread_cmd_t *cmd = (thread_cmd_t *)chunk->buff_;		
			switch (cmd->op_) {
				case CMD_CONN_ADD: {
					get_owner()->on_conn_add(((cmd_add_t *)cmd)->conn_);
				}
				case CMD_CONN_REMOVE: {
					get_owner()->on_conn_remove(((cmd_remove_t *)cmd)->fd_);
				}
				default : {

				}
			}
		}
		input_pool_free(chunk);
	}
}

input_chunk_t* net_thread_t::input_pool_alloc() {
	return input_pool_.alloc();
}

void net_thread_t::input_pool_free(input_chunk_t *chunk) {
	chunk->~chunk_t();
	input_pool_.free(chunk);
}

output_chunk_t* net_thread_t::output_pool_alloc() {
	return output_pool_.alloc();
}

void net_thread_t::output_pool_free(output_chunk_t *chunk) {
	chunk->~chunk_t();
	output_pool_.free(chunk);
}

void net_thread_t::read(int fd, void* ud) {
	tcp_connection_t *conn = (tcp_connection_t *)ud;
	net_thread_t *thread = conn->get_thread();
	if (!conn->connected()) {
		thread->on_conn_remove(conn);
		return;
	}
	int n = conn->get_input_stream().read_fd(ud, fd);
	if (n < 0) {
		thread->on_conn_remove(conn);
		return;
	}
	if (!thread->get_owner()->on_message_recv(conn)) {
		conn->get_input_stream().reset();
	}
}

void net_thread_t::write(int fd, void* ud) {
	tcp_connection_t *conn = (tcp_connection_t *) ud;
	net_thread_t *thread = conn->get_thread();
	if (!conn->connected()) {
		thread->on_conn_remove(conn);
		return;
	}
	if (conn->get_output_stream().size() <= 0) {
		// del event write ???
		return;
	}
	int n = conn->get_output_stream().write_fd(ud, fd);
	if (n < 0) {
		thread->on_conn_remove(conn);
		return;
	}
}

input_queue_t& net_thread_t::assign_read_fd_buff() {
	while (read_fd_buff_.size() < NUM_READ_IOVEC) {
		input_chunk_t *chunk = input_pool_alloc();
		read_fd_buff_.push_back(chunk);
	}
	return read_fd_buff_;
}

void net_thread_t::on_conn_add(tcp_connection_t *conn) {
	poller_->register_read(conn->get_fd(), conn, net_thread_t::read);
	poller_->register_write(conn->get_fd(), conn, net_thread_t::write);
	conn_num_++;
	thread_post_cmd<cmd_add_t>(conn);
}

void net_thread_t::on_conn_remove(tcp_connection_t *conn) {
	if (conn->is_closed()) {
		return;
	}
	conn->set_closed(false);
	conn->get_input_stream().reset();
	conn->get_output_stream().reset();
	poller_->deregister_read(conn->get_fd());
	poller_->deregister_write(conn->get_fd());
	conn_num_--;
	thread_post_cmd<cmd_remove_t>(conn->get_fd());
}
