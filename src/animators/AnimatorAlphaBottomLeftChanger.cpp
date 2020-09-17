/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaBottomLeftChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaBottomLeftChanger::AlphaBottomLeftChanger(chstr name) :
			Animator(name)
		{
		}

		AlphaBottomLeftChanger::AlphaBottomLeftChanger(const AlphaBottomLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* AlphaBottomLeftChanger::createInstance(chstr name)
		{
			return new AlphaBottomLeftChanger(name);
		}

		float AlphaBottomLeftChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getAlphaBottomLeft();
		}

		void AlphaBottomLeftChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setAlphaBottomLeft((unsigned char)value);
		}

		void AlphaBottomLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
