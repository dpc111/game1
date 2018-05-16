#include "entity.h"

entity_t::entity_t(int32 entity_id, int32 type_id) :
	object_base_t(entity_id, type_id) {
	level_ = 1;
	cd_ = 1;
	blood_ = 0;
	damage_ = 0;
	frozen_ = 0;
	grid_ = NULL;
}

entity_t::~entity_t() {

}