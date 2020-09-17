/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaTopRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaTopRightChanger::AlphaTopRightChanger(chstr name) :
			Animator(name)
		{
		}

		AlphaTopRightChanger::AlphaTopRightChanger(const AlphaTopRightChanger& other) :
			Animator(other)
		{
		}

		Animator* AlphaTopRightChanger::createInstance(chstr name)
		{
			return new AlphaTopRightChanger(name);
		}

		float AlphaTopRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getAlphaTopRight();
		}

		void AlphaTopRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setAlphaTopRight((unsigned char)value);
		}

		void AlphaTopRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
