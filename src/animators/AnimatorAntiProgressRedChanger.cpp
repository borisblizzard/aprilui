/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAntiProgressRedChanger.h"
#include "Object.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	namespace Animators
	{
		AntiProgressRedChanger::AntiProgressRedChanger(chstr name) :
			Animator(name)
		{
		}

		AntiProgressRedChanger::AntiProgressRedChanger(const AntiProgressRedChanger& other) :
			Animator(other)
		{
		}

		Animator* AntiProgressRedChanger::createInstance(chstr name)
		{
			return new AntiProgressRedChanger(name);
		}

		float AntiProgressRedChanger::_getObjectValue() const
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressRedChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)progressObject->getAntiProgressRed();
		}

		void AntiProgressRedChanger::_setObjectValue(float value)
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressRedChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			progressObject->setAntiProgressRed((unsigned char)value);
		}

		void AntiProgressRedChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
