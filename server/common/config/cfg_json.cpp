#include <fstream> 
#include "cfg_json.h"
#include "log.h"

cfg_json_t::cfg_json_t(cfg_json_mgr_t *mgr) {
	mgr_ = mgr;
	file_name_ = NULL;
}

cfg_json_t::~cfg_json_t() {
	mgr_ = NULL;
	file_name_ = NULL;
}

bool cfg_json_t::init(const char *file_name) {
	file_name_ = file_name;
	// std::ifstream ifs;
	// ifs.open(file_name, std::ios::binary);
	std::ifstream ifs(file_name);
	if (!ifs.is_open()) {
		ERROR("open failed %d", file_name);
		return false;
	}
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse("{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}", value)) {
		ERROR("parse failed %s", file_name);
	}
	if (!mgr_->get_json_reader()->parse(ifs, json_value_))
	{
		ERROR("parse failed %s", file_name);
		ifs.close();
		return false;
	}
	ifs.close();
	return true;
}

int cfg_json_t::get_int(const char *key1) {
	return json_value_[key1].asInt();
}

int cfg_json_t::get_int(int key1) {
	return json_value_[key1].asInt();
}

int cfg_json_t::get_int(const char *key1, const char *key2) {
	return json_value_[key1][key2].asInt();
}

int cfg_json_t::get_int(const char *key1, int key2) {
	return json_value_[key1][key2].asInt();
}

int cfg_json_t::get_int(int key1, const char* key2) {
	return json_value_[key1][key2].asInt();
}

int cfg_json_t::get_int(int key1, int key2) {
	return json_value_[key1][key2].asInt();
}

std::string cfg_json_t::get_string(const char *key1) {
	return json_value_[key1].asString();
}

std::string cfg_json_t::get_string(int key1) {
	return json_value_[key1].asString();
}

std::string cfg_json_t::get_string(const char *key1, const char *key2) {
	return json_value_[key1][key2].asString();
}

std::string cfg_json_t::get_string(const char *key1, int key2) {
	return json_value_[key1][key2].asString();
}

std::string cfg_json_t::get_string(int key1, const char* key2) {
	return json_value_[key1][key2].asString();
}

std::string cfg_json_t::get_string(int key1, int key2) {
	return json_value_[key1][key2].asString();
}