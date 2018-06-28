#ifndef TIMER_HANDLE_H
#define TIMER_HANDLE_H

#include "timer.h"
#include <hash_map>

// #define TIMER_CALLBACK(func, caller) \
// 	std::tr1::bind(&func, \
// 	caller, \
// 	std::tr1::placeholders::_1, \
// 	std::tr1::placeholders::_2) 

// #define REGISTER_TIMER(func, delay) register_timer(TIMER_CALLBACK(func, this), delay, 1) 

// #define REGISTER_TIMER_REPEAT(func, interval) register_timer(TIMER_CALLBACK(func, this), interval, ~0u) 

class timer_handle_t {
public:
	typedef void (callback_t) (void *, uint32);

	typedef std::hash_map<int, timer_t *> timers_t;

	typedef std::hash_map<std::string, timer_t *> name_timers_t;

public:
	timer_handle_t(timer_axis_t *timer_axis);

	virtual ~timer_handle_t();

	virtual void register_timer_ms(const timer_t::cb_t& cb, uint32 interval, uint32 times = 1, const char *name = NULL, void *data = NULL);

	virtual void register_timer_sec(const callback_t& cb, float interval, uint32 times = 1, const char *name = NULL, void *data = NULL);

	virtual void register_timer_repeat(const callback_t& cb, float interval, const char *name = NULL, void *data = NULL);

	virtual void register_timer_delay(const callback_t& cb, float interval, const char *name = NULL, void *data = NULL);

	virtual void unregister_timer(const char *name);

	void on_register(timer_t *timer);

	void on_unregister(timer_t *timer);

protected:
	timers_t times_;

	name_timers_t name_timers_;

	timer_axis_t *timer_axis_;
}

#endif