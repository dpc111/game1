#ifndef LUA_FRAME_H
#define LUA_FRAME_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

class lua_frame_t {
public:
	typedef std::map<const char *, int> func_map_t;

public:
	lua_frame_t();

	~lua_frame_t();

	lua_State *get_lua_state();

	int load_string(const char *str);

	int load_script(const char *file);

	int run_script();

	static int traceback(lua_State *l);

	bool call_func(const char *funcname, const char *args, ...);

	bool call_func(const char *funcname, const char *args, va_list vlist);

	bool run_func(const char *funcname, const char *fmt, va_list vlist);

	int run_func(const char *funcname, int nargs, int nres, int errfunc);
private:
	func_map_t *func_map_;

	lua_State *lua_state_;
};

#endif