#ifndef CFG_JSON_MGR_H
#define CFG_JSON_MGR_H

#define JSON_ERROR_INT 0xFFFFFFFF
#define JSON_ERROR_STRING "json_error_string"

#include "json/json.h"

class cfg_json_t;

class cfg_json_mgr_t {
public:
	typedef std::map<std::string, cfg_json_t *> cfg_json_map_t;

public:
	cfg_json_mgr_t();

	~cfg_json_mgr_t();

	void load(std::string file_name);

	JsonReader* get_json_reader() { return reader_; }

	cfg_json_t* get_json(std::string file_name);

	int32 get_int(std::string file_name, const char *key1);

	int32 get_int(std::string file_name, int32 *key1);

	int32 get_int(std::string file_name, const char *key1, const char *key2);

	int32 get_int(std::string file_name, const char *key1, int key2);

	int32 get_int(std::string file_name, int *key1, const char* key2);

	int32 get_int(std::string file_name, int *key1, int key2);

	std::string get_string(std::string file_name, const char *key1);

	std::string get_string(std::string file_name, int32 *key1);

	std::string get_string(std::string file_name, const char *key1, const char *key2);

	std::string get_string(std::string file_name, const char *key1, int key2);

	std::string get_string(std::string file_name, int *key1, const char* key2);

	std::string get_string(std::string file_name, int *key1, int key2);

private:
	JsonReader *reader_;

	cfg_json_map_t jsons_;
};

#endif