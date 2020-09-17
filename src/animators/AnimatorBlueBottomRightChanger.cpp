/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorBlueBottomRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		BlueBottomRightChanger::BlueBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		BlueBottomRightChanger::BlueBottomRightChanger(const BlueBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* BlueBottomRightChanger::createInstance(chstr name)
		{
			return new BlueBottomRightChanger(name);
		}

		float BlueBottomRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::BlueBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getBlueBottomRight();
		}

		void BlueBottomRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::BlueBottomRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setBlueBottomRight((unsigned char)value);
		}

		void BlueBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
