#include "entity_mgr.h"
#include "entity.h"
#include "id_generate.h"

entity_mgr_t::entity_mgr_t() {
	entitys.clear();
	id_generate_ = id_generate_t();
}

entity_mgr_t::~entity_mgr_t() {
	
}

void entity_mgr_t::create_entity(int32 type_id, int32 grid_id) {

}
