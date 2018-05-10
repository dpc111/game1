#include "cfg_json_mgr.h"
#include "cfg_json.h"

cfg_json_mgr_t::cfg_json_mgr_t() {
	reader_ = new JsonReader();	
}

cfg_json_mgr_t::~cfg_json_mgr_t() {
	delete reader_;
}

void cfg_json_mgr_t::load(std::string file_name) {
	cfg_json_t *json = new cfg_json_t(this);
	if (json->init(file_name.c_str());) {
		jsons_[file_name] = json;
	} else {
		delete json;
	}
}

cfg_json_t* cfg_json_mgr_t::get_json(std::string file_name) {
	cfg_json_map_t::iterator it = jsons_.find(file_name);
	if (it == jsons_.end()) {
		return NULL;
	}
	return it->second;
}

int cfg_json_mgr_t::get_int(std::string file_name, const char *key1) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1);
}

int cfg_json_mgr_t::get_int(std::string file_name, int *key1) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1);
}

int cfg_json_mgr_t::get_int(std::string file_name, const char *key1, const char *key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string file_name, const char *key1, int key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string file_name, int *key1, const char* key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string file_name, int *key1, int key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

std::string cfg_json_mgr_t::get_string(std::string file_name, const char *key1) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1);
}

std::string cfg_json_mgr_t::get_string(std::string file_name, int *key1) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1);
}

std::string cfg_json_mgr_t::get_string(std::string file_name, const char *key1, const char *key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}

std::string cfg_json_mgr_t::get_string(std::string file_name, const char *key1, int key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}

std::string cfg_json_mgr_t::get_string(std::string file_name, int *key1, const char* key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1);
}

std::string cfg_json_mgr_t::get_string(std::string file_name, int *key1, int key2) {
	cfg_json_t *json = this->get_json(file_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}