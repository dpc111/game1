#ifndef ENTITY_MGR_H
#define ENTITY_MGR_H

#include <map>

class entity_t;

class entity_mgr_t {
public:
	typedef std::map<int, entity_t *> entity_map_t;

public:
	entity_mgr_t();

	~entity_mgr_t();

	void create_entity(int32 type_id, int32 grid_id);

private:
	entity_map_t entitys;

	id_generate_t id_generate_;
};

#endif