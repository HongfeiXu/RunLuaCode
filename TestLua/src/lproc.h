#pragma once

// Refs: Programming in Lua, 4th ed. Ch33 Threads and States

// show the use of multiple Lua states in a multithreading context

#include <pthread\pthread.h>
#include <string.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma comment(lib, "pthreadVC2.lib")

// the structure that represents a process
typedef struct Proc {
	lua_State* L;
	pthread_t thread;
	pthread_cond_t cond;			// condition variable
	const char* channel;			// channel that the process is waiting
	struct Proc* previous, * next;	// link the process structure into a waiting list
}Proc;

// two waiting lists
static Proc* waitsend = NULL;
static Proc* waitreceive = NULL;
// associated mutex
static pthread_mutex_t kernel_access = PTHREAD_MUTEX_INITIALIZER;

#pragma region auxiliary function

// retrieve the Proc structure associated with a given state
static Proc* getself(lua_State* L)
{
	Proc* p;
	lua_getfield(L, LUA_REGISTRYINDEX, "_SELF");
	p = (Proc*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	return p;
}

// move values from a sender process to a receiver process
static void movevalues(lua_State* send, lua_State* rec)
{
	int n = lua_gettop(send);
	int i;
	luaL_checkstack(rec, n, "too many results");	// 扩展栈空间
	for (i = 2; i <= n; ++i)	// 栈中下标为1的元素是 channel，不需要拷贝
	{
		int t = lua_type(send, i);
		switch (t)
		{
		case LUA_TSTRING:
			lua_pushstring(rec, lua_tostring(send, i));
			break;
		case LUA_TNUMBER:
			lua_pushnumber(rec, lua_tonumber(send, i));
		case LUA_TBOOLEAN:
			lua_pushboolean(rec, lua_toboolean(send, i));
			break;
		case LUA_TFUNCTION:
			lua_pushcfunction(rec, lua_tocfunction(send, i));
			break;
		case LUA_TUSERDATA:
			lua_pushlightuserdata(rec, lua_touserdata(send, i));	// ???
			break;
		case LUA_TLIGHTUSERDATA:
			lua_pushlightuserdata(rec, lua_touserdata(send, i));	// ???
			break;
		case LUA_TTHREAD:
			lua_pushthread(lua_tothread(send, i));
			break;
		case LUA_TNIL:
			lua_pushnil(rec);
			break;
		case LUA_TTABLE:
			lua_newtable(rec);
			lua_pushnil(send);
			while (lua_next(send, i) != 0)
			{
				lua_pushnil(send);
				lua_copy(send, -3, -1);
				lua_xmove(send, rec, 2);
				lua_settable(rec, -3);
			}
			break;
		default:
			break;
		}

	}
}

// search for a process waiting for a channel
// traverse a list looking for a process that is waiting for a given channel
static Proc* searchmatch(const char* channel, Proc** list)
{
	Proc* node = *list;
	if (node == NULL)
	{
		return NULL;
	}
	// 遍历双向循环链表
	while (node->next != *list)
	{
		if (strcmp(channel, node->channel) == 0)	// match?
		{
			// remove node from the list(circular double-linked list)
			if (*list == node)	// is this node the first element?
			{
				if (node->next == node)	// is this node the only element?
				{
					*list = NULL;
				}
				else
				{
					*list = node->next;
				}
			}
			node->previous->next = node->next;
			node->next->previous = node->previous;
			return node;
		}
	}
	if (strcmp(channel, node->channel) == 0)	// match?
	{
		// remove node from the list(circular double-linked list)
		if (*list == node)	// is this node the first element?
		{
			if (node->next == node)	// is this node the only element?
			{
				*list = NULL;
			}
			else
			{
				*list = node->next;
			}
		}
		node->previous->next = node->next;
		node->next->previous = node->previous;
		return node;
	}

	return NULL;	// match not found
}

// add a process to a waiting list
// called when a process cannot find a match
static void waitonlist(lua_State* L, const char* channel, Proc** list)
{
	Proc* p = getself(L);

	// link itself at the end of the list
	if (*list == NULL)
	{
		*list = p;
		p->previous = p->next = p;
	}
	else
	{
		p->previous = (*list)->previous;
		p->next = *list;
		p->previous->next = p->next->previous = p;
	}

	p->channel = channel;	// waiting channel

	do {	// wait on its condition variable
		pthread_cond_wait(&p->cond, &kernel_access);
	} while (p->channel);	// when a process wakes another, it sets the other process's field channel to NULL
							// so, if p->channel is not NULL, it has to keep waiting
}

#pragma endregion

// functions to send and receive message
static int ll_send(lua_State* L)
{
	Proc* p;
	const char* channel = luaL_checkstring(L, 1);

	pthread_mutex_lock(&kernel_access);

	p = searchmatch(channel, &waitreceive);

	if (p)	// found a matching receiver?
	{
		movevalues(L, p->L);			// move values to receiver
		p->channel = NULL;				// mark receiver as not waiting
		pthread_cond_signal(&p->cond);	// wait it up
	}
	else
	{
		waitonlist(L, channel, &waitsend);
	}
	
	pthread_mutex_unlock(&kernel_access);
	return 0;
}

// function to create new process
static int ll_receive(lua_State* L)
{
	Proc* p;
	const char* channel = luaL_checkstring(L, 1);

	pthread_mutex_lock(&kernel_access);

	p = searchmatch(channel, &waitsend);

	if (p)
	{
		movevalues(p->L, L);			// get values from sender
		p->channel = NULL;				// mark sender as not waiting
		pthread_cond_signal(&p->cond);	// wake it up
	}
	else
	{
		waitonlist(L, channel, &waitreceive);
	}

	pthread_mutex_unlock(&kernel_access);

	return lua_gettop(L) - 1;			// return all stack values except the channel
}

static void* ll_thread(void* arg);		// A new process needs a new POSIX thread, 
										// and a new thread needs a body to run

// function to create new processes
// create a new Lua state; start a new thread; compile the given chunk
static int ll_start(lua_State* L)
{
	pthread_t thread;
	const char* chunk = luaL_checkstring(L, 1);
	lua_State* L1 = luaL_newstate();

	if (L1 == NULL)
	{
		luaL_error(L, "unable to create new state");
	}
	if (luaL_loadstring(L1, chunk) != LUA_OK)
	{
		luaL_error(L, "error in thread body %s", lua_tostring(L1, -1));
	}
	// create a new thread with body ll_thread, 
	// passing the new state L1 as the argument to the body
	if (pthread_create(&thread, NULL, ll_thread, L1) != 0)	
	{
		luaL_error(L, "unable to create new thread");
	}
	// tell the system that we will not want any final answer from this thread
	pthread_detach(thread);
	return 0;
}

int luaopen_lproc(lua_State* L);
void openlibs(lua_State* L);

// body for new threads
// receive its corresponding Lua state(created by ll_start) with only the precompiled main chunk on the stack
static void* ll_thread(void* arg)
{
	lua_State* L = (lua_State*)arg;
	Proc* self;		// own control block

	//luaL_openlibs(L);

	openlibs(L);	// register libraries to be opened on demand

	luaL_requiref(L, "lproc", luaopen_lproc, 1);	// open the lproc library
	lua_pop(L, 1);	// remove result from previous call

	// create and initialize its own control block
	self = (Proc*)lua_newuserdata(L, sizeof(Proc));
	lua_setfield(L, LUA_REGISTRYINDEX, "_SELF");
	self->L = L;
	self->thread = pthread_self();
	self->channel = NULL;
	pthread_cond_init(&self->cond, NULL);

	// call main chunk
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		fprintf(stderr, "thread error: %s", lua_tostring(L, -1));
	}

	// destroy condition variable and close its Lua state
	pthread_cond_destroy(&getself(L)->cond);	// &self->cond 可以吗？
	lua_close(L);
	return NULL;
}

// extra functions for lproc module

static int ll_exit(lua_State* L)
{
	pthread_exit(NULL);
	return 0;
}

static const struct luaL_Reg ll_funcs[] = {
	{"start",  ll_start},
	{"send", ll_send},
	{"receive", ll_receive},
	{"exit", ll_exit},
	{NULL, NULL}
};

int luaopen_lproc(lua_State* L)
{
	luaL_newlib(L, ll_funcs);	// open library
	return 1;
}

// Registering libraries to be opened on demand
static void registerlib(lua_State* L, const char* name, lua_CFunction f)
{
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");		// get package.preload
	lua_pushcfunction(L, f);		// [package - preload - f]
	lua_setfield(L, -2, name);		// [package - preload] package.preload[name] = f
	lua_pop(L, 2);	// pop "package" and "preload" tables
}

static void openlibs(lua_State* L)
{
	luaL_requiref(L, "_G", luaopen_base, 1);
	luaL_requiref(L, "package", luaopen_package, 1);
	lua_pop(L, 2);
	registerlib(L, "coroutine", luaopen_coroutine);
	registerlib(L, "table", luaopen_table);
	registerlib(L, "io", luaopen_io);
	registerlib(L, "os", luaopen_os);
	registerlib(L, "string", luaopen_string);
	registerlib(L, "math", luaopen_math);
	registerlib(L, "utf8", luaopen_utf8);
	registerlib(L, "debug", luaopen_debug);
}

void test()
{
	lua_State* L = luaL_newstate();
	
	//luaL_openlibs(L);

	openlibs(L);	// register libraries to be opened on demand

	luaL_requiref(L, "lproc", luaopen_lproc, 1);
	lua_pop(L, 1);

	if (luaL_dofile(L, "lua/lproc.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}


