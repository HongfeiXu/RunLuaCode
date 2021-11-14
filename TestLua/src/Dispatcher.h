#pragma once

#include "LuaUtils.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

const int EVENT_TYPE_NUMBER = 0;
const int EVENT_TYPE_STRING = 1;

struct Event {
	int eventType;
	int eventNumber;
	std::string eventString;
	
	Event(int _eventType, int _eventNumber=0, std::string _eventString="")
		: eventType(_eventType), eventNumber(_eventNumber), eventString(_eventString)
	{
	}
};

struct EventHash {
	std::size_t operator()(const Event& k) const
	{
		if(k.eventType == EVENT_TYPE_NUMBER)
			return std::hash<int>()(k.eventType) ^ (std::hash<int>()(k.eventNumber) << 1);
		else
			return std::hash<int>()(k.eventType) ^ (std::hash<std::string>()(k.eventString) << 1);

	}
};

struct EventEqual {
	bool operator()(const Event& lhs, const Event& rhs) const
	{
		return lhs.eventType == rhs.eventType && 
			lhs.eventNumber == rhs.eventNumber && 
			lhs.eventString == rhs.eventString;
	}
};


struct Listener {
	std::shared_ptr<Event> pEvent;
	int callbackRef;		// store a callback ref£¬²Î¿¼ ExpatBinding.h
};

struct Dispatcher {
	int ID;
	std::unordered_map<Event, std::unordered_set<std::shared_ptr<Listener>>, EventHash, EventEqual> event2Listeners;
};




void test()
{
	Listener listener;
	std::cout << listener.callbackRef << std::endl;

	Event a = Event(0, 1, "");
	Event b = Event(0, 1, "sdaf");
	std::unordered_map<Event, int, EventHash, EventEqual> aha;
	aha.insert(std::make_pair(a, 10));
	aha.insert(std::make_pair(b, 10));
	for (auto iter : aha)
	{
		std::cout << iter.first.eventString << " " << iter.second << std::endl;
	}

	std::unordered_set<std::shared_ptr<Listener>> lala;
	std::shared_ptr<Listener> p = std::make_shared<Listener>(listener);
	lala.insert(p);

}
