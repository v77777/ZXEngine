#pragma once
#include "../Component/Animator.h"

extern "C"
{
#include "../External/Lua/lua.h"
#include "../External/Lua/lualib.h"
#include "../External/Lua/lauxlib.h"
}

static int Play(lua_State* L)
{
	ZXEngine::Animator** animator = (ZXEngine::Animator**)luaL_checkudata(L, -2, "ZXEngine.Animator");

	const char* animationName = lua_tostring(L, -1);

	(*animator)->Play(animationName);

	return 0;
}

static int Switch(lua_State* L)
{
	int argc = lua_gettop(L);

	if (argc == 2)
	{
		ZXEngine::Animator** animator = (ZXEngine::Animator**)luaL_checkudata(L, -2, "ZXEngine.Animator");

		const char* animationName = lua_tostring(L, -1);

		(*animator)->Switch(animationName);
	}
	else if (argc == 3)
	{
		ZXEngine::Animator** animator = (ZXEngine::Animator**)luaL_checkudata(L, -3, "ZXEngine.Animator");

		const char* animationName = lua_tostring(L, -2);

		float time = (float)lua_tonumber(L, -1);

		(*animator)->Switch(animationName, time);
	}
	else
	{
		ZXEngine::Debug::LogError("No matched lua warp function to call: ZXEngine::Animator::Switch");
	}

	return 0;
}

static const luaL_Reg Animator_Funcs[] = {
	{NULL, NULL}
};

static const luaL_Reg Animator_Funcs_Meta[] = {
	{"Play", Play},
	{"Switch", Switch},
	{NULL, NULL}
};

LUAMOD_API int luaopen_Animator(lua_State* L) {
	luaL_newmetatable(L, "ZXEngine.Animator"); /* ����Ԫ�� */
	lua_pushvalue(L, -1); /* ����Ԫ���������Ϊ��һ�д������׼������ */
	lua_setfield(L, -2, "__index"); /* mt.__index = mt ��-2λ���ϵ�mt��(��һ��newmetatable��)��__index�ֶ�����Ϊmt(�ڶ���pushvalue���Ƶ�)*/
	luaL_setfuncs(L, Animator_Funcs_Meta, 0); /* ע��Ԫ���� */

	luaL_newlib(L, Animator_Funcs);
	return 1;
}