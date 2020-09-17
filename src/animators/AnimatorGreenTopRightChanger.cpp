/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenTopRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		GreenTopRightChanger::GreenTopRightChanger(chstr name) :
			Animator(name)
		{
		}

		GreenTopRightChanger::GreenTopRightChanger(const GreenTopRightChanger& other) :
			Animator(other)
		{
		}

		Animator* GreenTopRightChanger::createInstance(chstr name)
		{
			return new GreenTopRightChanger(name);
		}

		float GreenTopRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getGreenTopRight();
		}

		void GreenTopRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setGreenTopRight((unsigned char)value);
		}

		void GreenTopRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
