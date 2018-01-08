#ifndef LUA_DATA_H
#define LUA_DATA_H

class lua_data_t {
public:
	typedef std::map<const char *, int> func_map_t;

public:
	lua_data_t();

	~lua_data_t();

	lua_State *get_lua_state();

	int load_string(const char *str);

	int load_script(const char *file);

	int run_script();

	int run_func(const char *func, int nargs, int nres, int errfunc);

	bool run_func(const char *func, const char *args, ...);

	bool run_func(const char *funcname, const char *args, va_list vlist);

	bool call_func(const char *funcname, const char *fmt, va_list vlist);

private:
	func_map_t *func_map_;
	lua_State *lua_state_;


}

#endif