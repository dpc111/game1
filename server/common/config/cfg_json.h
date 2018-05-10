#ifndef CFG_JSON_H
#define CFG_JSON_H

#include "json/json.h"
#include "cfg_json_mgr.h"

class cfg_json_t
{
public:
	cfg_json_t(cfg_json_mgr_t *mgr);

	~cfg_json_t();

	bool init(const char *file_name);

	int get_int(const char *key1);

	int get_int(int key1);

	int get_int(const char *key1, const char *key2);

	int get_int(const char *key1, int key2);

	int get_int(int key1, const char* key2);

	int get_int(int key1, int key2);

	std::string get_string(const char *key1);

	std::string get_string(int key1);

	std::string get_string(const char *key1, const char *key2);

	std::string get_string(const char *key1, int key2);

	std::string get_string(int key1, const char* key2);

	std::string get_string(int key1, int key2);
private:
	cfg_json_mgr_t *mgr_;

	const char *file_name_;

	Json::Value json_value_;
};

#endif