/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaBottomRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaBottomRightChanger::AlphaBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		AlphaBottomRightChanger::AlphaBottomRightChanger(const AlphaBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* AlphaBottomRightChanger::createInstance(chstr name)
		{
			return new AlphaBottomRightChanger(name);
		}

		float AlphaBottomRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getAlphaBottomRight();
		}

		void AlphaBottomRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setAlphaBottomRight((unsigned char)value);
		}

		void AlphaBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
