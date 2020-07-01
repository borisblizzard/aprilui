/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorProgressAlphaChanger.h"
#include "Object.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	namespace Animators
	{
		ProgressAlphaChanger::ProgressAlphaChanger(chstr name) :
			Animator(name)
		{
		}

		ProgressAlphaChanger::ProgressAlphaChanger(const ProgressAlphaChanger& other) :
			Animator(other)
		{
		}

		Animator* ProgressAlphaChanger::createInstance(chstr name)
		{
			return new ProgressAlphaChanger(name);
		}

		float ProgressAlphaChanger::_getObjectValue() const
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::ProgressAlphaChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return (float)progressObject->getProgressAlpha();
		}

		void ProgressAlphaChanger::_setObjectValue(float value)
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::ProgressAlphaChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			progressObject->setProgressAlpha((unsigned char)value);
		}

		void ProgressAlphaChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
