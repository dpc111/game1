cfg_json_t::cfg_json_t(cfg_json_mgr_t *mgr) {
	mgr_ = mgr;
	file_name_ = NULL;
}

cfg_json_t::~cfg_json_t() {
	mgr_ = NULL;
	file_name_ = NULL;
}

void cfg_json_t::init(const char *file_name) {
	file_name_ = file_name;
	std::ifstream ifs;
	ifs.open(file_name, std::ios::binary);
	if (!reader.parse(ifs, root, false))
	{
		ifs.close();
		return false;
	}
	ifs.close();
	return true;
}

int32 cfg_json_t::get_int(const char *key1) {
	return json_value_[key1].asInt();
}

int32 cfg_json_t::get_int(int32 *key1) {
	return json_value_[key1].asInt();
}

int32 cfg_json_t::get_int(const char *key1, const char *key2) {
	return json_value_[key1][key2].asInt();
}

int32 cfg_json_t::get_int(const char *key1, int key2) {
	return json_value_[key1][key2].asInt();
}

int32 cfg_json_t::get_int(int *key1, const char* key2) {
	return json_value_[key1][key2].asInt();
}

int32 cfg_json_t::get_int(int *key1, int key2) {
	return json_value_[key1][key2].asInt();
}

std::string cfg_json_t::get_string(const char *key1) {
	return json_value_[key1].asString();
}

std::string cfg_json_t::get_string(int32 *key1) {
	return json_value_[key1].asString();
}

std::string cfg_json_t::get_string(const char *key1, const char *key2) {
	return json_value_[key1][key2].asString();
}

std::string cfg_json_t::get_string(const char *key1, int key2) {
	return json_value_[key1][key2].asString();
}

std::string cfg_json_t::get_string(int *key1, const char* key2) {
	return json_value_[key1][key2].asString();
}

std::string cfg_json_t::get_string(int *key1, int key2) {
	return json_value_[key1][key2].asString();
}

// //从文件中解析json
// bool read_json_from_file(const char* filename)
// {
// 	JsonReader reader; 
// 	JsonValue root;        

// 	std::ifstream ifs;
// 	ifs.open(filename, std::ios::binary);
// 	if (!reader.parse(ifs, root, false))
// 	{
// 		ifs.close();
// 		return false;
// 	}
// 	ifs.close();
// 	return true;

// 	// std::string code;
// 	// if (!root["recent_games"].isNull())
// 	// 	code = root["user"].asString();

// 	// code = root.get("user", "null").asString();
// 	// cout << code << endl;

// 	// int count = root["recent_games"].size();// 得到"recent_games"的数组个数  
// 	// for (int i = 0; i < count; ++i)// 遍历数组
// 	// {
// 	// 	JsonValue date = root["recent_games"][i]["date"];
// 	// 	JsonValue time = root["recent_games"][i]["time"];

// 	// 	std::string sz_date = date.asString();
// 	// 	std::string sz_time = time.asString();
// 	// 	printf("date: %s, time: %s\r\n", sz_date.c_str(), sz_time.c_str());

// 	// 	JsonValue games = root["recent_games"][i]["games"];
// 	// 	int game_size = games.size();
// 	// 	for (int j = 0; j < game_size; ++j)
// 	// 	{
// 	// 		std::string url = games[j]["url"].asString();
// 	// 		std::string name = games[j]["name"].asString();
// 	// 		std::string server = games[j]["server"].asString();
// 	// 		printf("url : %s, name : %s, server : %s \n", url.c_str(), name.c_str(), server.c_str());
// 	// 	}
// 	// 	printf("\r\n");
// 	// }
// }