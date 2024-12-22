#pragma once

#include <iostream>
#include <string>

//#ifdef _WIN32
//#pragma comment(lib, "lua/lua53.lib")
//#endif // _WIN32



extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

#include <Lua5.4.6/lua.h>


bool CheckLua(lua_State* L, int r);

int lua_HostFunction(lua_State* L);