#include "cfg_json_mgr.h"

cfg_json_mgr_t::cfg_json_mgr_t() {
	reader_ = new JsonReader();	
}

cfg_json_mgr_t::~cfg_json_mgr_t() {
	delete reader_;
}

void cfg_json_mgr_t::load(std::string file_name) {
	
}
