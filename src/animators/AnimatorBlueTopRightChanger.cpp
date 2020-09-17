/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorBlueTopRightChanger.h"
#include "ObjectColored.h"

namespace aprilui
{
	namespace Animators
	{
		BlueTopRightChanger::BlueTopRightChanger(chstr name) :
			Animator(name)
		{
		}

		BlueTopRightChanger::BlueTopRightChanger(const BlueTopRightChanger& other) :
			Animator(other)
		{
		}

		Animator* BlueTopRightChanger::createInstance(chstr name)
		{
			return new BlueTopRightChanger(name);
		}

		float BlueTopRightChanger::_getObjectValue() const
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::BlueTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)coloredObject->getBlueTopRight();
		}

		void BlueTopRightChanger::_setObjectValue(float value)
		{
			Colored* coloredObject = dynamic_cast<Colored*>(this->parent);
			if (coloredObject == NULL)
			{
				hlog::errorf(logTag, "Animators::BlueTopRightChanger: parent object '%s' not a subclass of Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			coloredObject->setBlueTopRight((unsigned char)value);
		}

		void BlueTopRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
