#ifndef TIMER_H
#define TIMER_H

class timer_t;
class timers_t;
typedef timestamp int;

//////////////////////////////////////////////////////////////////////////////////////////////////
class timer_handle_t {
public:
	timer_handle_t(timer_t *timer = NULL) : timer_(timer) {}

	void cancel();

	void clear_without_cancel() { timer_ = NULL; }

	bool is_set() const { return timer_ != NULL; }

	friend bool operator==(timer_handle_t h1, timer_handle_t h2);

	timer_t *timer() const { return timer_; }

private:
	timer_t *timer_;
};

inline bool operator==(timer_handle_t h1, timer_handle_t h2) {
	return h1.timer_ == h2.timer_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
class timer_handler_t {
public:
	timer_handler_t() : register_num_(0) {}

	virtual void ~timer_handler_t() { assert(reg_num_ == 0); }

	virtual void handle_timeout(timer_handle_t handle, void *user) = 0;

protected:
	virtual void on_release(timer_handle_t handle, void *user) {}

private:
	friend class timer_t;

	void inc_register_num() { ++register_num_; }

	void dec_register_num() { --register_num_; }

	void release(timer_handle_t handle, void *user) {
		this->dec_register_num();
		this->on_release(handle, user);
	}

private:
	int register_num_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class timer_t {
public:
	enum time_state_t {
		TIME_PENDING,
		TIME_EXECUTING,
		TIME_CANCELLED
	};

public:
	timer_t(timers_t &owner, timer_handler_t *handler, void *user_data, timestamp time, timestamp interval);
	
	timer_t(const timer_t &);

	~timer_t() {}

	timer_t &operator=(const timer_t &);

	void cancel();

	void trigger();

	timers_t& owner() const { return owner_; }

	timer_handler_t *handler() const { return handler_; }

	void *user_data() const { return user_data_; }

	timestamp time() const { return time_; }

	timestamp interval() const { return interval_; }

	bool is_cancelled() const { return state_ == TIME_CANCELLED; }

	bool is_executing() const { return state_ == TIME_EXECUTING; }

private:
	timers_t& owner_;

	timer_handler_t *handler_;

	void *user_data_;

	timestamp time_;

	timestamp interval_;

	time_state_t state_;
};


//////////////////////////////////////////////////////////////////////////////////////////////////
class comparator_t {
public:
	bool operator()(const timer_t *a, const timer_t *b) {
		return a->time() > b->time();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class priority_queue_t {
public:
	typedef std::vector<timer_t *> container_t;

public:
	bool empty() const { return container_.empty(); }

	int size() const { return container_.size(); }

	const timer_t& top() const { return container_.front(); }

	void push(const value_type& x) {
		container_.push_back(x);
		std::push_heap(container_.begin(), container_.end(), comparator_t());
	}

	void pop() {
		std::pop_heap(container_.begin(), container_.end(), comparator_t());
		container_.pop_back();
	}

	timer_t *unsafe_pop_back() {
		timer_t *time = container_.back();
		container_.pop_back();
		return time;
	}

	container_t &container() { return container_; }

	void make_heap() { std::make_heap(container_.begin(), container_.end(), comparator_t()); }

private:
	container_t container_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
class timers_t {
public:
	timers_t();

	virtual ~timers_t();

	inline int size() const { return time_queue_.size(); }

	inline bool empty() const { return time_queue_.empty(); }

	int process(timestamp now);

	bool legal(timer_handle_t handle) const;

	timestamp next_exp(timestamp now) const;

	void clear(bool should_call_cancel = true);

	bool get_timer_info(timer_handle_t handle, timestamp& time, timestamp& interval, void *& user) const;

	timer_handle_t add(timer_handler_t *handler, void *user, timestamp start, timestamp interval);

	void purge_cancelled_times();

	void on_cancel();

private:
	priority_queue_t time_queue_;

	timer_t *process_node_;

	timestamp last_process_time_;

	int cancelled_num_;
};

#endif