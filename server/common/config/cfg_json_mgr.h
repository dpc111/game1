#ifndef CFG_JSON_MGR_H
#ifndef CFG_JSON_MGR_H

#include "json/json.h"

class cfg_json_mgr_t {
public:
	typedef std::map<std::string, cfg_json_t *> cfg_json_map_t;

public:
	cfg_json_mgr_t();

	~cfg_json_mgr_t();

	void load(std::string file_name);

	JsonReader* get_json_reader() { return reader_; }

private:
	JsonReader *reader_;

	cfg_json_map_t jsons_;
};

#endif