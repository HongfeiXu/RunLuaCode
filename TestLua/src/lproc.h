#pragma once

// Refs: Programming in Lua, 4th ed. Ch33 Threads and States

// show the use of multiple Lua states in a multithreading context

#include <pthread.h>
#include <string.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

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
		lua_pushstring(rec, lua_tostring(send, i));
	}
}

// search for a process waiting for a channel
// traverse a list looking for a process that is waiting for a given channel
static Proc* searchmatch(const char* channel, Proc** list)
{
	Proc* node;
	// traverse the list
	for (node = *list; node != NULL; node = node->next)
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

// TODO: 

// function to create new process

// body for new threads

// extra functions for lproc module

// registering libraries to be opened on demand



