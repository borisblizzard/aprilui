/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for classes that can receive and handle events.

#ifndef APRILUI_EVENT_RECEIVER_H
#define APRILUI_EVENT_RECEIVER_H

#include <april/Keys.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Cloneable.h"
#include "Event.h"

namespace aprilui
{
	class Dataset;
	class EventArgs;

	class apriluiExport EventReceiver : public Cloneable
	{
		APRILUI_CLONEABLE_ABSTRACT_DERIVED(EventReceiver);
	public:
		EventReceiver();
		~EventReceiver();

		virtual hstr getName() const = 0;
		inline hmap<hstr, Event*>& getEvents() { return this->events; }

		bool registerEvent(chstr type, void(*callback)(EventArgs*));
		bool registerEvent(chstr type, Event* event);
		bool unregisterEvent(chstr type);
		bool isEventRegistered(chstr type) const;

		virtual void notifyEvent(chstr type, EventArgs* args);

		virtual bool triggerEvent(chstr type, april::Key keyCode);
		virtual bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		virtual bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL);
		virtual bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		virtual bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		virtual bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		Dataset* dataset;
		hmap<hstr, Event*> events;

	};

}
#endif
