#ifndef TIMER_HANDLE_H
#define TIMER_HANDLE_H

#include "timer.h"

#define TIMER_CALLBACK(func, caller) \
	std::tr1::bind(&func, \
	caller, \
	std::tr1::placeholders::_1, \
	std::tr1::placeholders::_2)

class timer_handle_t {
public:
	typedef std::hash_map<int, timer_t *> timers_t;

	typedef std::hash_map<std::string, timer_t *> name_timers_t;

public:
	timer_handle_t(timer_axis_t *timer_axis);

	virtual ~timer_handle_t();

	virtual void register_timer(const timer_t::cb_t& cb, uint32 interval, uint32 times = 1, const char *name = NULL, void *data = NULL);

	virtual void unregister_timer(const char *name);

	void on_register(timer_t *timer);

	void on_unregister(timer_t *timer);

protected:
	timers_t times_;

	name_timers_t name_timers_;

	timer_axis_t *timer_axis_;
}

#endif