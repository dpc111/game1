#ifndef POLLER_H
#define POLLER_H

#define POLLER_MAX_SIZE 10240
#define POLLER_EVENTS_ONCE 1024


typedef int (read_func) (int);

typedef int (write_func) (int);

struct read_handle_t {
	void *ud_;
	read_func *func_;
};

struct write_handle_t {
	void *ud_;
	write_func *func_;
};

typedef std::map<int, read_handle_t> read_handles_t;

typedef std::map<int, write_handle_t> write_handles_t;

class poller_t
{
public:
	poller_t();

	~poller_t();

	void register_read(int fd, void *ud, read_func *func);

	void register_write(int fd, write_handle *handle);

	void deregister_read(int fd);

	void deregister_write(int fd);

	void deregister_all(int fd);

	int process_once(int wait_time = 0);

private:
	bool is_register_read(int fd);

	bool is_register_write(int fd);

	bool trigger_read(int fd);

	bool trigger_write(int fd);

	bool trigger_error(int fd);

private:
	read_handles_t read_handles_;

	write_handles_t write_handles_;

	int epoll_fd_;

	struct epoll_event events_[POLLER_EVENTS_ONCE];
};

#endif
