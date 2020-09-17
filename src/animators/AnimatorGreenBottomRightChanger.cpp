/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenBottomRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		GreenBottomRightChanger::GreenBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		GreenBottomRightChanger::GreenBottomRightChanger(const GreenBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* GreenBottomRightChanger::createInstance(chstr name)
		{
			return new GreenBottomRightChanger(name);
		}

		float GreenBottomRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getGreenBottomRight();
		}

		void GreenBottomRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setGreenBottomRight((unsigned char)value);
		}

		void GreenBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
