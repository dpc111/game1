#ifndef BULLET_MGR_H
#define BULLET_MGR_H

#include "stdafx.h"
#include "object_mgr_base.h"

class bullet_mgr_t : object_mgr_base_t {
public:
	bullet_mgr_t();

	virtual ~bullet_map_t();

	bullet_t *create_bullet(int32 type_id);

	void delete_bullet(int32 bullet_id);

private:

};

#endif