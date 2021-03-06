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
/// Defines a callback object.

#ifndef APRILUI_CALLBACK_OBJECT_H
#define APRILUI_CALLBACK_OBJECT_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport CallbackObject : public Object
	{
		APRILUI_CLONEABLE(CallbackObject);
	public:
		CallbackObject(chstr name);
		inline hstr getClassName() const override { return "CallbackObject"; }
		static Object* createInstance(chstr name);

		inline void (*getDrawCallback())(CallbackObject*) { return this->drawCallback; }
		inline void setDrawCallback(void (*value)(CallbackObject*)) { this->drawCallback = value; }
		inline void setUpdateCallback(void (*value)(float)) { this->updateCallback = value; }
		
	protected:
		void (*drawCallback)(CallbackObject*);
		void (*updateCallback)(float);

		void _update(float timeDelta) override;
		void _draw() override;

	};

}
#endif
