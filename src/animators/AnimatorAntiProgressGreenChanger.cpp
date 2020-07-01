/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAntiProgressGreenChanger.h"
#include "Object.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	namespace Animators
	{
		AntiProgressGreenChanger::AntiProgressGreenChanger(chstr name) :
			Animator(name)
		{
		}

		AntiProgressGreenChanger::AntiProgressGreenChanger(const AntiProgressGreenChanger& other) :
			Animator(other)
		{
		}

		Animator* AntiProgressGreenChanger::createInstance(chstr name)
		{
			return new AntiProgressGreenChanger(name);
		}

		float AntiProgressGreenChanger::_getObjectValue() const
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressGreenChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)progressObject->getAntiProgressGreen();
		}

		void AntiProgressGreenChanger::_setObjectValue(float value)
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressGreenChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			progressObject->setAntiProgressGreen((unsigned char)value);
		}

		void AntiProgressGreenChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
