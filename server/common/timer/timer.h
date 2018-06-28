#ifndef TIMER_H
#define TIMER_H

#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include "type.h"
#include "timer_axis.h"

class timer_mt {
public:
	typedef std::tr1::function<void (void *, uint32)> cb_t;

public:
	timer_mt(const timer_mt::cb_t& cb, uint32 interval, uint32 times = ~0u, const char *name = NULL, void *data = NULL);

	~timer_mt();

	void on_register();

	void on_unregister();

	cb_t cb_;

	void *obj_;

	void *data_;

	char *name_;

	uint32 times_;

	uint32 interval_;

	uint32 slot_;

	timer_axis_t::slot_t::iterator pos_;

	timestamp last_time_;
};

#endif