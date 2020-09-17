/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenBottomLeftChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		GreenBottomLeftChanger::GreenBottomLeftChanger(chstr name) :
			Animator(name)
		{
		}

		GreenBottomLeftChanger::GreenBottomLeftChanger(const GreenBottomLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* GreenBottomLeftChanger::createInstance(chstr name)
		{
			return new GreenBottomLeftChanger(name);
		}

		float GreenBottomLeftChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getGreenBottomLeft();
		}

		void GreenBottomLeftChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setGreenBottomLeft((unsigned char)value);
		}

		void GreenBottomLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
