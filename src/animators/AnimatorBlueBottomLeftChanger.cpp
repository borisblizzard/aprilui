/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorBlueBottomLeftChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		BlueBottomLeftChanger::BlueBottomLeftChanger(chstr name) :
			Animator(name)
		{
		}

		BlueBottomLeftChanger::BlueBottomLeftChanger(const BlueBottomLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* BlueBottomLeftChanger::createInstance(chstr name)
		{
			return new BlueBottomLeftChanger(name);
		}

		float BlueBottomLeftChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::BlueBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getBlueBottomLeft();
		}

		void BlueBottomLeftChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::BlueBottomLeftChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setBlueBottomLeft((unsigned char)value);
		}

		void BlueBottomLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
