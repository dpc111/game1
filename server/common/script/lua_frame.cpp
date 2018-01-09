#include "lua_frame.h"
#include "log.h"
#include "type.h"

lua_State *lua_frame_t::get_lua_state() {
	return lua_state_;
}

int lua_frame_t::load_string(const char *str) {
	if (luaL_loadstring(lua_state_, str)) {
		ERROR("%s\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_frame_t::load_script(const char *file) {
	if (luaL_loadfile(lua_state_, file)) {
		ERROR("%s\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_frame_t::run_script() {
	if (lua_pcall(lua_state_, 0, LUA_MULTRET, 0)) {
		ERROR("%d\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_frame_t::traceback(lua_State *l) {
	ERROR(lua_tostring(l, -1));
	return 1;
}

bool lua_frame_t::call_func(const char *funcname, const char *args, ...) {
	va_list vlist;
	va_start(vlist, args);
	bool res = call_func(funcname, args, vlist);
	va_end(vlist);
	return res;
}

bool lua_frame_t::call_func(const char *funcname, const char *args, va_list vlist) {
	int top = lua_gettop(lua_state_);
	bool res = run_func(funcname, args, vlist);
	lua_settop(lua_state_, top);
	return res;
}

bool lua_frame_t::run_func(const char *funcname, const char *fmt, va_list vlist) {
	lua_getglobal(lua_state_, funcname);
	if (!lua_isfunction(lua_state_, -1)) {
		return false;
	}
	lua_pushvalue(lua_state_, -1);
	int nargs = lua_gettop(lua_state_);
	while(*fmt != '\0') {
		switch(*fmt) {
		case 'b' :
			lua_pushboolean(lua_state_, va_arg(vlist, int));
			break;
		case 'i' :
			lua_pushnumber(lua_state_, va_arg(vlist, int));
			break;
		case 'l' :
			lua_pushnumber(lua_state_, (double) va_arg(vlist, int64));
			break;
		case 'd' :
			lua_pushnumber(lua_state_, (double) va_arg(vlist, double));
			break;
		case 's' :
			lua_pushstring(lua_state_, (char *) va_arg(vlist, char *));
			break;
		case 'p' :
			lua_pushlightuserdata(lua_state_, va_arg(vlist, void *));
			break;
		case ':' :
			break;
		default :
			return false;
		}
		if (*fmt == ':') {
			++fmt;
			break;
		}
		++fmt;
	}

	nargs = lua_gettop(lua_state_) - nargs;
	int nres = 0;
	const char *walk = fmt;
	while (*walk != '\0') {
		nres += 1;
		++walk;
	}
	lua_pushcfunction(lua_state_, traceback);
	lua_insert(lua_state_, -nargs - 2);
	if (lua_pcall(lua_state_, nargs, nres, -nres - 2) != 0) {
		return false;
	}
	while (*fmt != '\0') {
		switch (*fmt) {
		case 'b' :
			*va_arg(vlist, bool *) = lua_toboolean(lua_state_, -nres) != 0;
			break;
		case 'i' :
			*va_arg(vlist, int *) = (int) lua_tonumber(lua_state_, -nres);
			break;
		case 'l' :
			*va_arg(vlist, int64 *) = (int64) lua_tonumber(lua_state_, -nres);
			break;
		case 'd' :
			*va_arg(vlist, double *) = (double) lua_tonumber(lua_state_, -nres);
			break;
		case 's' :
			*va_arg(vlist, const char **) = lua_tostring(lua_state_, -nres);
			break;
		case 'p' :
			*va_arg(vlist, const void **) = aux_topointer(lua_state_, -nres);
			break;
		default :
			return false;
		}
		++fmt;
		if (--nres == 0) {
			break;
		}
	}
	return true;
}

int lua_frame_t::run_func(const char *funcname, int nargs, int nres, int errfunc) {
	return lua_pcall(funcname, nargs, nres, errfunc);
}
