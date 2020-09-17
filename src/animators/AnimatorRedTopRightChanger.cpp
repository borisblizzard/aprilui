/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorRedTopRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		RedTopRightChanger::RedTopRightChanger(chstr name) :
			Animator(name)
		{
		}

		RedTopRightChanger::RedTopRightChanger(const RedTopRightChanger& other) :
			Animator(other)
		{
		}

		Animator* RedTopRightChanger::createInstance(chstr name)
		{
			return new RedTopRightChanger(name);
		}

		float RedTopRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::RedTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getRedTopRight();
		}

		void RedTopRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::RedTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setRedTopRight((unsigned char)value);
		}

		void RedTopRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
