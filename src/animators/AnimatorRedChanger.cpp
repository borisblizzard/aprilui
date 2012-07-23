/// @file
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "AnimatorRedChanger.h"

namespace aprilui
{
	namespace Animators
	{
		RedChanger::RedChanger(chstr name) : Animator(name)
		{
		}

		RedChanger::~RedChanger()
		{
		}

		Animator* RedChanger::createInstance(chstr name)
		{
			return new RedChanger(name);
		}

		void RedChanger::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getRed();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void RedChanger::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				this->mValue = hclamp(this->_calculateValue(this->mTimeSinceLastFrame), 0.0f, 255.0f);
				if ((unsigned char)this->mValue != this->mParent->getRed())
				{
					this->mParent->setRed((unsigned char)this->mValue);
				}
			}
		}
		
	}
}
