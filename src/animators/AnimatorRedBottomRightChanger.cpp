/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorRedBottomRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		RedBottomRightChanger::RedBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		RedBottomRightChanger::RedBottomRightChanger(const RedBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* RedBottomRightChanger::createInstance(chstr name)
		{
			return new RedBottomRightChanger(name);
		}

		float RedBottomRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::RedBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getRedBottomRight();
		}

		void RedBottomRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::RedBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setRedBottomRight((unsigned char)value);
		}

		void RedBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
