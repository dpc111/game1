#include "timer.h"

inline time_base_t::time_base_t(timers_base_t &owner, timer_handler_t *handler, void *user_data) 
	: owner_(owner)
	, handler_(handler)
	, user_data_(user_data)
	, state_(TIME_PENDING) {
	handler_->inc_register_num();
}

inline void time_base_t::cancel() {
	if (this->is_cancelled()) {
		return;
	}
	assert((state_ == TIME_PENDING) || (state_ == TIME_EXECUTING));
	state_ = TIME_CANCELLED;
	if (handler_) {
		handler_->release(timer_handle_t(this), user_data_);
		handler_ = NULL;
	}
	owner_.on_cancel(); 
}

timers_t::timers_t()
	: time_queue_()
	, process_node_(NULL)
	, last_process_time_(0)
	, cancelled_num_(0) {
}

timers_t::~timers_t() {
	this->clear();
}

int timers_t::process(timestamp now) {
	int fired_num = 0;
	while ((!time_queue_.empty()) 
		&& (time_queue_.top()->time() <= now
		|| time_queue_.top()->is_cancelled())) {
		timer_t *time = process_node_ = time_queue_.top();
		time_queue_.pop();
		if (!time->is_cancelled()) {
			++fired_num;
			time->trigger();
		}
		if (!time->is_cancelled()) {
			time_queue_.push(time);
		} else {
			delete time;
			assert(cancelled_num_ > 0);
			--cancelled_num_;
		}
	}
	process_node_ = NULL;
	last_process_time_ = now;
	return fired_num;
}

bool timers_t::legal(timer_handle_t handle) const {
	typedef timer_t * const * time_iter;
	timer_t *time = static_cast<timer_t *>(handle.time());
	if (time == NULL) {
		return false;
	}
	if (time == process_node_) {
		return true;
	}
	time_iter begin = &time_queue_.top();
	time_iter end = begin + time_queue_.size();
	for (time_iter it = begin; it != end; ++it) {
		if (*it == time) {
			return true;
		}
	}
	return false;
}

timestamp timers_t::next_exp(timestamp now) const {
	if (time_queue_.empty() || now > time_queue_.top()->time()) {
		return 0;
	}
	return time_queue_.top()->time() - now;
}

void timers_t::clear(bool should_call_cancel = true) {
	int max_loop_num = time_queue_.size();
	while (!time_queue_.empty()) {
		timer_t *time = time_queue_.unsafe_pop_back();
		if (!time->is_cancelled() && should_call_cancel) {
			--cancelled_num_;
			time->cancel();
			if (--max_loop_num == 0) {
				should_call_cancel = false;
			}
		} else if (time->is_cancelled()) {
			--cancelled_num_;
		}
		delete time;
	}
	cancelled_num_ = 0;
	time_queue_ = priority_queue_t();
}

bool timers_t::get_timer_info(timer_handle_t handle, timestamp& time, timestamp& interval, void *& user) const {
	timer_t *time = static_cast<timer_t *>(handle.time());
	if (!time->is_cancelled()) {
		time = time->time();
		interval = time->interval();
		user = time->get_user_data();
		return true;
	}
	return false;
}

timer_handle_t timers_t::add(timestamp start, timestamp interval, timer_handler_t *handler, void *user) {
	timer_t *time = new timer_t(*this, start_time, interval, handler, user_data);
	time_queue_.push(time);
	return timer_handle_t(time);
}

template <class T>
class is_not_cancelled_t {
public:
	bool operator()(const T *time) {
		return !time->is_cancelled();
	}
}

void timers_t::purge_cancelled_times() {
	priority_queue_t::container_t &container = time_queue_.container();
	priority_queue_t::container_t::iterator new_end = 
		std::partition(container.begin(), container.end(), is_not_cancelled_t<timer_t>());
	for (priority_queue_t::container_t::iterator it = new_end; it != container.end(); ++it) {
		delete *it;
	} 
	const int purged_num = container.end() - new_end;
	cancelled_num_ -= purged_num;
	assert((cancelled_num_ == 0) || (cancelled_num_ == 1));
	container.erase(new_end, container.end());
	time_queue_.make_heap();
}

void timers_t::on_cancel() {
	++cancelled_num_;
	if ((cancelled_num_ * 2) > int(time_queue_.size())) {
		this->purge_cancelled_times();
	}
}
