#include "lua_data.h"

lua_State *lua_state_t::get_lua_state() {
	return lua_state_;
}

int lua_data_t::load_string(const char *str) {
	if (luaL_loadstring(lua_state_)) {
		ERROR("%d\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0
}

int lua_data_t::load_script(const char *file) {
	if (luaL_loadfile(lua_state_, file)) {
		ERROR("%s\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_data_t::run_script() {
	if (lua_pcall(lua_state_, 0, LUA_MULTRET, 0)) {
		ERROR("%d\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

bool lua_data_t::call_func(const char *funcname, const char *fmt, va_list vlist) {
	lua_getglobal(lua_state_, funcname);
	if (!lua_isfunction(lua_state_, -1)) {
		return false;
	}
	lua_pushvalue(lua_state_, -1);
	int nargs = lua_gettop(lua_state_);
	
}