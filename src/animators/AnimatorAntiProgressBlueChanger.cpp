/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAntiProgressBlueChanger.h"
#include "Object.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	namespace Animators
	{
		AntiProgressBlueChanger::AntiProgressBlueChanger(chstr name) :
			Animator(name)
		{
		}

		AntiProgressBlueChanger::AntiProgressBlueChanger(const AntiProgressBlueChanger& other) :
			Animator(other)
		{
		}

		Animator* AntiProgressBlueChanger::createInstance(chstr name)
		{
			return new AntiProgressBlueChanger(name);
		}

		float AntiProgressBlueChanger::_getObjectValue() const
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressBlueChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)progressObject->getAntiProgressBlue();
		}

		void AntiProgressBlueChanger::_setObjectValue(float value)
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressBlueChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			progressObject->setAntiProgressBlue((unsigned char)value);
		}

		void AntiProgressBlueChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
