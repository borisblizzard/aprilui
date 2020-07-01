/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAntiProgressAlphaChanger.h"
#include "Object.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	namespace Animators
	{
		AntiProgressAlphaChanger::AntiProgressAlphaChanger(chstr name) :
			Animator(name)
		{
		}

		AntiProgressAlphaChanger::AntiProgressAlphaChanger(const AntiProgressAlphaChanger& other) :
			Animator(other)
		{
		}

		Animator* AntiProgressAlphaChanger::createInstance(chstr name)
		{
			return new AntiProgressAlphaChanger(name);
		}

		float AntiProgressAlphaChanger::_getObjectValue() const
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressAlphaChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)progressObject->getAntiProgressAlpha();
		}

		void AntiProgressAlphaChanger::_setObjectValue(float value)
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::AntiProgressAlphaChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			progressObject->setAntiProgressAlpha((unsigned char)value);
		}

		void AntiProgressAlphaChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
