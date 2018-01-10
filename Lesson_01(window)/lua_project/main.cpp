
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif
#include "luaJit/lua.h"
#include "luaJit/lualib.h"
#include "luaJit/lauxlib.h"
#ifdef __cplusplus
}
#endif


#pragma comment(lib,"luaCompile.lib")




void    printTable(lua_State* pState, int index)
{
	int top = lua_gettop(pState);

	/// 把栈上给定索引处的元素作一个副本压栈。
	lua_pushvalue(pState, index);

	fprintf(stdout, "{\n");

	lua_pushnil(pState);

	while (lua_next(pState, -2))
	{
		fprintf(stdout, "\t");
		int type = lua_type(pState, -2);
		switch (type)
		{
		case LUA_TNUMBER:
			fprintf(stdout, "%g", lua_tonumber(pState, -2));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d", int(lua_toboolean(pState, -2)));
			break;
		case LUA_TSTRING:
			fprintf(stdout, "%s", lua_tostring(pState, -2));
			break;
		default:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -2));
			break;
		}

		fprintf(stdout, "\t\t=\t");

		type = lua_type(pState, -1);
		switch (type)
		{
		case LUA_TNUMBER:
			fprintf(stdout, "%g", lua_tonumber(pState, -1));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d", int(lua_toboolean(pState, -1)));
			break;
		case LUA_TSTRING:
			fprintf(stdout, "%s", lua_tostring(pState, -1));
			break;
		case  LUA_TFUNCTION:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
			break;
		case LUA_TUSERDATA:
			fprintf(stdout, "%s:%s", lua_typename(pState, type), lua_tostring(pState, -1));
			break;
		default:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
		}

		fprintf(stdout, "\n");

		lua_pop(pState, 1);
	}

	fprintf(stdout, "}\n");

	lua_settop(pState, top);
}

void    stackDump(lua_State *l)
{
	int i;
	int top = lua_gettop(l);
	printf("------start-----%d\n", top);

	for (i = 1; i <= top; i++)
	{
		int t = lua_type(l, i);
		printf("type: %s value:", lua_typename(l, t));

		switch (t)
		{
		case LUA_TSTRING:
			printf("%s", lua_tostring(l, i));
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(l, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(l, i));
			break;
		case LUA_TTABLE:
			printTable(l, i);
			break;
		default:
			printf("%s", lua_typename(l, t));
			break;
		}
		printf("\n");
	}

	printf("------end----\n");
}



class Enemy
{
protected:
	char    _name[32];
	int     _life;
public:
	Enemy(const char *name)
	{
		strcpy(_name, name);
		_life = 0;
	}
	~Enemy(void)
	{}
	void    setLife(int life)
	{
		_life = life;
	}
	int     getLife()
	{
		return  _life;
	}
};


class LuaEnemy
{
public:
	LuaEnemy()
	{

	}
	~LuaEnemy(void)
	{

	}
	static void Register(lua_State *L)
	{
		//1. new method table 保存所有的函数
		lua_newtable(L);

		int methodTable = lua_gettop(L);
		//2. new metatable for L to save "__metatable", "__index",  "__gc", etc
		luaL_newmetatable(L, "Enemy");

		int metaTable = lua_gettop(L);

		//3. metatable["__metatable"] = methodtable;同 lua_settable(1,metatable)
		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methodTable);
		lua_rawset(L, metaTable);

		//4. metatable["__index"] = methodtable ;同 lua_settable(1,metatable)
		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methodTable);
		lua_rawset(L, metaTable);

		//5. metatable["__gc"] = myFunction  
		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, gc_Enemy);
		lua_rawset(L, metaTable);

		lua_pop(L, 1);


		lua_pushcclosure(L, LuaEnemy::setLife, 0);
		lua_setfield(L, -2, "setLife");

		lua_pop(L, 1);

		lua_register(L, "Enemy", LuaEnemy::create);
		stackDump(L);
	}

	static int create(lua_State *L)
	{
		stackDump(L);
		char*   name = (char*)lua_tostring(L, -1);
		Enemy*  a = new Enemy(name);
		void**  p = (void**)lua_newuserdata(L, sizeof(void*));
		*p = a;

		luaL_getmetatable(L, "Enemy");
		stackDump(L);
		lua_setmetatable(L, -2);
		stackDump(L);
		return 1;
	}
	static int gc_Enemy(lua_State *L)
	{
		stackDump(L);
		Enemy*  enemy = (Enemy*)(*(void**)lua_touserdata(L, -1));
		stackDump(L);
		delete  enemy;
		return  0;
	}

	static int setLife(lua_State *L)
	{
		stackDump(L);
		int     life = lua_tonumber(L, 2);
		luaL_checktype(L, 1, LUA_TUSERDATA);
		void*   ud = luaL_checkudata(L, 1, "Enemy");
		Enemy*  enemy = *(Enemy**)ud;
		enemy->setLife(life);
		return  0;
	}
	static int getLife(lua_State *L)
	{
		return  1;
	}
};


int main(int argc, char** argv)
{
	lua_State*  L = luaL_newstate();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

	LuaEnemy::Register(L);

	if (luaL_dofile(L, "test.lua"))
	{
		stackDump(L);
	}
	lua_close(L);

	scanf("%s", "");
	return  0;
}