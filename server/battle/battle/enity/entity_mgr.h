#ifndef ENTITY_MGR_H
#define ENTITY_MGR_H

#include "stdafx.h"
#include "object_mgr_base.h"

class entity_t;

class entity_mgr_t : object_mgr_base_t {
public:
	entity_mgr_t();

	virtual ~entity_mgr_t();

	entity_t *create_entity(int32 type_id, int32 row, int32 col);

	void *delete_entity(int32 entity_id);

private:
	
};

#endif