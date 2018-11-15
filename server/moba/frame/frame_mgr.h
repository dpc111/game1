#ifndef FRAME_MGR_H
#define FRAME_MGR_H

#include "stdafx.h"
#include "type.h"
#include "chunk.h"
#include "cache.h"

#define FRAME_INTERVAL_MS 50

class room_t;

class frame_mgr_t {
public:
	typedef chunk_t<512> frame_chunk_t;

public:
	frame_mgr_t(room_t *room);

	~frame_mgr_t();

	void recv(int uid, void *buff, int size);

	void refresh();

	void sync();

private:
	room_t *room_;

	frame_chunk_t chunk_;

	int frame_;

	cache_t cache_;
};

#endif