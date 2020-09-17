/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorRedBottomLeftChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		RedBottomLeftChanger::RedBottomLeftChanger(chstr name) :
			Animator(name)
		{
		}

		RedBottomLeftChanger::RedBottomLeftChanger(const RedBottomLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* RedBottomLeftChanger::createInstance(chstr name)
		{
			return new RedBottomLeftChanger(name);
		}

		float RedBottomLeftChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::RedBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getRedBottomLeft();
		}

		void RedBottomLeftChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::RedBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setRedBottomLeft((unsigned char)value);
		}

		void RedBottomLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
