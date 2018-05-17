#include "entity_mgr.h"
#include "entity.h"
#include "global.h"

entity_mgr_t::entity_mgr_t(room_t *room) :
	object_mgr_base_t(),
	room_(room) {

}

entity_mgr_t::~entity_mgr_t() {

}

entity_t *entity_mgr_t::create_entity(int32 type_id, int32 row, int32 col) {
	int32 entity_id = gen_id();
	if (get_object(entity_id)) {
		return NULL;
	}
	if (!get_json_mgr()->exist("entity", type_id - 1)) {
		return NULL;
	}
	entity_t *entity = new entity_t(entity_id, type_id);
	entity->set_level(1);
	entity->set_cd(get_json_mgr()->get_float("entity", type_id - 1, "cd"));
	entity->set_blood(get_json_mgr()->get_int("entity", type_id - 1, "blood"));
	entity->set_damage(get_json_mgr()->get_float("entity", type_id - 1, "damage"));
	entity->set_grid(row, col);
	add_object(entity_id, entity);
	return entity;
}

void entity_mgr_t::delete_entity(int32 entity_id) {
	entity_t *entity = (entity_t *)get_object(entity_id);
	if (!entity) {
		return;
	}
	delete entity;
	del_object(entity_id);
}